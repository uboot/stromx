set (STROMX_CVIMGPROC_VERSION_MAJOR 0)
set (STROMX_CVIMGPROC_VERSION_MINOR 0)
set (STROMX_CVIMGPROC_VERSION_PATCH 1)

configure_file (
    ${CMAKE_CURRENT_SOURCE_DIR}/Config.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/Config.h
)

include_directories (
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
)

set (SOURCES 
    Cvimgproc.cpp
    Utility.cpp
    BilateralFilter.cpp
    Blur.cpp
    BoxFilter.cpp
    Dilate.cpp
    Erode.cpp
    GaussianBlur.cpp
    MedianBlur.cpp
    MorphologyEx.cpp
    Laplacian.cpp
    PyrDown.cpp
    PyrUp.cpp
    Scharr.cpp
    Sobel.cpp
    Resize.cpp
    AdaptiveThreshold.cpp
    Threshold.cpp
    WarpAffine.cpp
    WarpPerspective.cpp
    Undistort.cpp
    UndistortPoints.cpp
    DistanceTransform.cpp
    FloodFill.cpp
    Integral.cpp
    CalcHist1D.cpp
    FindContours.cpp
    DrawContours.cpp
    ApproxPolyDP.cpp
    BoundingRect.cpp
    ConvexHull.cpp
    FitEllipse.cpp
    FitLine.cpp
    MinAreaRect.cpp
    MinEnclosingCircle.cpp
    Canny.cpp
    CornerHarris.cpp
    CornerMinEigenVal.cpp
    CornerSubPix.cpp
    GoodFeaturesToTrack.cpp
    HoughLinesP.cpp
    PreCornerDetect.cpp
)

add_library (stromx_cvimgproc SHARED ${SOURCES})

set(VERSION_STRING "${STROMX_CVIMGPROC_VERSION_MAJOR}.${STROMX_CVIMGPROC_VERSION_MINOR}.${STROMX_CVIMGPROC_VERSION_PATCH}")

set_target_properties (stromx_cvimgproc PROPERTIES
    VERSION ${VERSION_STRING}
    SOVERSION ${VERSION_STRING}
    FOLDER "library"
)

target_link_libraries (stromx_cvimgproc
    ${OpenCV_LIBS}
    stromx_runtime
    stromx_cvsupport
)

if(WIN32)
    install (TARGETS stromx_cvimgproc
        RUNTIME DESTINATION .
        LIBRARY DESTINATION ${LIB_DIR}
        ARCHIVE DESTINATION ${LIB_DIR}
    )
else(WIN32)
    install (TARGETS stromx_cvimgproc
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION ${LIB_DIR}
        ARCHIVE DESTINATION ${LIB_DIR}
    )
endif(WIN32)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/stromx/cvimgproc
    DESTINATION include/stromx
    FILES_MATCHING PATTERN "*.h"
    PATTERN "test*" EXCLUDE
)

install(DIRECTORY ${CMAKE_BINARY_DIR}/stromx/cvimgproc
    DESTINATION include/stromx
    FILES_MATCHING PATTERN "*.h"
)

if(BUILD_TESTS)
    add_subdirectory(test)
endif(BUILD_TESTS)
