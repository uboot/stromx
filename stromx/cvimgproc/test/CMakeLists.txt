add_test(NAME stromx_cvimgproc_test COMMAND stromx_cvimgproc_test)

if(MSVC)
    add_definitions(/DSTROMX_CVIMGPROC_STATIC)
endif(MSVC)

include_directories (
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${CPPUNIT_INCLUDE_DIR}
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/lenna.jpg DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/circle.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/affine.npy DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/perspective.npy DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/camera_matrix.npy DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/dist_coeffs.npy DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/points_2d.npy DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/edges.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

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
    ../Canny.cpp
    ../CornerHarris.cpp
    ../CornerMinEigenVal.cpp
    ../HoughLinesP.cpp
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
    CannyTest.cpp
    CornerHarrisTest.cpp
    CornerMinEigenValTest.cpp
    HoughLinesPTest.cpp
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

