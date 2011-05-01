project(streamtest)

include_directories(../..)
add_executable(streamtest main.cpp DataContainerTest.cpp)

target_link_libraries(streamtest stream cppunit dl)
