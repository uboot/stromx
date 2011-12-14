project(stromx_base)

set (BASE_VERSION_MAJOR 0)
set (BASE_VERSION_MINOR 1)
set (BASE_VERSION_PATCH 0)

configure_file (
  "${PROJECT_SOURCE_DIR}/Config.h.in"
  "${PROJECT_SOURCE_DIR}/Config.h"
)

if(BUILD_TESTS)
    add_subdirectory(test)
endif()

include_directories(
    ../..
    ${Boost_INCLUDE_DIRS}
	${OpenCV2_INCLUDE_DIR}
)

set(SOURCES
    AdjustRgbChannels.cpp
    Base.cpp
    Buffer.cpp
    Camera.cpp
    Canny.cpp
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
    ${OpenCV2_IMGPROC_LIBRARY}
    ${OpenCV2_HIGHGUI_LIBRARY}
    stromx_core
)

install (TARGETS stromx_base
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(DIRECTORY . 
    DESTINATION include/stromx/base
    FILES_MATCHING PATTERN "*.h"
    PATTERN "test*" EXCLUDE
)