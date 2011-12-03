# To find OpenCV 2 library visit http://opencv.willowgarage.com/wiki/
#
# The follwoing variables are optionally searched for defaults
#  OpenCV2_ROOT_DIR:                   Base directory of OpenCV 2 tree to use.
#
# The following are set after configuration is done: 
#  OpenCV2_FOUND
#  OpenCV2_INCLUDE_DIRS
#  OpenCV2_LIBRARIES
#
# $Id: $
#  
# Balazs [2011-01-18]:
# - Created from scratch for the reorganized OpenCV 2 structure introduced at version 2.2
#
# This file should be removed when CMake will provide an equivalent


# typical root dirs of installations, exactly one of them is used
IF(WIN32)
    SET(OpenCV2_POSSIBLE_ROOT_DIRS
        "$ENV{OpenCV_ROOT_DIR}"
        "$ENV{SystemDrive}/OpenCV2.3"                     # Windows: OpenCV 2.3 default installation dir (expected future revision)
        "$ENV{SystemDrive}/OpenCV2.2"                     # Windows: OpenCV 2.2 default installation dir
        "$ENV{SystemDrive}/Program Files/OpenCV2.3"       # 32 bit ProgramFiles dir on Win32;  64 bit ProgramFiles dir on Win64 (expected future revision)
        "$ENV{SystemDrive}/Program Files/OpenCV2.2"       # 32 bit ProgramFiles dir on Win32;  64 bit ProgramFiles dir on Win64
        "$ENV{SystemDrive}/Program Files (x86)/OpenCV2.3" # 32 bit ProgramFiles dir on Win64 (expected future revision)
        "$ENV{SystemDrive}/Program Files (x86)/OpenCV2.2" # 32 bit ProgramFiles dir on Win64
        )
ELSE(WIN32)
    SET(OpenCV2_POSSIBLE_ROOT_DIRS
        /usr/local                                      # Linux: default dir by CMake
        /usr                                            # Linux
        /opt/local                                      # OS X: default MacPorts location
        )
ENDIF(WIN32)


# select exactly ONE OpenCV 2 base directory
# to avoid mixing different version headers and libs
FIND_PATH(OpenCV2_ROOT_DIR
          NAMES include/opencv2/opencv.hpp
          PATHS ${OpenCV2_POSSIBLE_ROOT_DIRS}
          )

# absolute path to all libraries 
IF(WIN32)
    SET(OPENCV2_LIBRARY_SEARCH_PATHS "${OpenCV2_ROOT_DIR}/lib")
ELSE(WIN32)
    SET(OPENCV2_LIBRARY_SEARCH_PATHS "${OpenCV2_ROOT_DIR}/lib")
ENDIF(WIN32)

IF(WIN32)
    SET(OPENCV2_LIBRARY_SEARCH_PATHS
        ${OPENCV2_LIBRARY_SEARCH_PATHS}
        "${OpenCV2_ROOT_DIR}/lib/Release"
        "${OpenCV2_ROOT_DIR}/lib/Debug"
        )
    FIND_LIBRARY(OpenCV2_CORE_LIBRARY
                 NAMES opencv_core231 opencv_core230 opencv_core220
                 PATHS ${OPENCV2_LIBRARY_SEARCH_PATHS})
    FIND_LIBRARY(OpenCV2_IMGPROC_LIBRARY
                 NAMES opencv_imgproc231 opencv_imgproc230 opencv_imgproc220
                 PATHS ${OPENCV2_LIBRARY_SEARCH_PATHS})
    FIND_LIBRARY(OpenCV2_FEATURES2D_LIBRARY
                 NAMES opencv_features2d231 opencv_features2d230 opencv_features2d220
                 PATHS ${OPENCV2_LIBRARY_SEARCH_PATHS})
    FIND_LIBRARY(OpenCV2_FLANN_LIBRARY
                 NAMES opencv_flann231 opencv_flann230 opencv_flann220
                 PATHS ${OPENCV2_LIBRARY_SEARCH_PATHS})
    FIND_LIBRARY(OpenCV2_CALIB3D_LIBRARY
                 NAMES opencv_calib3d231 opencv_calib3d230 opencv_calib3d220
                 PATHS ${OPENCV2_LIBRARY_SEARCH_PATHS})
    FIND_LIBRARY(OpenCV2_OBJDETECT_LIBRARY
                 NAMES opencv_objdetect231 opencv_objdetect230 opencv_objdetect220
                 PATHS ${OPENCV2_LIBRARY_SEARCH_PATHS})
    FIND_LIBRARY(OpenCV2_LEGACY_LIBRARY
                 NAMES opencv_legacy231 opencv_legacy230 opencv_legacy220
                 PATHS ${OPENCV2_LIBRARY_SEARCH_PATHS})
    FIND_LIBRARY(OpenCV2_CONTRIB_LIBRARY
                 NAMES opencv_contrib231 opencv_contrib230 opencv_contrib220
                 PATHS ${OPENCV2_LIBRARY_SEARCH_PATHS})
    FIND_LIBRARY(OpenCV2_HIGHGUI_LIBRARY
                 NAMES opencv_highgui231 opencv_highgui230 opencv_highgui220
                 PATHS ${OPENCV2_LIBRARY_SEARCH_PATHS})
    FIND_LIBRARY(OpenCV2_ML_LIBRARY
                 NAMES opencv_ml231 opencv_ml230 opencv_ml220
                 PATHS ${OPENCV2_LIBRARY_SEARCH_PATHS})
    FIND_LIBRARY(OpenCV2_VIDEO_LIBRARY
                 NAMES opencv_video231 opencv_video230 opencv_video220
                 PATHS ${OPENCV2_LIBRARY_SEARCH_PATHS})
    FIND_LIBRARY(OpenCV2_GPU_LIBRARY
                 NAMES opencv_gpu231 opencv_gpu230 opencv_gpu220
                 PATHS ${OPENCV2_LIBRARY_SEARCH_PATHS})
    FIND_LIBRARY(OpenCV2_FFMPEG_LIBRARY
                 NAMES opencv_ffmpeg231 opencv_ffmpeg230 opencv_ffmpeg220
                 PATHS ${OPENCV2_LIBRARY_SEARCH_PATHS})
    FIND_LIBRARY(OpenCV2_TS_LIBRARY
                 NAMES opencv_ts231 opencv_ts230 opencv_ts220
                 PATHS ${OPENCV2_LIBRARY_SEARCH_PATHS})
