set (STROMX_CVCALIB3D_VERSION_MAJOR 0)
set (STROMX_CVCALIB3D_VERSION_MINOR 0)
set (STROMX_CVCALIB3D_VERSION_PATCH 1)

configure_file (
    ${CMAKE_CURRENT_SOURCE_DIR}/Config.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/Config.h
)

include_directories (
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
)

set (SOURCES 
    Cvcalib3d.cpp
    Utility.cpp
    CalibrateCamera.cpp
    FindChessboardCorners.cpp
    GeneratePattern.cpp
    SolvePnP.cpp
)

add_library (stromx_cvcalib3d SHARED ${SOURCES})

set(VERSION_STRING "${STROMX_CVCALIB_3D_VERSION_MAJOR}.${STROMX_CVCALIB_3D_VERSION_MINOR}.${STROMX_CVCALIB_3D_VERSION_PATCH}")

set_target_properties (stromx_cvcalib3d PROPERTIES
    VERSION ${VERSION_STRING}
    SOVERSION ${VERSION_STRING}
    FOLDER "library"
)

target_link_libraries (stromx_cvcalib3d
    ${OpenCV_LIBS}
    stromx_runtime
    stromx_cvsupport
)

if(WIN32)
    install (TARGETS stromx_cvcalib3d
        RUNTIME DESTINATION .
        LIBRARY DESTINATION ${LIB_DIR}
        ARCHIVE DESTINATION ${LIB_DIR}
    )
else(WIN32)
    install (TARGETS stromx_cvcalib3d
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION ${LIB_DIR}
        ARCHIVE DESTINATION ${LIB_DIR}
    )
endif(WIN32)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/stromx/cvcalib3d
    DESTINATION include/stromx
    FILES_MATCHING PATTERN "*.h"
    PATTERN "test*" EXCLUDE
)

install(DIRECTORY ${CMAKE_BINARY_DIR}/stromx/cvcalib3d
    DESTINATION include/stromx
    FILES_MATCHING PATTERN "*.h"
)

if(BUILD_TESTS)
    add_subdirectory(test)
endif(BUILD_TESTS)
