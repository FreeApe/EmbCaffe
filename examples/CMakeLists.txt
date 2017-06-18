#-------------------------------------
# CMake and project setting
#-------------------------------------
cmake_minimum_required (VERSION 3.0.2)
project (
    embcaffe_examples
    VERSION 0.1.1
    )

if(MSVC)
    option(CPU_ONLY  "Build Caffe without CUDA support" ON) # TODO: rename to USE_CUDA
    add_definitions(-DCPU_ONLY)
else()
    option(CPU_ONLY  "Build Caffe without CUDA support" OFF) # TODO: rename to USE_CUDA
endif()
#-------------------------------------
# Setting link directories for include files and library files
#-------------------------------------
if(MSVC)
    add_definitions(-DGLEW_STATIC)
    add_definitions(-DFREEGLUT_STATIC)
    add_definitions(-DGOOGLE_GLOG_DLL_DECL=)
    add_definitions(-DGLOG_NO_ABBREVIATED_SEVERITIES)
    remove_definitions(-D_HDF5USEDLL_)
    remove_definitions(-DH5_BUILT_AS_DYNAMIC_LIB)
endif()

if(MSVC)
	set (CAFFE_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/../embcaffe/cmake_build/install/include")
	set (BLAS_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/../../embcaffe_3rdparty/openblas/include")
	set (BOOST_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/../../embcaffe_3rdparty/boost/include")
	set (GFLAGS_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/../../embcaffe_3rdparty/gflags/include")
	set (GLOG_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/../../embcaffe_3rdparty/glog/include")
	set (HDF5_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/../../embcaffe_3rdparty/hdf5/include")
	set (PROTOBUF_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/../../embcaffe_3rdparty/protobuf/include")
	set (OPENCV_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/../../embcaffe_3rdparty/opencv/include")
	set (CAFFE_LIBRARY_DIR "${PROJECT_SOURCE_DIR}/../embcaffe/cmake_build/install/lib")
	set (BLAS_LIBRARY_DIR "${PROJECT_SOURCE_DIR}/../../embcaffe_3rdparty/openblas/lib")
	set (BOOST_LIBRARY_DIR "${PROJECT_SOURCE_DIR}/../../embcaffe_3rdparty/boost/lib")
	set (GFLAGS_LIBRARY_DIR "${PROJECT_SOURCE_DIR}/../../embcaffe_3rdparty/gflags/lib")
	set (GLOG_LIBRARY_DIR "${PROJECT_SOURCE_DIR}/../../embcaffe_3rdparty/glog/lib")
	set (HDF5_LIBRARY_DIR "${PROJECT_SOURCE_DIR}/../../embcaffe_3rdparty/hdf5/lib")
	set (PROTOBUF_LIBRARY_DIR "${PROJECT_SOURCE_DIR}/../../embcaffe_3rdparty/protobuf/lib")
	set (OPENCV_LIBRARY_DIR "${PROJECT_SOURCE_DIR}/../../embcaffe_3rdparty/opencv/lib")
else()
	set (CAFFE_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/../embcaffe/cmake_build/install/include")
	set (CAFFE_LIBRARY_DIR "${PROJECT_SOURCE_DIR}/../embcaffe/cmake_build/install/lib")
	set (BLAS_INCLUDE_DIR "/usr/include/atlas")
	set (CUDA_INCLUDE_DIR "/usr/local/cuda-7.5/include/")
	set (CUDA_LIBRARY_DIR "/usr/local/cuda-7.5/lib64/")
endif()

list (APPEND SAMPLES_INCLUDE_DIRS
		${CAFFE_INCLUDE_DIR}
		${BLAS_INCLUDE_DIR}
		${BOOST_INCLUDE_DIR}
		${GFLAGS_INCLUDE_DIR}
		${GLOG_INCLUDE_DIR}
		${HDF5_INCLUDE_DIR}
		${PROTOBUF_INCLUDE_DIR}
		${OPENCV_INCLUDE_DIR}
        ${CUDA_INCLUDE_DIR}
		)

list (APPEND SAMPLES_LINK_LIBRARIES
		${CAFFE_LIBRARY_DIR}
		${BLAS_LIBRARY_DIR}
		${BOOST_LIBRARY_DIR}
		${GFLAGS_LIBRARY_DIR}
		${GLOG_LIBRARY_DIR}
		${HDF5_LIBRARY_DIR}
		${PROTOBUF_LIBRARY_DIR}
		${OPENCV_LIBRARY_DIR}
        ${CUDA_LIBRARY_DIR}
		)

include_directories (
	${SAMPLES_INCLUDE_DIRS}
    )

link_directories (
    ${SAMPLES_LINK_LIBRARIES}
    )

#-------------------------------------
# Setting source list
#-------------------------------------
if (MSVC AND CPU_ONLY)
	aux_source_directory (${PROJECT_SOURCE_DIR}/../embcaffe/src/caffe/layers/ CAFFE_LAYERS)
	list (APPEND CAFFE_LAYERS ${PROJECT_SOURCE_DIR}/../embcaffe/src/caffe/layer_factory.cpp)
	list (APPEND CAFFE_LAYERS ${PROJECT_SOURCE_DIR}/../embcaffe/src/caffe/common.cpp)
	list (APPEND CAFFE_LAYERS ${PROJECT_SOURCE_DIR}/../embcaffe/src/caffe/data_reader.cpp)
	list (APPEND CAFFE_LAYERS ${PROJECT_SOURCE_DIR}/../embcaffe/src/caffe/data_transformer.cpp)
	list (APPEND CAFFE_LAYERS ${PROJECT_SOURCE_DIR}/../embcaffe/src/caffe/blob.cpp)
endif()

aux_source_directory (. PROJECT_EXAMPLES_SOURCE_LIST)
string (REPLACE "./" "" PROJECT_EXAMPLES_SOURCE_LIST "${PROJECT_EXAMPLES_SOURCE_LIST}")
string (REPLACE ".cpp" "" PROJECT_EXAMPLES_SOURCE_LIST "${PROJECT_EXAMPLES_SOURCE_LIST}")
message (STATUS "examples source lists: ${PROJECT_EXAMPLES_SOURCE_LIST}")

#-------------------------------------
# Output
#-------------------------------------
foreach (EXAMPLE_SOURCE_FILE_NAME ${PROJECT_EXAMPLES_SOURCE_LIST})
    add_executable (${EXAMPLE_SOURCE_FILE_NAME} ${EXAMPLE_SOURCE_FILE_NAME}.cpp ${CAFFE_LAYERS})
    #add_dependencies (${EXAMPLE_SOURCE_FILE_NAME} ${LOWER_PROJECT_NAME})
    set_target_properties (${EXAMPLE_SOURCE_FILE_NAME} PROPERTIES INSTALL_RPATH "")
	if (MSVC)
		target_link_libraries (${EXAMPLE_SOURCE_FILE_NAME}
			"libboost_filesystem-vc140-mt-1_64.lib"
			"libboost_system-vc140-mt-1_64.lib"
			"libboost_thread-vc140-mt-1_64.lib"
			"gflags_static.lib"
			"glog.lib"
			"hdf5.lib"
			"hdf5_hl.lib"
			"libopenblas.lib"
			"protobuf.lib"
			"protoc.lib"
			"opencv_core320.lib"
			"opencv_highgui320.lib"
			"opencv_imgcodecs320.lib"
			"opencv_imgproc320.lib"
			"opencv_videoio320.lib"
			"caffe.lib"
			"proto.lib"
			"libgflags.lib"
			"gflags_nothreads_static.lib"
			"shlwapi.lib"
			)
	else()
		target_link_libraries (${EXAMPLE_SOURCE_FILE_NAME}
			"libopencv_highgui.so"
			"libopencv_core.so"
			"libopencv_imgproc.so"
			"libglog.so"
			"libcaffe.so"
			"libboost_system.so"
			"libboost_regex.so"
			"libgflags.so"
			)
	endif()
endforeach()



