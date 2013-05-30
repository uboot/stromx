add_test(NAME stromx_imgutil_test COMMAND stromx_imgutil_test)

if(MSVC)
    add_definitions(/DSTROMX_IMGUTIL_STATIC)
endif(MSVC)

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${CPPUNIT_INCLUDE_DIR}
    ${Boost_INCLUDE_DIRS}
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/double_matrix.npy DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/edges.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/empty_float_matrix.npy DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/lenna.jpg DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/lenna_bw.jpg DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/uint16_matrix.npy DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(stromx_imgutil_test
    ../AdjustRgbChannels.cpp
    ../Imgutil.cpp
    ../Buffer.cpp
    ../Camera.cpp
    ../Clip.cpp
    ../ConstImage.cpp
    ../ConvertPixelType.cpp
    ../Image.cpp
    ../Matrix.cpp
    ../Utilities.cpp
    ../WebCamera.cpp
    ../impl/CameraBuffer.cpp
    AdjustRgbChannelsTest.cpp
    BufferTest.cpp
    CameraBufferTest.cpp
    CameraTest.cpp
    ClipTest.cpp
    ConvertPixelTypeTest.cpp
    ConstImageTest.cpp
    ImageTest.cpp
    ImgutilTest.cpp
    main.cpp
    MatrixTest.cpp
    WebCameraTest.cpp
)

target_link_libraries(stromx_imgutil_test 
    ${CPPUNIT_LIBRARY}
    ${CMAKE_DL_LIBS}
    ${OpenCV_LIBS}
    stromx_runtime
)

add_dependencies(stromx_imgutil_test stromx_imgutil)
