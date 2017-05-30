g++ demo_ssd_detect.cpp -I./include -I/usr/include/atlas -I/usr/local/cuda-7.5/include/ -L./lib -lcaffe -L/usr/local/cuda-7.5/lib64/ -lboost_system -lboost_regex -lcublas -lcudart -lcurand -lglog -lopencv_core -lopencv_imgproc -lopencv_highgui -lgflags -Wl,--no-as-needed

