project(base)

set (BASE_VERSION_MAJOR 0)
set (BASE_VERSION_MINOR 1)
set (PACKAGE_NAME "\"Base\"")

configure_file (
  "${PROJECT_SOURCE_DIR}/Config.h.in"
  "${PROJECT_BINARY_DIR}/Config.h"
)

if(BUILD_TESTS)
    add_subdirectory(test)
endif()

include_directories(
    ..
    ${PROJECT_BINARY_DIR}
)

set(SOURCES
    Clip.cpp
    Image.cpp
    Utilities.cpp
)


include_directories(${OpenCV_ROOT_DIR})
add_library(stream_base_operators SHARED ${SOURCES})
target_link_libraries(stream_base_operators
    ${OpenCV2_CORE_LIBRARY}
    ${OpenCV2_HIGHGUI_LIBRARY}
)