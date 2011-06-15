project(base_test)

include_directories(
    ../..
    ${CPPUNIT_INCLUDE_DIR}
    ${PROJECT_BINARY_DIR}/..
)

SET(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/lenna.jpg DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(base_test
    ../AdjustRgbChannels.cpp
    AdjustRgbChannelsTest.cpp
    ../Clip.cpp
    ../Utilities.cpp
    ClipTest.cpp
    ../Image.cpp
    ImageTest.cpp
    main.cpp
)

target_link_libraries(base_test 
    ${Boost_LIBRARIES}
    ${CPPUNIT_LIBRARY}
    ${CMAKE_DL_LIBS}
    ${CMAKE_THREAD_LIBS_INIT}
    ${OpenCV2_CORE_LIBRARY}
    ${OpenCV2_HIGHGUI_LIBRARY}
    stream
)
