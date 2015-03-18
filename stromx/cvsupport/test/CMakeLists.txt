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

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/double_matrix.npy ${CMAKE_CURRENT_BINARY_DIR}/double_matrix.npy COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/edges.png ${CMAKE_CURRENT_BINARY_DIR}/edges.png COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/empty_float_matrix.npy ${CMAKE_CURRENT_BINARY_DIR}/empty_float_matrix.npy COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/lenna.jpg ${CMAKE_CURRENT_BINARY_DIR}/lenna.jpg COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/lenna_bw.jpg ${CMAKE_CURRENT_BINARY_DIR}/lenna_bw.jpg COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/uint16_matrix.npy ${CMAKE_CURRENT_BINARY_DIR}/uint16_matrix.npy COPYONLY)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/edges.png ${CMAKE_CURRENT_BINARY_DIR}/ReadDirectoryTest/directory/edges.png COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/lenna.jpg ${CMAKE_CURRENT_BINARY_DIR}/ReadDirectoryTest/directory/lenna.jpg COPYONLY)

add_executable(stromx_cvsupport_test
    ../AdjustRgbChannels.cpp
    ../Cvsupport.cpp
    ../Buffer.cpp
    ../Clip.cpp
    ../ConstImage.cpp
    ../ConvertPixelType.cpp
    ../DummyCamera.cpp
    ../Flicker.cpp
    ../Image.cpp
    ../Matrix.cpp
    ../ReadDirectory.cpp
    ../Utilities.cpp
    ../impl/CameraBuffer.cpp
    AdjustRgbChannelsTest.cpp
    BufferTest.cpp
    CameraBufferTest.cpp
    ClipTest.cpp
    ConvertPixelTypeTest.cpp
    ConstImageTest.cpp
    DummyCameraTest.cpp
    FlickerTest.cpp
    ImageTest.cpp
    CvsupportTest.cpp
    main.cpp
    MatrixTest.cpp
    ReadDirectoryTest.cpp
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
