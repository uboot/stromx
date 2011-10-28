project(stromx)

cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)

include(UseDoxygen OPTIONAL)

find_package(Boost 1.36.0)

set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.36.0 COMPONENTS thread filesystem)

find_package(CppUnit)

find_package(Threads)

find_package(OpenCV2)

find_package(XercesC)

if(CPPUNIT_FOUND)
    option(BUILD_TESTS "Build unit tests" ON)
endif()

if(Boost_FOUND)
    set(BUILD_CORE true)
endif()

if(OpenCV2_FOUND)
    option(BUILD_BASE "Build base operators" ON)
endif()

if(XERCES_FOUND)
    option(BUILD_XML_SUPPORT "Build XML persistence" ON)
endif()

if(BUILD_CORE)
    add_subdirectory(stromx/core)
endif()

if(BUILD_BASE)
   add_subdirectory(stromx/base)
   add_subdirectory(example)
endif()
