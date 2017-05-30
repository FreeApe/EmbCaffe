# Embedded Caffe

&emsp;&emsp;Make the depth learning model run on Embedded Systems.

# Target and Features

- Based on [caffe](https://github.com/BVLC/caffe)
- Compact，Only forward calculation
- Including the mainstream algorithm, like faster rcnn, ssd, and so on.
- Support GPU/CPU model
- Cross-platform

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

# TODO

- Remove Backward calculation
- Support ssd
- Support windows system
- Optimize the calculation
- ......
