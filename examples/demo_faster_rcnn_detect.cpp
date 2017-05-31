#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <memory>
#include <map>

#include "caffe/caffe.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#define INPUT_SIZE_NARROW  600
#define INPUT_SIZE_LONG  1000


using std::string;
using std::vector;
using std::max;
using std::min;

using namespace std;
using namespace caffe;


// RPN
namespace RPN{
    struct abox
    {
        float x1;
        float y1;
        float x2;
        float y2;
        float score;
        bool operator <(const abox&tmp) const{
            return score < tmp.score;
        }
    };
    void nms(std::vector<abox>& input_boxes,float nms_thresh);
    cv::Mat bbox_tranform_inv(cv::Mat, cv::Mat);
} // namespace RPN


// RPN
namespace RPN{
    cv::Mat bbox_tranform_inv(cv::Mat local_anchors, cv::Mat boxs_delta){
        cv::Mat pre_box(local_anchors.rows, local_anchors.cols, CV_32FC1);
        for (int i = 0; i < local_anchors.rows; i++)
        {
            double pred_ctr_x, pred_ctr_y, src_ctr_x, src_ctr_y;
            double dst_ctr_x, dst_ctr_y, dst_scl_x, dst_scl_y;
            double src_w, src_h, pred_w, pred_h;
            src_w = local_anchors.at<float>(i, 2) - local_anchors.at<float>(i, 0) + 1;
            src_h = local_anchors.at<float>(i, 3) - local_anchors.at<float>(i, 1) + 1;
            src_ctr_x = local_anchors.at<float>(i, 0) + 0.5 * src_w;
            src_ctr_y = local_anchors.at<float>(i, 1) + 0.5 * src_h;

            dst_ctr_x = boxs_delta.at<float>(i, 0);
            dst_ctr_y = boxs_delta.at<float>(i, 1);
            dst_scl_x = boxs_delta.at<float>(i, 2);
            dst_scl_y = boxs_delta.at<float>(i, 3);
            pred_ctr_x = dst_ctr_x*src_w + src_ctr_x;
            pred_ctr_y = dst_ctr_y*src_h + src_ctr_y;
            pred_w = exp(dst_scl_x) * src_w;
            pred_h = exp(dst_scl_y) * src_h;

            pre_box.at<float>(i, 0) = pred_ctr_x - 0.5*pred_w;
            pre_box.at<float>(i, 1) = pred_ctr_y - 0.5*pred_h;
            pre_box.at<float>(i, 2) = pred_ctr_x + 0.5*pred_w;
            pre_box.at<float>(i, 3) = pred_ctr_y + 0.5*pred_h;
        }
        return pre_box;
    }
    void nms(std::vector<abox> &input_boxes, float nms_thresh){
        std::vector<float>vArea(input_boxes.size());
        for (int i = 0; i < input_boxes.size(); ++i)
        {
            vArea[i] = (input_boxes.at(i).x2 - input_boxes.at(i).x1 + 1) * (input_boxes.at(i).y2
                    - input_boxes.at(i).y1 + 1);
        }
        for (int i = 0; i < input_boxes.size(); ++i)
        {
            for (int j = i + 1; j < input_boxes.size();)
            {
                float xx1 = std::max(input_boxes[i].x1, input_boxes[j].x1);
                float yy1 = std::max(input_boxes[i].y1, input_boxes[j].y1);
                float xx2 = std::min(input_boxes[i].x2, input_boxes[j].x2);
                float yy2 = std::min(input_boxes[i].y2, input_boxes[j].y2);
                float w = std::max(float(0), xx2 - xx1 + 1);
                float h = std::max(float(0), yy2 - yy1 + 1);
                float inter = w * h;
                float ovr = inter / (vArea[i] + vArea[j] - inter);
                if (ovr >= nms_thresh)
                {
                    input_boxes.erase(input_boxes.begin() + j);
                    vArea.erase(vArea.begin() + j);
                }
                else
                {
                    j++;
                }
            }
        }
    }
}


class ObjectDetector
{
    public:

        ObjectDetector(const std::string &model_file, const std::string &weights_file);
        map<int,vector<cv::Rect> > detect(const cv::Mat& image, map<int,vector<float> >* score=NULL);

