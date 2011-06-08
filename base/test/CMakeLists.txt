project(base_test)

include_directories(..)
include_directories(../..)
include_directories(${CPPUNIT_INCLUDE_DIR})

add_executable(base_test
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
