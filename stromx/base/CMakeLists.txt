project(stromx_base)

set (BASE_VERSION_MAJOR 0)
set (BASE_VERSION_MINOR 1)
set (BASE_VERSION_PATCH 0)

configure_file (
  "${PROJECT_SOURCE_DIR}/Config.h.in"
  "${PROJECT_BINARY_DIR}/Config.h"
)

if(BUILD_TESTS)
    add_subdirectory(test)
endif()

include_directories(
    ../..
    ${PROJECT_BINARY_DIR}
    ${Boost_INCLUDE_DIRS}
	${OpenCV2_INCLUDE_DIR}
)

set(SOURCES
    AdjustRgbChannels.cpp
    Base.cpp
    Camera.cpp
    impl/CameraBuffer.cpp
    Clip.cpp
    ConvertPixelType.cpp
    ConstImage.cpp
    Counter.cpp
    Image.cpp
    Queue.cpp
    PeriodicDelay.cpp
    Trigger.cpp
    Utilities.cpp
)

add_library(stromx_base SHARED ${SOURCES})

set(VERSION_STRING "${BASE_VERSION_MAJOR}.${BASE_VERSION_MINOR}.${BASE_VERSION_PATCH}")

set_target_properties(stromx_base PROPERTIES
    VERSION ${VERSION_STRING}
    SOVERSION ${VERSION_STRING}
)

target_link_libraries(stromx_base
    ${OpenCV2_CORE_LIBRARY}
    ${OpenCV2_HIGHGUI_LIBRARY}
)