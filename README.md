# Embedded Caffe

&emsp;&emsp;Make the depth learning model run on Embedded Systems.

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
```

# Build

## Build EmbCaffe


- Way 1

```bash
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

## Build demo

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


