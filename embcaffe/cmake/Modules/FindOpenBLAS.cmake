IF(UNIX)
    SET(Open_BLAS_INCLUDE_SEARCH_PATHS
      /usr/include
      /usr/include/openblas
      /usr/include/openblas-base
      /usr/local/include
      /usr/local/include/openblas
      /usr/local/include/openblas-base
      /opt/OpenBLAS/include
      $ENV{OpenBLAS_HOME}
      $ENV{OpenBLAS_HOME}/include
    )

    SET(Open_BLAS_LIB_SEARCH_PATHS
            /lib/
            /lib/openblas-base
            /lib64/
            /usr/lib
            /usr/lib/openblas-base
            /usr/lib64
            /usr/local/lib
            /usr/local/lib64
            /opt/OpenBLAS/lib
            $ENV{OpenBLAS}cd
            $ENV{OpenBLAS}/lib
            $ENV{OpenBLAS_HOME}
            $ENV{OpenBLAS_HOME}/lib
     )
ELSEIF(MSVC)
    IF(NOT DEFINED $ENV{Open_BLAS_ROOT_DIR})
        SET(Open_BLAS_ROOT_DIR CACHE PATH "Folder contains OpenBLAS")
    ENDIF()
    SET(Open_BLAS_INCLUDE_SEARCH_PATHS
        ${Open_BLAS_ROOT_DIR}/include
        ${Open_BLAS_ROOT_DIR}/include/openblas
        ${Open_BLAS_ROOT_DIR}/include/openblas-base
        $ENV{OpenBLAS_HOME}
        $ENV{OpenBLAS_HOME}/include
    )

    SET(Open_BLAS_LIB_SEARCH_PATHS
        ${Open_BLAS_ROOT_DIR}/lib/
        $ENV{OpenBLAS_HOME}
        $ENV{OpenBLAS_HOME}/lib
     )
ELSE()
    message(FATAL_ERROR "Do not support this platform\n")
ENDIF()

FIND_PATH(OpenBLAS_INCLUDE_DIR NAMES cblas.h PATHS ${Open_BLAS_INCLUDE_SEARCH_PATHS})
IF(UNIX)
    FIND_LIBRARY(OpenBLAS_LIB NAMES openblas PATHS ${Open_BLAS_LIB_SEARCH_PATHS})
ELSEIF(MSVC)
    FIND_LIBRARY(OpenBLAS_LIB NAMES libopenblas.lib libopenblas.dll PATHS ${Open_BLAS_LIB_SEARCH_PATHS})
ELSE()
    message(FATAL_ERROR "Do not support this platform\n")
ENDIF()

SET(OpenBLAS_FOUND ON)

#    Check include files
IF(NOT OpenBLAS_INCLUDE_DIR)
    SET(OpenBLAS_FOUND OFF)
    MESSAGE(STATUS "Could not find OpenBLAS include. Turning OpenBLAS_FOUND off")
ENDIF()

#    Check libraries
IF(NOT OpenBLAS_LIB)
    SET(OpenBLAS_FOUND OFF)
    MESSAGE(STATUS "Could not find OpenBLAS lib. Turning OpenBLAS_FOUND off")
ENDIF()

IF (OpenBLAS_FOUND)
  IF (NOT OpenBLAS_FIND_QUIETLY)
    MESSAGE(STATUS "Found OpenBLAS libraries: ${OpenBLAS_LIB}")
    MESSAGE(STATUS "Found OpenBLAS include: ${OpenBLAS_INCLUDE_DIR}")
  ENDIF (NOT OpenBLAS_FIND_QUIETLY)
ELSE (OpenBLAS_FOUND)
  IF (OpenBLAS_FIND_REQUIRED)
    MESSAGE(FATAL_ERROR "Could not find OpenBLAS")
  ENDIF (OpenBLAS_FIND_REQUIRED)
ENDIF (OpenBLAS_FOUND)

MARK_AS_ADVANCED(
	Open_BLAS_ROOT_DIR	
    OpenBLAS_INCLUDE_DIR
    OpenBLAS_LIB
    OpenBLAS
)

