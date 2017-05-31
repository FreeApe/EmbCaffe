# Embedded Caffe

&emsp;&emsp;Make the depth learning model running on Embedded Systems and multi-platform.

# Features

- Based on [caffe](https://github.com/BVLC/caffe)
- Compact，Only forward calculation
- Support faster rcnn
- Support ssd
- Support GPU/CPU model
- Cross-platform

# Requirements

```
System          : Linux / Ubuntu 14.04
C++ compiler    : 4.8+
BLAS            : Atlas
Boost           : 1.54
glog            : Y
gflags          : Y
protobuf        : 2.6.1
CUDA            : 7.5
cuDNN           : 5.0.5
OpenCV          : N (for examples)
```

# Build

## Build EmbCaffe


- Way 1

```bash
# Note: Test examples with this way
$ cd EmbCaffe
$ mkdir cmake_build
$ cd cmake_build
$ cmake ..
$ make all -j

```

- Way 2


```
$ cd EmbCaffe
$ make all -j

```

## Build examples

```
$ cd examples
$ mkdir build
$ cd build
$ cmake ..
$ make -j
```

&emsp;&emsp;Running the examples, the results show as : 

![ssd](http://img.blog.csdn.net/20170531232140806?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRnJlZUFwZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![faster-rcnn](http://img.blog.csdn.net/20170531232203775?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRnJlZUFwZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


## Build Errors

```
1. error ‘type name’ declared as function returning an array escape

    Make sure the CUDA version is 7.5
```

# TODO / Targets

- Remove Backward calculation
- Support windows system
- Optimize the calculation
- ......

# Thanks

- https://github.com/BVLC/caffe
- https://github.com/intel/caffe
- http://blog.csdn.net/zxj942405301/article/details/72775463