    private:
        boost::shared_ptr< caffe::Net<float> > net_;
        int class_num_;
};

ObjectDetector::ObjectDetector(const std::string &model_file,const std::string &weights_file){
#ifdef CPU_ONLY
    Caffe::set_mode(Caffe::CPU);
#else
    Caffe::set_mode(Caffe::GPU);
#endif
    net_.reset(new Net<float>(model_file, TEST));
    net_->CopyTrainedLayersFrom(weights_file);
    this->class_num_ = net_->blob_by_name("cls_prob")->channels();  // number of class
    std::cout << "class_num_ : " << this->class_num_ << endl;
    /*
       vector<string> bn = net_->blob_names();
       for (int i = 0; i < bn.size(); i++) {
       std::cout << "Blob name : " << bn[i] << endl;
       }
       */
}

map<int,vector<cv::Rect> > ObjectDetector::detect(const cv::Mat& image,map<int,vector<float> >* objectScore){

    if(objectScore!=NULL)   //如果需要保存置信度
        objectScore->clear();

    float CONF_THRESH = 0.8;  // score threshold
    float NMS_THRESH = 0.3;   // nms threshold
    int max_side = max(image.rows, image.cols);
    int min_side = min(image.rows, image.cols);
    float max_side_scale = float(max_side) / float(INPUT_SIZE_LONG);    // scale size
    float min_side_scale = float(min_side) / float(INPUT_SIZE_NARROW);
    float max_scale = max(max_side_scale, min_side_scale);

    float img_scale = float(1) / max_scale;
    int height = int(image.rows * img_scale);
    int width = int(image.cols * img_scale);

    int num_out;
    cv::Mat cv_resized;
    image.convertTo(cv_resized, CV_32FC3);
    cv::resize(cv_resized, cv_resized, cv::Size(width, height));
    cv::Mat mean(height, width, cv_resized.type(), cv::Scalar(102.9801, 115.9465, 122.7717));
    cv::Mat normalized;
    subtract(cv_resized, mean, normalized);

    float im_info[3];
    im_info[0] = height;
    im_info[1] = width;
    im_info[2] = img_scale;
    shared_ptr<Blob<float> > input_layer = net_->blob_by_name("data");
    input_layer->Reshape(1, normalized.channels(), height, width);
    net_->Reshape();
    float* input_data = input_layer->mutable_cpu_data();
    vector<cv::Mat> input_channels;
    for (int i = 0; i < input_layer->channels(); ++i) {
        cv::Mat channel(height, width, CV_32FC1, input_data);
        input_channels.push_back(channel);
        input_data += height * width;
    }
    cv::split(normalized, input_channels);
    net_->blob_by_name("im_info")->set_cpu_data(im_info);
    net_->ForwardPrefilled();  // forward


    int num = net_->blob_by_name("rois")->num();  // numbers of ROI
    std::cout << "rois nums : " << num << endl;
    const float *rois_data = net_->blob_by_name("rois")->cpu_data();
    int num1 = net_->blob_by_name("bbox_pred")->num();
    std::cout << "bbox_pred nums: " << num1 << endl;
    cv::Mat rois_box(num, 4, CV_32FC1);
    for (int i = 0; i < num; ++i)
    {
        rois_box.at<float>(i, 0) = rois_data[i * 5 + 1] / img_scale;
        rois_box.at<float>(i, 1) = rois_data[i * 5 + 2] / img_scale;
        rois_box.at<float>(i, 2) = rois_data[i * 5 + 3] / img_scale;
        rois_box.at<float>(i, 3) = rois_data[i * 5 + 4] / img_scale;
    }

    shared_ptr<Blob<float> > bbox_delt_data = net_->blob_by_name("bbox_pred");   // 13949*84
    shared_ptr<Blob<float> > score = net_->blob_by_name("cls_prob");             // 3949*21

    map<int,vector<cv::Rect> > label_objs;
    for (int i = 1; i < class_num_; ++i){
        cv::Mat bbox_delt(num, 4, CV_32FC1);
        for (int j = 0; j < num; ++j){
            bbox_delt.at<float>(j, 0) = bbox_delt_data->data_at(j, i * 4 + 0, 0, 0);
            bbox_delt.at<float>(j, 1) = bbox_delt_data->data_at(j, i * 4 + 1, 0, 0);
            bbox_delt.at<float>(j, 2) = bbox_delt_data->data_at(j, i * 4 + 2, 0, 0);
            bbox_delt.at<float>(j, 3) = bbox_delt_data->data_at(j, i * 4 + 3, 0, 0);
        }
        cv::Mat box_class = RPN::bbox_tranform_inv(rois_box, bbox_delt);

        vector<RPN::abox> aboxes;   // the rectangle of class i
        for (int j = 0; j < box_class.rows; ++j){
            if (box_class.at<float>(j, 0) < 0)  box_class.at<float>(j, 0) = 0;
            if (box_class.at<float>(j, 0) > (image.cols - 1))   box_class.at<float>(j, 0) = image.cols - 1;
            if (box_class.at<float>(j, 2) < 0)  box_class.at<float>(j, 2) = 0;
            if (box_class.at<float>(j, 2) > (image.cols - 1))   box_class.at<float>(j, 2) = image.cols - 1;

            if (box_class.at<float>(j, 1) < 0)  box_class.at<float>(j, 1) = 0;
            if (box_class.at<float>(j, 1) > (image.rows - 1))   box_class.at<float>(j, 1) = image.rows - 1;
            if (box_class.at<float>(j, 3) < 0)  box_class.at<float>(j, 3) = 0;
            if (box_class.at<float>(j, 3) > (image.rows - 1))   box_class.at<float>(j, 3) = image.rows - 1;
            RPN::abox tmp;
            tmp.x1 = box_class.at<float>(j, 0);
            tmp.y1 = box_class.at<float>(j, 1);
            tmp.x2 = box_class.at<float>(j, 2);
            tmp.y2 = box_class.at<float>(j, 3);
            tmp.score = score->data_at(j, i, 0, 0);
            aboxes.push_back(tmp);
        }
        std::sort(aboxes.rbegin(), aboxes.rend());
        RPN::nms(aboxes, NMS_THRESH);  // NMS
        for (int k = 0; k < aboxes.size();){
            if (aboxes[k].score < CONF_THRESH)
                aboxes.erase(aboxes.begin() + k);
            else
                k++;
        }

        vector<cv::Rect> rect(aboxes.size());    // get rectangle of class i
        for(int ii=0;ii<aboxes.size();++ii) {
            rect[ii]=cv::Rect(cv::Point(aboxes[ii].x1,aboxes[ii].y1),cv::Point(aboxes[ii].x2,aboxes[ii].y2));
        }
        label_objs[i]=rect;

        if(objectScore!=NULL){
            vector<float> tmp(aboxes.size());       // the score of class i
            for(int ii=0;ii<aboxes.size();++ii)
                tmp[ii]=aboxes[ii].score;
            objectScore->insert(pair<int,vector<float> >(i,tmp));
        }
    }
    return label_objs;
}

