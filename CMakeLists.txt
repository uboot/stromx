project(stromx)

cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)

if(CMAKE_COMPILER_IS_GNUCXX)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall")
#   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Wextra")
endif(CMAKE_COMPILER_IS_GNUCXX)

if(MSVC)
    add_definitions(/DBOOST_ALL_DYN_LINK)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4251 /wd4275 /wd4661")
endif(MSVC)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDEBUG)
endif(CMAKE_BUILD_TYPE STREQUAL "Debug")

include(UseDoxygen OPTIONAL)

find_package(Boost 1.36.0)

set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.36.0 COMPONENTS thread filesystem date_time system)

find_package(CppUnit)

find_package(Threads)

find_package(OpenCV2)

find_package(XercesC)

find_package(LibZip)

if(CPPUNIT_FOUND)
    option(BUILD_TESTS "Build unit tests" ON)
endif()

if(Boost_FOUND)
    set(BUILD_CORE true)
endif()

if(OpenCV2_FOUND)
    option(BUILD_BASE "Build base operators" ON)
endif()

if(XERCES_FOUND AND LIBZIP_FOUND)
    option(BUILD_FILE_PERSISTENCE "Build file persistence" ON)
endif()

if(BUILD_CORE)
    add_subdirectory(stromx/core)
endif()

if(BUILD_BASE)
   add_subdirectory(stromx/base)
   add_subdirectory(example)
endif()

find_package(Boost 1.36.0 COMPONENTS thread filesystem python)
find_package(PythonLibs)

if(Boost_FOUND AND PYTHON_INCLUDE_PATH)
    option(BUILD_PYTHON "Build Python wrapper" ON)
endif()

if(BUILD_PYTHON)
    add_subdirectory(python)
endif()
