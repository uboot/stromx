cmake_minimum_required(VERSION 2.8)
project(stromx)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)

SET(GCC_DEBUG_FLAGS "-g")

# http://stackoverflow.com/a/11437693
# Add a coverage build type
set(CMAKE_CXX_FLAGS_COVERAGE
    "${GCC_DEBUG_FLAGS} --coverage"
    CACHE STRING "Flags used by the C++ compiler during coverage builds."
    FORCE )
set(CMAKE_C_FLAGS_COVERAGE
    "${GCC_DEBUG_FLAGS} --coverage"
    CACHE STRING "Flags used by the C compiler during coverage builds."
    FORCE )
set(CMAKE_EXE_LINKER_FLAGS_COVERAGE
    ""
    CACHE STRING "Flags used for linking binaries during coverage builds."
    FORCE )
set(CMAKE_SHARED_LINKER_FLAGS_COVERAGE
    ""
    CACHE STRING "Flags used by the shared libraries linker during coverage builds."
    FORCE )
mark_as_advanced(
    CMAKE_CXX_FLAGS_COVERAGE
    CMAKE_C_FLAGS_COVERAGE
    CMAKE_EXE_LINKER_FLAGS_COVERAGE
    CMAKE_SHARED_LINKER_FLAGS_COVERAGE )

set(STROMX_VERSION_MAJOR 0)
set(STROMX_VERSION_MINOR 8)
set(STROMX_VERSION_PATCH 0)

include(GetGitRevisionDescription)
get_git_head_revision(REFSPEC GIT_HASH)
if (NOT GIT_HASH STREQUAL "GITDIR-NOTFOUND")
    string(SUBSTRING ${GIT_HASH} 0 6 GIT_HASH)
    set(STROMX_VERSION_STRING ${STROMX_VERSION_MAJOR}.${STROMX_VERSION_MINOR}.${STROMX_VERSION_PATCH}-${GIT_HASH})
else()
    set(STROMX_VERSION_STRING ${STROMX_VERSION_MAJOR}.${STROMX_VERSION_MINOR}.${STROMX_VERSION_PATCH})
endif()
message(STATUS "Stromx version: ${STROMX_VERSION_STRING}")

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

set(LIB_DIR lib${LIB_SUFFIX})
if(NOT PYTHON_INSTALL_DIR)
    set(PYTHON_INSTALL_DIR "" CACHE FILEPATH "Python install directory")
endif()

if(NOT DOC_INSTALL_DIR)
    set(DOC_INSTALL_DIR "" CACHE FILEPATH "Documentation install directory")
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -std=c++0x -pedantic -Wall -Wextra")
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

set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost 1.50.0 REQUIRED COMPONENTS chrono date_time filesystem locale regex serialization system thread timer)

find_package(CppUnit)

find_package(Threads)

find_package(OpenCV COMPONENTS calib3d core imgproc highgui ml objdetect)

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

# setup the LD_LIBRARY_PATH for unit tests on launchpad 
set(LD_LIBRARY_PATH "${CMAKE_BINARY_DIR}/stromx/runtime:${CMAKE_BINARY_DIR}/stromx/cvsupport:${CMAKE_BINARY_DIR}/stromx/test")

add_subdirectory(stromx)
add_subdirectory(doc)

if (BUILD_OPENCV_WRAPPER AND BUILD_FILE_PERSISTENCE)
    add_subdirectory(example)
endif()

find_package(PythonInterp 2.7)
find_package(PythonLibs 2.7)
find_package(Boost 1.50.0 COMPONENTS filesystem python thread)
if(PYTHONLIBS_FOUND AND PYTHONINTERP_FOUND AND Boost_FOUND)
    message("Found Python 2")
    option(BUILD_PYTHON "Build Python 2 wrapper" ON)
else()
    find_package(PythonInterp 3)
    find_package(PythonLibs 3)
    find_package(Boost 1.50.0 COMPONENTS filesystem python3 thread)
    if(PYTHONLIBS_FOUND AND PYTHONINTERP_FOUND AND Boost_FOUND)
        message("Found Python 3")
        option(BUILD_PYTHON "Build Python 3 wrapper" ON)
    endif()
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