string num2str(float i){
    stringstream ss;
    ss<<i;
    return ss.str();
}

int main(int argc,char **argv){
    if (4 != argc) {
        fprintf(stderr, "[Usage]: %s [*.prototxt] [*.caffemodel] [image file]\n", argv[0]);
        return -1;
    }
#ifdef CPU_ONLY
    cout<<"Use CPU\n";
#else
    cout<<"Use GPU\n";
#endif

    ObjectDetector detect(argv[1], argv[2]);

    cv::Mat img=cv::imread(argv[3]);
    map<int,vector<float> > score;
    map<int,vector<cv::Rect> > label_objs = detect.detect(img,&score);

    for(map<int,vector<cv::Rect> >::iterator it=label_objs.begin(); it != label_objs.end(); it++) {
        int label=it->first;  // label
        vector<cv::Rect> rects=it->second;  // detect box
        for(int j=0;j<rects.size();j++) {
            rectangle(img,rects[j],cv::Scalar(0,0,255),2);
            string txt=num2str(label)+" : "+num2str(score[label][j]);
            putText(img,txt,cv::Point(rects[j].x,rects[j].y),CV_FONT_HERSHEY_SIMPLEX,0.5,cv::Scalar(0,255,0));
        }
    }

    cv::imwrite("out.jpg", img);
    //imshow("", img);
    //waitKey();
    return 0;
}
