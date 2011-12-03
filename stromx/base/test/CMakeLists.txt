project(stromx_base_test)

if(MSVC)
    add_definitions(/DSTROMX_BASE_STATIC)
endif(MSVC)

include_directories(
    ../../..
    ${CPPUNIT_INCLUDE_DIR}
    ${Boost_INCLUDE_DIRS}
	${OpenCV2_INCLUDE_DIR}
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/lenna.jpg DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(stromx_base_test
    ../AdjustRgbChannels.cpp
    AdjustRgbChannelsTest.cpp
    ../Base.cpp
    BaseTest.cpp
    ../impl/CameraBuffer.cpp
    CameraBufferTest.cpp
    ../Camera.cpp
    CameraTest.cpp
    ../Clip.cpp
    ../Utilities.cpp
    ClipTest.cpp
    ../ConvertPixelType.cpp
    ConvertPixelTypeTest.cpp
    ../ConstImage.cpp
    ConstImageTest.cpp
    ../Counter.cpp
    CounterTest.cpp
    ../Image.cpp
    ImageTest.cpp
    main.cpp
    ../Queue.cpp
    QueueTest.cpp
    ../PeriodicDelay.cpp
    PeriodicDelayTest.cpp
    ../Trigger.cpp
    TriggerTest.cpp
)

target_link_libraries(stromx_base_test 
    ${CPPUNIT_LIBRARY}
    ${CMAKE_DL_LIBS}
    ${OpenCV2_CORE_LIBRARY}
    ${OpenCV2_IMGPROC_LIBRARY}
    ${OpenCV2_HIGHGUI_LIBRARY}
    stromx_core
)
