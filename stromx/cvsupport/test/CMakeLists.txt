add_test(NAME stromx_cvsupport_test COMMAND stromx_cvsupport_test)

if(MSVC)
    add_definitions(/DSTROMX_CVSUPPORT_STATIC)
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

add_executable(stromx_cvsupport_test
    ../AdjustRgbChannels.cpp
    ../Cvsupport.cpp
    ../Buffer.cpp
    ../Clip.cpp
    ../ConstImage.cpp
    ../ConvertPixelType.cpp
    ../DummyCamera.cpp
    ../Image.cpp
    ../Matrix.cpp
    ../Utilities.cpp
    ../impl/CameraBuffer.cpp
    AdjustRgbChannelsTest.cpp
    BufferTest.cpp
    CameraBufferTest.cpp
    ClipTest.cpp
    ConvertPixelTypeTest.cpp
    ConstImageTest.cpp
    DummyCameraTest.cpp
    ImageTest.cpp
    CvsupportTest.cpp
    main.cpp
    MatrixTest.cpp
#     SendReceiveTest.cpp
    UtilitiesTest.cpp
)

set_target_properties(stromx_cvsupport_test PROPERTIES
    FOLDER "test"
)

target_link_libraries(stromx_cvsupport_test 
    ${CPPUNIT_LIBRARY}
    ${CMAKE_DL_LIBS}
    ${OpenCV_LIBS}
    stromx_runtime
)
