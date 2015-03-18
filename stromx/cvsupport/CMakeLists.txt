set (STROMX_CVSUPPORT_VERSION_MAJOR ${STROMX_VERSION_MAJOR})
set (STROMX_CVSUPPORT_VERSION_MINOR ${STROMX_VERSION_MINOR})
set (STROMX_CVSUPPORT_VERSION_PATCH ${STROMX_VERSION_PATCH})

configure_file (
  ${CMAKE_CURRENT_SOURCE_DIR}/Config.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/Config.h
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
    Buffer.cpp
    Clip.cpp
    ConvertPixelType.cpp
    ConstImage.cpp
    DummyCamera.cpp
    Flicker.cpp
    Image.cpp
    Cvsupport.cpp
    Matrix.cpp
    Utilities.cpp
    ReadDirectory.cpp
    impl/CameraBuffer.cpp
)

add_library(stromx_cvsupport SHARED ${SOURCES})

set(VERSION_STRING "${STROMX_CVSUPPORT_VERSION_MAJOR}.${STROMX_CVSUPPORT_VERSION_MINOR}.${STROMX_CVSUPPORT_VERSION_PATCH}")

set_target_properties(stromx_cvsupport PROPERTIES
    VERSION ${VERSION_STRING}
    SOVERSION ${VERSION_STRING}
    FOLDER "library"
)

target_link_libraries(stromx_cvsupport
    ${OpenCV_LIBS}
    stromx_runtime
)

if(WIN32)    
    install (TARGETS stromx_cvsupport
        RUNTIME DESTINATION .
        LIBRARY DESTINATION ${LIB_DIR}
        ARCHIVE DESTINATION ${LIB_DIR}
    )

else(WIN32)
    install (TARGETS stromx_cvsupport
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION ${LIB_DIR}
        ARCHIVE DESTINATION ${LIB_DIR}
    )
endif(WIN32)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/stromx/cvsupport
    DESTINATION include/stromx
    FILES_MATCHING PATTERN "*.h"
    PATTERN "test*" EXCLUDE
)

install(DIRECTORY ${CMAKE_BINARY_DIR}/stromx/cvsupport
    DESTINATION include/stromx
    FILES_MATCHING PATTERN "*.h"
)
