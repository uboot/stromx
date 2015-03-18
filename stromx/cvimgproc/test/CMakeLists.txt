add_test(NAME stromx_cvimgproc_test COMMAND stromx_cvimgproc_test)

if(MSVC)
    add_definitions(/DSTROMX_CVIMGPROC_STATIC)
endif(MSVC)

include_directories (
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${CPPUNIT_INCLUDE_DIR}
    ${Boost_INCLUDE_DIRS}
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/lenna.jpg ${CMAKE_CURRENT_BINARY_DIR}/lenna.jpg COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/circle.png ${CMAKE_CURRENT_BINARY_DIR}/circle.png COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/affine.npy ${CMAKE_CURRENT_BINARY_DIR}/affine.npy COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/perspective.npy ${CMAKE_CURRENT_BINARY_DIR}/perspective.npy COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/camera_matrix.npy ${CMAKE_CURRENT_BINARY_DIR}/camera_matrix.npy COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/dist_coeffs.npy ${CMAKE_CURRENT_BINARY_DIR}/dist_coeffs.npy COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/edges.png ${CMAKE_CURRENT_BINARY_DIR}/edges.png COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/contours.png ${CMAKE_CURRENT_BINARY_DIR}/contours.png COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/corners.png ${CMAKE_CURRENT_BINARY_DIR}/corners.png COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/corners.npy ${CMAKE_CURRENT_BINARY_DIR}/corners.npy COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/contour_1.npy ${CMAKE_CURRENT_BINARY_DIR}/contour_1.npy COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/contour_2.npy ${CMAKE_CURRENT_BINARY_DIR}/contour_2.npy COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/contour_f64.npy ${CMAKE_CURRENT_BINARY_DIR}/contour_f64.npy COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/contour_f32.npy ${CMAKE_CURRENT_BINARY_DIR}/contour_f32.npy COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/non_convex_f32.npy ${CMAKE_CURRENT_BINARY_DIR}/non_convex_f32.npy COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/points_i32.npy ${CMAKE_CURRENT_BINARY_DIR}/points_i32.npy COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/points_f32.npy ${CMAKE_CURRENT_BINARY_DIR}/points_f32.npy COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/points_f64.npy ${CMAKE_CURRENT_BINARY_DIR}/points_f64.npy COPYONLY)

set (SOURCES 
    ../BilateralFilter.cpp
    ../Blur.cpp
    ../BoxFilter.cpp
    ../Dilate.cpp
    ../Erode.cpp
    ../GaussianBlur.cpp
    ../MedianBlur.cpp
    ../MorphologyEx.cpp
    ../Laplacian.cpp
    ../PyrDown.cpp
    ../PyrUp.cpp
    ../Scharr.cpp
    ../Sobel.cpp
    ../Resize.cpp
    ../AdaptiveThreshold.cpp
    ../Threshold.cpp
    ../WarpAffine.cpp
    ../WarpPerspective.cpp
    ../Undistort.cpp
    ../UndistortPoints.cpp
    ../DistanceTransform.cpp
    ../FloodFill.cpp
    ../Integral.cpp
    ../CalcHist1D.cpp
    ../FindContours.cpp
    ../DrawContours.cpp
    ../ApproxPolyDP.cpp
    ../BoundingRect.cpp
    ../ConvexHull.cpp
    ../FitEllipse.cpp
    ../FitLine.cpp
    ../MinAreaRect.cpp
    ../MinEnclosingCircle.cpp
    ../Canny.cpp
    ../CornerHarris.cpp
    ../CornerMinEigenVal.cpp
    ../CornerSubPix.cpp
    ../GoodFeaturesToTrack.cpp
    ../HoughLinesP.cpp
    ../PreCornerDetect.cpp
    ../Utility.cpp
    ../Cvimgproc.cpp
    BilateralFilterTest.cpp
    BlurTest.cpp
    BoxFilterTest.cpp
    DilateTest.cpp
    ErodeTest.cpp
    GaussianBlurTest.cpp
    MedianBlurTest.cpp
    MorphologyExTest.cpp
    LaplacianTest.cpp
    PyrDownTest.cpp
    PyrUpTest.cpp
    ScharrTest.cpp
    SobelTest.cpp
    ResizeTest.cpp
    AdaptiveThresholdTest.cpp
    ThresholdTest.cpp
    WarpAffineTest.cpp
    WarpPerspectiveTest.cpp
    UndistortTest.cpp
    UndistortPointsTest.cpp
    DistanceTransformTest.cpp
    FloodFillTest.cpp
    IntegralTest.cpp
    CalcHist1DTest.cpp
    FindContoursTest.cpp
    DrawContoursTest.cpp
    ApproxPolyDPTest.cpp
    BoundingRectTest.cpp
    ConvexHullTest.cpp
    FitEllipseTest.cpp
    FitLineTest.cpp
    MinAreaRectTest.cpp
    MinEnclosingCircleTest.cpp
    CannyTest.cpp
    CornerHarrisTest.cpp
    CornerMinEigenValTest.cpp
    CornerSubPixTest.cpp
    GoodFeaturesToTrackTest.cpp
    HoughLinesPTest.cpp
    PreCornerDetectTest.cpp
    main.cpp
)

add_executable(stromx_cvimgproc_test ${SOURCES})

set_target_properties(stromx_cvimgproc_test PROPERTIES FOLDER "test")

target_link_libraries (stromx_cvimgproc_test
    ${CPPUNIT_LIBRARY}
    ${CMAKE_DL_LIBS}
    ${OpenCV_LIBS}
    stromx_runtime
    stromx_cvsupport
)

