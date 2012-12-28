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
)

set(SOURCES
    AdjustRgbChannels.cpp
    Base.cpp
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

add_library(stromx_base SHARED ${SOURCES})

set(VERSION_STRING "${BASE_VERSION_MAJOR}.${BASE_VERSION_MINOR}.${BASE_VERSION_PATCH}")

set_target_properties(stromx_base PROPERTIES
    VERSION ${VERSION_STRING}
    SOVERSION ${VERSION_STRING}
)

target_link_libraries(stromx_base
    ${OpenCV_LIBS}
    stromx_core
)


if(WIN32)    
    install (TARGETS stromx_base
        RUNTIME DESTINATION .
        LIBRARY DESTINATION ${LIB_DIR}
        ARCHIVE DESTINATION ${LIB_DIR}
    )

else(WIN32)
    install (TARGETS stromx_base
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION ${LIB_DIR}
        ARCHIVE DESTINATION ${LIB_DIR}
    )
endif(WIN32)

install(DIRECTORY . 
    DESTINATION include/stromx/base
    FILES_MATCHING PATTERN "*.h"
    PATTERN "test*" EXCLUDE
)