project(stream)

cmake_minimum_required(VERSION 2.6)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)

include(UseDoxygen OPTIONAL)

find_package(Boost 1.36.0)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.36.0 COMPONENTS thread)

find_package(CppUnit)

find_package(Threads)

if(CPPUNIT_FOUND)
    option(BUILD_TESTS "Build unit tests" ON)
endif()

if(Boost_FOUND)
    set(BUILD_STREAM true)
endif()


if(BUILD_STREAM)
    add_subdirectory(stream)
    add_subdirectory(client)
endif()