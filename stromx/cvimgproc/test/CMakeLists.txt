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
    ../Utility.cpp
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
    main.cpp
)

add_executable(stromx_cvimgproc_test ${SOURCES})

set_target_properties(stromx_cvimgproc_test PROPERTIES FOLDER "test")

add_dependencies(stromx_cvimgproc_test stromx_cvimgproc)

target_link_libraries (stromx_cvimgproc_test
    ${CPPUNIT_LIBRARY}
    ${CMAKE_DL_LIBS}
    ${OpenCV_LIBS}
    stromx_runtime
    stromx_cvimgutil
)

