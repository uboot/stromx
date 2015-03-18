project(stromx)

set (STROMX_VERSION_MAJOR 0)
set (STROMX_VERSION_MINOR 5)
set (STROMX_VERSION_PATCH 0)

cmake_minimum_required(VERSION 2.8)
enable_testing()

include(CheckTypeSize)
check_type_size("float" FLOAT_SIZE)
check_type_size("double" DOUBLE_SIZE)
if(NOT FLOAT_SIZE EQUAL 4)
    message(FATAL_ERROR "Float must be 4 bytes.")
endif()
if(NOT DOUBLE_SIZE EQUAL 8)
    message(FATAL_ERROR "Double must be 8 bytes.")
endif()

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

include(FindGettext)
include(UseDoxygen OPTIONAL)

set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost 1.50.0 REQUIRED COMPONENTS chrono date_time filesystem locale regex serialization system thread timer)

find_package(CppUnit)

find_package(Threads)

find_package(OpenCV COMPONENTS calib3d core imgproc highgui)

find_package(XercesC)

find_package(LibZip)

if(CPPUNIT_FOUND)
    option(BUILD_TESTS "Build unit tests" ON)
endif()

if(OpenCV_FOUND)
    option(BUILD_OPENCV_WRAPPER "Build OpenCV packages" ON)
endif()

if(XERCES_FOUND AND LIBZIP_FOUND)
    option(BUILD_FILE_PERSISTENCE "Build file persistence" ON)
endif()

if(WIN32)
    set(LOCALE_SUFFIX "locale")
else(WIN32)
    set(LOCALE_SUFFIX "share/locale")
endif(WIN32)

set(LOCALE_DIR "${CMAKE_INSTALL_PREFIX}/${LOCALE_SUFFIX}")

add_subdirectory(stromx)
add_subdirectory(doc)

if (BUILD_OPENCV_WRAPPER AND BUILD_FILE_PERSISTENCE)
    add_subdirectory(example)
endif()

find_path(mmal_DIR "interface/mmal/mmal.h" HINTS /opt/vc/include)
if (mmal_DIR)
    option(BUILD_RASPI "Build raspi package" ON)
endif()

find_package(Boost 1.50.0 COMPONENTS filesystem python thread)
find_package(PythonLibs)

if(Boost_FOUND AND PYTHON_INCLUDE_PATH)
    option(BUILD_PYTHON "Build Python wrapper" ON)
endif()

if(BUILD_PYTHON)
    add_subdirectory(python)
endif()

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

