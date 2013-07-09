set (CVCORE_VERSION_MAJOR 0)
set (CVCORE_VERSION_MINOR 0)
set (CVCORE_VERSION_PATCH 1)

configure_file (
    ${CMAKE_CURRENT_SOURCE_DIR}/Config.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/Config.h
)

include_directories (
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
)

set (SOURCES 
    Cvcore.cpp
    Utility.cpp
    Absdiff.cpp
    Add.cpp
)

add_library (stromx_cvcore SHARED ${SOURCES})

add_dependencies(stromx_cvcore stromx_runtime)

set(VERSION_STRING "${CVCORE_VERSION_MAJOR}.${CVCORE_VERSION_MINOR}.${CVCORE_VERSION_PATCH}")

set_target_properties (stromx_cvcore PROPERTIES
    VERSION ${VERSION_STRING}
    SOVERSION ${VERSION_STRING}
    FOLDER "library"
)

target_link_libraries (stromx_cvcore
    ${OpenCV_LIBS}
    stromx_runtime
    stromx_cvsupport
)

if(WIN32)
    install (TARGETS stromx_cvcore
        RUNTIME DESTINATION .
        LIBRARY DESTINATION ${LIB_DIR}
        ARCHIVE DESTINATION ${LIB_DIR}
    )
else(WIN32)
    install (TARGETS stromx_cvcore
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION ${LIB_DIR}
        ARCHIVE DESTINATION ${LIB_DIR}
    )
endif(WIN32)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/stromx/cvcore
    DESTINATION include/stromx
    FILES_MATCHING PATTERN "*.h"
    PATTERN "test*" EXCLUDE
)

install(DIRECTORY ${CMAKE_BINARY_DIR}/stromx/cvcore
    DESTINATION include/stromx
    FILES_MATCHING PATTERN "*.h"
)

if(BUILD_TESTS)
    add_subdirectory(test)
endif(BUILD_TESTS)
