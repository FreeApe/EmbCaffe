# Embedded Caffe

&emsp;&emsp;Make the depth learning model running on Embedded Systems and multi-platform.

# Features

- Based on [caffe](https://github.com/BVLC/caffe)
- Compact，Only forward calculation
- Support faster rcnn
- Support ssd
- Support ShuffleNet
- Support MobileNet
- Support GPU/CPU model(Windows only support CPU model now)
- Cross-platform
- Small
- Remove the gtest
- Remove the test
- Remove the python wrapper

# Requirements

## Linux

```
System              : Linux / Ubuntu 14.04
Cmake               : 3.4+
C++ compiler        : 4.8+
BLAS                : Atlas
Boost               : 1.54
glog                : Y
gflags              : Y
protobuf            : 2.6.1
CUDA                : 7.5
cuDNN               : 5.0.5
OpenCV              : N (for examples)
```
## Windows

- CPU_ONLY mode support x86/x86_64 archtecture.
- GPU model only support x86_64 archtecture, because NVIDIA only support the x86_64 archtecture libraries since CUDA7.0.

```
System              : Windows
Cmake-gui           : 3.8+
C++ compiler        : vc140(VS2015)
embcaffe_3rdparty   : https://github.com/FreeApe/embcaffe_3rdparty
```


# Build

## Build EmbCaffe on Linux


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

## Build EmbCaffe on Windows

```
1. with CMake-gui3.8+
2. git clone https://github.com/FreeApe/embcaffe_3rdparty(You can also compile these third-party libraries yourself)
2. Configure and Generate(You should configure CMAKE_INCLUDE_PATH and CMAKE_LIBRARY_PATH)
3. Open Caffe.sln with VS2015
4. build solutions
```


## Build examples

&emsp;&emsp;**Linux:**

```
$ cd examples
$ mkdir build
$ cd build
$ cmake ..
$ make -j
``` 

&emsp;&emsp;**Windows:**

```
1. with CMake-gui3.8+
2. Configure and Generate
3. Open Caffe-example.sln with VS2015
4. build solutions
5. run demos
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
- Support GPU model on windows
- Optimize the calculation
- ......

# Thanks

- https://github.com/BVLC/caffe
- https://github.com/intel/caffe
- http://blog.csdn.net/zxj942405301/article/details/72775463
