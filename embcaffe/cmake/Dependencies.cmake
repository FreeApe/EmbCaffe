# This list is required for static linking and exported to CaffeConfig.cmake
set(Caffe_LINKER_LIBS "")

# ---[ Boost
set(BOOST_ROOT_DIR "" CACHE PATH "Only set it when BOOST_ROOT environment varibale isn't defined")

if(NOT DEFINED ENV{BOOST_ROOT})
    message(STATUS "Not define the BOOST_ROOT environment varibal")
    set(BOOST_ROOT "${BOOST_ROOT_DIR}")
else()
    message(STATUS "BOOST_ROOT environment varibale: $ENV{BOOST_ROOT}")
	set(BOOST_ROOT "$ENV{BOOST_ROOT}")
endif()
message(STATUS "Boost_ROOT ${Boost_ROOT}")
if(NOT MSVC)
	find_package(Boost 1.46 REQUIRED COMPONENTS system thread filesystem)
else()
	find_package(Boost 1.64.0)
endif()

if(Boost_FOUND)
	mark_as_advanced(BOOST_ROOT_DIR BOOST_ROOT)
    message(STATUS "Boost_INCLUDE_DIRS : ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARY_DIRS : ${Boost_LIBRARY_DIRS}")	    
	if(MSVC)
		#unset(Boost_LIBRARIES CACHE)
		set(Boost_LIBRARIES
			"${Boost_LIBRARY_DIRS}/libboost_system-vc140-mt-1_64.lib"
			"${Boost_LIBRARY_DIRS}/libboost_thread-vc140-mt-1_64.lib"
			"${Boost_LIBRARY_DIRS}/libboost_filesystem-vc140-mt-1_64.lib"	
			)
	endif()
	message(STATUS "Boost_LIBRARIES : ${Boost_LIBRARIES}")
    include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
	link_directories(SYSTEM ${Boost_LIBRARY_DIRS})
    list(APPEND Caffe_LINKER_LIBS ${Boost_LIBRARIES})
else()
	message(WARNING "Boost not found")
endif(Boost_FOUND)

# ---[ Threads
find_package(Threads REQUIRED)
list(APPEND Caffe_LINKER_LIBS ${CMAKE_THREAD_LIBS_INIT})

# ---[ Google-glog
include("cmake/External/glog.cmake")
include_directories(SYSTEM ${GLOG_INCLUDE_DIRS})
list(APPEND Caffe_LINKER_LIBS ${GLOG_LIBRARIES})

# ---[ Google-gflags
include("cmake/External/gflags.cmake")
include_directories(SYSTEM ${GFLAGS_INCLUDE_DIRS})
list(APPEND Caffe_LINKER_LIBS ${GFLAGS_LIBRARIES})

# ---[ Google-protobuf
include(cmake/ProtoBuf.cmake)

# ---[ HDF5
if(NOT DEFINED ENV{HDF5_ROOT})
	set(HDF5_ROOT CACHE PATH "")
    message(STATUS "Not define the HDF5_ROOT environment varibal")
    set(HDF5_DIR "${HDF5_ROOT}")
else()
    message(STATUS "HDF5_ROOT environment varibale: $ENV{BOOST_ROOT}")
	set(HDF5_DIR "ENV{HDF5_ROOT}")
endif()

#find_package(HDF5 COMPONENTS HL REQUIRED)
find_package(HDF5 COMPONENTS HL REQUIRED)
if(HDF5_FOUND)
  mark_as_advanced(HDF5_DIR)
  include_directories(SYSTEM ${HDF5_INCLUDE_DIRS} ${HDF5_HL_INCLUDE_DIR})
  if(MSVC)
	set(HDF5_LIBRARIES ${HDF5_LIBRARIES} ${HDF5_ROOT}/lib/hdf5_hl.lib)
  endif()
  message(STATUS "HDF5_LIBRARIES: ${HDF5_LIBRARIES}")
  list(APPEND Caffe_LINKER_LIBS ${HDF5_LIBRARIES})
