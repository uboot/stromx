project(stromx_base_test)

add_test(NAME stromx_base_test COMMAND stromx_base_test)

if(MSVC)
    add_definitions(/DSTROMX_BASE_STATIC)
endif(MSVC)

include_directories(
    ../../..
    ${CPPUNIT_INCLUDE_DIR}
    ${Boost_INCLUDE_DIRS}
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/lenna.jpg DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/lenna_bw.jpg DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(stromx_base_test
    ../AdjustRgbChannels.cpp
    ../Base.cpp
    ../Blur.cpp
    ../Buffer.cpp
    ../Camera.cpp
    ../Canny.cpp
    ../Clip.cpp
    ../ConstImage.cpp
    ../Counter.cpp
    ../ConvertPixelType.cpp
    ../Image.cpp
    ../PeriodicDelay.cpp
    ../Queue.cpp
    ../Trigger.cpp
    ../Utilities.cpp
    ../impl/CameraBuffer.cpp
    AdjustRgbChannelsTest.cpp
    BaseTest.cpp
    BlurTest.cpp
    BufferTest.cpp
    CameraBufferTest.cpp
    CameraTest.cpp
    CannyTest.cpp
    ClipTest.cpp
    ConvertPixelTypeTest.cpp
    ConstImageTest.cpp
    CounterTest.cpp
    ImageTest.cpp
    QueueTest.cpp
    PeriodicDelayTest.cpp
    TriggerTest.cpp
    main.cpp
)

target_link_libraries(stromx_base_test 
    ${CPPUNIT_LIBRARY}
    ${CMAKE_DL_LIBS}
    ${OpenCV_LIBS}
    stromx_core
)
