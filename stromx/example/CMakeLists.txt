project(stromx_example)

set (STROMX_EXAMPLE_VERSION_MAJOR 0)
set (STROMX_EXAMPLE_VERSION_MINOR 2)
set (STROMX_EXAMPLE_VERSION_PATCH 0)

configure_file (
  "Config.h.in"
  "Config.h"
)

if(BUILD_TESTS)
    add_subdirectory(test)
endif()

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${Boost_INCLUDE_DIRS}
)

set(SOURCES
    AdjustRgbChannels.cpp
    Example.cpp
    Blur.cpp
    Buffer.cpp
    Camera.cpp
    Canny.cpp
    Close.cpp
    impl/CameraBuffer.cpp
    Clip.cpp
    ConvertPixelType.cpp
    ConstImage.cpp
    Dilate.cpp
    Erode.cpp
    HoughLinesP.cpp
    Image.cpp
    ImageFilter.cpp
    Matrix.cpp
    MorphologicalFilter.cpp
    Open.cpp
    Utilities.cpp
    WebCamera.cpp
)

add_library(stromx_example SHARED ${SOURCES})

set(VERSION_STRING "${STROMX_EXAMPLE_VERSION_MAJOR}.${STROMX_EXAMPLE_VERSION_MINOR}.${STROMX_EXAMPLE_VERSION_PATCH}")

set_target_properties(stromx_example PROPERTIES
    VERSION ${VERSION_STRING}
    SOVERSION ${VERSION_STRING}
)

target_link_libraries(stromx_example
    ${OpenCV_LIBS}
    stromx_runtime
)


if(WIN32)    
    install (TARGETS stromx_example
        RUNTIME DESTINATION .
        LIBRARY DESTINATION ${LIB_DIR}
        ARCHIVE DESTINATION ${LIB_DIR}
    )

else(WIN32)
    install (TARGETS stromx_example
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION ${LIB_DIR}
        ARCHIVE DESTINATION ${LIB_DIR}
    )
endif(WIN32)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/stromx/example
    DESTINATION include/stromx
    FILES_MATCHING PATTERN "*.h"
    PATTERN "test*" EXCLUDE
)

install(DIRECTORY ${CMAKE_BINARY_DIR}/stromx/example
    DESTINATION include/stromx
    FILES_MATCHING PATTERN "*.h"
)
