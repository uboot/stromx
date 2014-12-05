set (STROMX_CVHIGHGUI_VERSION_MAJOR 0)
set (STROMX_CVHIGHGUI_VERSION_MINOR 0)
set (STROMX_CVHIGHGUI_VERSION_PATCH 1)

configure_file (
    ${CMAKE_CURRENT_SOURCE_DIR}/Config.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/Config.h
)

include_directories (
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${Boost_INCLUDE_DIRS}
)

set (SOURCES 
    Cvhighgui.cpp
    VideoCapture.cpp
)

add_library (stromx_cvhighgui SHARED ${SOURCES})

set(VERSION_STRING "${STROMX_CVHIGHGUI_VERSION_MAJOR}.${STROMX_CVHIGHGUI_VERSION_MINOR}.${STROMX_CVHIGHGUI_VERSION_PATCH}")

set_target_properties (stromx_cvhighgui PROPERTIES
    VERSION ${VERSION_STRING}
    SOVERSION ${VERSION_STRING}
    FOLDER "library"
)

target_link_libraries (stromx_cvhighgui
    ${OpenCV_LIBS}
    stromx_runtime
    stromx_cvsupport
)

if(WIN32)
    install (TARGETS stromx_cvhighgui
        RUNTIME DESTINATION .
        LIBRARY DESTINATION ${LIB_DIR}
        ARCHIVE DESTINATION ${LIB_DIR}
    )
else(WIN32)
    install (TARGETS stromx_cvhighgui
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION ${LIB_DIR}
        ARCHIVE DESTINATION ${LIB_DIR}
    )
endif(WIN32)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/stromx/cvhighgui
    DESTINATION include/stromx
    FILES_MATCHING PATTERN "*.h"
    PATTERN "test*" EXCLUDE
)

install(DIRECTORY ${CMAKE_BINARY_DIR}/stromx/cvhighgui
    DESTINATION include/stromx
    FILES_MATCHING PATTERN "*.h"
)

if(BUILD_TESTS)
    add_subdirectory(test)
endif(BUILD_TESTS)
