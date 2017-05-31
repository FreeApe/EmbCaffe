#-------------------------------------
# CMake and project setting
#-------------------------------------
cmake_minimum_required (VERSION 3.0.2)
project (
    embcaffe_examples
    VERSION 0.1.1
    )

#-------------------------------------
# Setting link directories for include files and library files
#-------------------------------------
set (BLAS_INCLUDE_DIR "/usr/include/atlas")
set (CUDA_INCLUDE_DIR "/usr/local/cuda-7.5/include/")
set (CUDA_LIBRARY_DIR "/usr/local/cuda-7.5/lib64/")

include_directories (
    ${PROJECT_SOURCE_DIR}/../embcaffe/cmake_build/install/include
    ${BLAS_INCLUDE_DIR}
    ${CUDA_INCLUDE_DIR}
    )

link_directories (
    ${PROJECT_BINARY_DIR}
    ${PROJECT_SOURCE_DIR}/../embcaffe/cmake_build/install/lib
    ${CUDA_LIBRARY_DIR}
    )

#-------------------------------------
# Setting source list
#-------------------------------------
aux_source_directory (. PROJECT_EXAMPLES_SOURCE_LIST)
string (REPLACE "./" "" PROJECT_EXAMPLES_SOURCE_LIST "${PROJECT_EXAMPLES_SOURCE_LIST}")
string (REPLACE ".cpp" "" PROJECT_EXAMPLES_SOURCE_LIST "${PROJECT_EXAMPLES_SOURCE_LIST}")
message (STATUS "examples source lists: ${PROJECT_EXAMPLES_SOURCE_LIST}")

#-------------------------------------
# Output
#-------------------------------------
foreach (EXAMPLE_SOURCE_FILE_NAME ${PROJECT_EXAMPLES_SOURCE_LIST})
    add_executable (${EXAMPLE_SOURCE_FILE_NAME} ${EXAMPLE_SOURCE_FILE_NAME}.cpp)
    #add_dependencies (${EXAMPLE_SOURCE_FILE_NAME} ${LOWER_PROJECT_NAME})
    set_target_properties (${EXAMPLE_SOURCE_FILE_NAME} PROPERTIES INSTALL_RPATH "")
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
endforeach()