ELSE(WIN32)
    FIND_LIBRARY(OpenCV2_CORE_LIBRARY       NAMES opencv_core       PATHS ${OPENCV2_LIBRARY_SEARCH_PATHS})
    FIND_LIBRARY(OpenCV2_IMGPROC_LIBRARY    NAMES opencv_imgproc    PATHS ${OPENCV2_LIBRARY_SEARCH_PATHS})
    FIND_LIBRARY(OpenCV2_FEATURES2D_LIBRARY NAMES opencv_features2d PATHS ${OPENCV2_LIBRARY_SEARCH_PATHS})
    FIND_LIBRARY(OpenCV2_FLANN_LIBRARY      NAMES opencv_flann      PATHS ${OPENCV2_LIBRARY_SEARCH_PATHS})
    FIND_LIBRARY(OpenCV2_CALIB3D_LIBRARY    NAMES opencv_calib3d    PATHS ${OPENCV2_LIBRARY_SEARCH_PATHS})
    FIND_LIBRARY(OpenCV2_OBJDETECT_LIBRARY  NAMES opencv_objdetect  PATHS ${OPENCV2_LIBRARY_SEARCH_PATHS})
    FIND_LIBRARY(OpenCV2_LEGACY_LIBRARY     NAMES opencv_legacy     PATHS ${OPENCV2_LIBRARY_SEARCH_PATHS})
    FIND_LIBRARY(OpenCV2_CONTRIB_LIBRARY    NAMES opencv_contrib    PATHS ${OPENCV2_LIBRARY_SEARCH_PATHS})
    FIND_LIBRARY(OpenCV2_HIGHGUI_LIBRARY    NAMES opencv_highgui    PATHS ${OPENCV2_LIBRARY_SEARCH_PATHS})
    FIND_LIBRARY(OpenCV2_ML_LIBRARY         NAMES opencv_ml         PATHS ${OPENCV2_LIBRARY_SEARCH_PATHS})
    FIND_LIBRARY(OpenCV2_VIDEO_LIBRARY      NAMES opencv_video      PATHS ${OPENCV2_LIBRARY_SEARCH_PATHS})
    FIND_LIBRARY(OpenCV2_GPU_LIBRARY        NAMES opencv_gpu        PATHS ${OPENCV2_LIBRARY_SEARCH_PATHS})
ENDIF(WIN32)

SET(OpenCV2_INCLUDE_DIR
    ${OpenCV2_ROOT_DIR}/include CACHE PATH "Path to OpenCV2 include dir"
    )

SET(OpenCV2_LIBRARIES
    ${OpenCV2_CORE_LIBRARY}
    ${OpenCV2_IMGPROC_LIBRARY}
    ${OpenCV2_FEATURES2D_LIBRARY}
    ${OpenCV2_FLANN_LIBRARY}
    ${OpenCV2_CALIB3D_LIBRARY}
    ${OpenCV2_OBJDETECT_LIBRARY}
    ${OpenCV2_LEGACY_LIBRARY}
    ${OpenCV2_CONTRIB_LIBRARY}
    ${OpenCV2_HIGHGUI_LIBRARY}
    ${OpenCV2_ML_LIBRARY}
    ${OpenCV2_VIDEO_LIBRARY}
    ${OpenCV2_GPU_LIBRARY}
    )

SET(OpenCV2_FOUND ON)
IF(NOT EXISTS ${OpenCV2_INCLUDE_DIR}/opencv2)
    SET(OpenCV2_FOUND OFF)
ENDIF(NOT EXISTS ${OpenCV2_INCLUDE_DIR}/opencv2)
FOREACH(NAME ${OpenCV2_LIBRARIES})
    IF(NOT EXISTS ${NAME})
        SET(OpenCV2_FOUND OFF)
    ENDIF(NOT EXISTS ${NAME})
ENDFOREACH(NAME)

MARK_AS_ADVANCED(FORCE
                 OpenCV2_ROOT_DIR
                 OpenCV2_INCLUDE_DIR
                 )

# display help message
IF(NOT OpenCV2_FOUND)
    # make FIND_PACKAGE friendly
    IF(OpenCV2_FIND_REQUIRED)
        MESSAGE(FATAL_ERROR "OpenCV 2 not found. Please specify it's location with the OpenCV2_ROOT_DIR env. variable.")
    ELSE(OpenCV2_FIND_REQUIRED)
        MESSAGE(STATUS "OpenCV 2 not found.")
    ENDIF(OpenCV2_FIND_REQUIRED)
ENDIF(NOT OpenCV2_FOUND)

