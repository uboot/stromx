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
    AdjustRgbChannels.cpp
    Base.cpp
    Camera.cpp
    impl/CameraBuffer.cpp
    Clip.cpp
    ConvertPixelType.cpp
    ConstImage.cpp
    Image.cpp
    Queue.cpp
    PeriodicDelay.cpp
    Trigger.cpp
    Utilities.cpp
)


include_directories(${OpenCV_ROOT_DIR})

add_library(stream_base SHARED ${SOURCES})

set(VERSION_STRING "${BASE_VERSION_MAJOR}.${BASE_VERSION_MINOR}")

set_target_properties(stream_base PROPERTIES
    VERSION ${VERSION_STRING}
    SOVERSION ${VERSION_STRING}
)

target_link_libraries(stream_base
    ${OpenCV2_CORE_LIBRARY}
    ${OpenCV2_HIGHGUI_LIBRARY}
)