else()
  message(WARNING "HDF5 not found")
endif()

# ---[ LMDB
if(USE_LMDB)
  find_package(LMDB REQUIRED)
  include_directories(SYSTEM ${LMDB_INCLUDE_DIR})
  list(APPEND Caffe_LINKER_LIBS ${LMDB_LIBRARIES})
  add_definitions(-DUSE_LMDB)
  if(ALLOW_LMDB_NOLOCK)
    add_definitions(-DALLOW_LMDB_NOLOCK)
  endif()
endif()

# ---[ LevelDB
if(USE_LEVELDB)
  find_package(LevelDB REQUIRED)
  include_directories(SYSTEM ${LevelDB_INCLUDE})
  list(APPEND Caffe_LINKER_LIBS ${LevelDB_LIBRARIES})
  add_definitions(-DUSE_LEVELDB)
endif()

# ---[ Snappy
if(USE_LEVELDB)
  find_package(Snappy REQUIRED)
  include_directories(SYSTEM ${Snappy_INCLUDE_DIR})
  list(APPEND Caffe_LINKER_LIBS ${Snappy_LIBRARIES})
endif()

# ---[ CUDA
include(cmake/Cuda.cmake)
if(NOT HAVE_CUDA)
  if(CPU_ONLY)
    message(STATUS "-- CUDA is disabled. Building without it...")
  else()
    message(WARNING "-- CUDA is not detected by cmake. Building without it...")
  endif()

  # TODO: remove this not cross platform define in future. Use caffe_config.h instead.
  add_definitions(-DCPU_ONLY)
endif()

# ---[ OpenCV
if(USE_OPENCV)
  find_package(OpenCV QUIET COMPONENTS core highgui imgproc imgcodecs)
  if(NOT OpenCV_FOUND) # if not OpenCV 3.x, then imgcodecs are not found
    find_package(OpenCV REQUIRED COMPONENTS core highgui imgproc)
  endif()
  include_directories(SYSTEM ${OpenCV_INCLUDE_DIRS})
  list(APPEND Caffe_LINKER_LIBS ${OpenCV_LIBS})
  message(STATUS "OpenCV found (${OpenCV_CONFIG_PATH})")
  add_definitions(-DUSE_OPENCV)
endif()

# ---[ BLAS
if(NOT APPLE)  
  if(MSVC)
	set(BLAS "Open" CACHE STRING "Selected BLAS library")	
  else()
	set(BLAS "Atlas" CACHE STRING "Selected BLAS library")	
  endif()
  set_property(CACHE BLAS PROPERTY STRINGS "Atlas;Open;MKL")
  
  message(STATUS "BLAS is : ${BLAS}")
  if(BLAS STREQUAL "Atlas" OR BLAS STREQUAL "atlas")
    find_package(Atlas REQUIRED)
    include_directories(SYSTEM ${Atlas_INCLUDE_DIR})
    list(APPEND Caffe_LINKER_LIBS ${Atlas_LIBRARIES})
  elseif(BLAS STREQUAL "Open" OR BLAS STREQUAL "open")
    find_package(OpenBLAS REQUIRED)
    include_directories(SYSTEM ${OpenBLAS_INCLUDE_DIR})
    list(APPEND Caffe_LINKER_LIBS ${OpenBLAS_LIB})
  elseif(BLAS STREQUAL "MKL" OR BLAS STREQUAL "mkl")
    find_package(MKL REQUIRED)
    include_directories(SYSTEM ${MKL_INCLUDE_DIR})
    list(APPEND Caffe_LINKER_LIBS ${MKL_LIBRARIES})
    add_definitions(-DUSE_MKL)
  endif()
elseif(APPLE)
  find_package(vecLib REQUIRED)
  include_directories(SYSTEM ${vecLib_INCLUDE_DIR})
  list(APPEND Caffe_LINKER_LIBS ${vecLib_LINKER_LIBS})
endif()


