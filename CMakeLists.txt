project(stromx)

set (STROMX_VERSION_MAJOR 0)
set (STROMX_VERSION_MINOR 1)
set (STROMX_VERSION_PATCH 0)

cmake_minimum_required(VERSION 2.8)
enable_testing()

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)
set(LIB_DIR lib${LIB_SUFFIX})
if(NOT PYTHON_INSTALL_DIR)
    set(PYTHON_INSTALL_DIR "" CACHE FILEPATH "Python install directory")
endif()

if(NOT DOC_INSTALL_DIR)
    set(DOC_INSTALL_DIR "" CACHE FILEPATH "Documentation install directory")
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Wextra")
#   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Wextra -Werror")
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
find_package(Boost 1.36.0 COMPONENTS thread date_time filesystem regex system)

find_package(CppUnit)

find_package(Threads)

find_package(OpenCV)

find_package(XercesC)

find_package(LibZip)

if(CPPUNIT_FOUND)
    option(BUILD_TESTS "Build unit tests" ON)
endif()

if(Boost_FOUND)
    set(BUILD_RUNTIME true)
endif()

if(OpenCV_FOUND)
    option(BUILD_EXAMPLE "Build example operators" ON)
endif()

if(XERCES_FOUND AND LIBZIP_FOUND)
    option(BUILD_FILE_PERSISTENCE "Build file persistence" ON)
endif()

if(BUILD_RUNTIME)
    add_subdirectory(stromx/runtime)
endif()

if(BUILD_EXAMPLE)
   add_subdirectory(stromx/example)
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

add_subdirectory(doc)

configure_file (
    "${PROJECT_SOURCE_DIR}/stromxConfig.cmake.in"
    "${PROJECT_BINARY_DIR}/stromxConfig.cmake"
    @ONLY
)

if(WIN32)
    install(FILES "${PROJECT_BINARY_DIR}/stromxConfig.cmake"
        DESTINATION .
    )
else(WIN32)
    install(FILES "${PROJECT_BINARY_DIR}/stromxConfig.cmake"
        DESTINATION share/stromx
    )
endif(WIN32)
