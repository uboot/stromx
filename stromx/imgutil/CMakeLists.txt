project(stromx_imgutil)

set (STROMX_IMGUTIL_VERSION_MAJOR 0)
set (STROMX_IMGUTIL_VERSION_MINOR 2)
set (STROMX_IMGUTIL_VERSION_PATCH 0)

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
    Buffer.cpp
    Camera.cpp
    Clip.cpp
    ConvertPixelType.cpp
    ConstImage.cpp
    Image.cpp
    Imgutil.cpp
    Matrix.cpp
    Utilities.cpp
    WebCamera.cpp
    impl/CameraBuffer.cpp
)

add_library(stromx_imgutil SHARED ${SOURCES})

set(VERSION_STRING "${STROMX_IMGUTIL_VERSION_MAJOR}.${STROMX_IMGUTIL_VERSION_MINOR}.${STROMX_IMGUTIL_VERSION_PATCH}")

set_target_properties(stromx_imgutil PROPERTIES
    VERSION ${VERSION_STRING}
    SOVERSION ${VERSION_STRING}
)

target_link_libraries(stromx_imgutil
    ${OpenCV_LIBS}
    stromx_runtime
)


if(WIN32)    
    install (TARGETS stromx_imgutil
        RUNTIME DESTINATION .
        LIBRARY DESTINATION ${LIB_DIR}
        ARCHIVE DESTINATION ${LIB_DIR}
    )

else(WIN32)
    install (TARGETS stromx_imgutil
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION ${LIB_DIR}
        ARCHIVE DESTINATION ${LIB_DIR}
    )
endif(WIN32)

install(DIRECTORY . 
    DESTINATION include/stromx/imgutil
    FILES_MATCHING PATTERN "*.h"
    PATTERN "test*" EXCLUDE
)