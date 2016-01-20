set (STROMX_CVOBJDETECT_VERSION_MAJOR 0)
set (STROMX_CVOBJDETECT_VERSION_MINOR 1)
set (STROMX_CVOBJDETECT_VERSION_PATCH 0)

configure_file (
    ${CMAKE_CURRENT_SOURCE_DIR}/Config.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/Config.h
)

include_directories (
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
)

set (SOURCES 
    Cvobjdetect.cpp
    Utility.cpp
    CascadeClassifier.cpp
)

add_library (stromx_cvobjdetect SHARED ${SOURCES})

set(VERSION_STRING "${STROMX_CVOBJDETECT_VERSION_MAJOR}.${STROMX_CVOBJDETECT_VERSION_MINOR}.${STROMX_CVOBJDETECT_VERSION_PATCH}")

set_target_properties (stromx_cvobjdetect PROPERTIES
    VERSION ${VERSION_STRING}
    SOVERSION ${VERSION_STRING}
    FOLDER "library"
)

target_link_libraries (stromx_cvobjdetect
    ${OpenCV_LIBS}
    stromx_runtime
    stromx_cvsupport
)

if(WIN32)
    install (TARGETS stromx_cvobjdetect
        RUNTIME DESTINATION .
        LIBRARY DESTINATION ${LIB_DIR}
        ARCHIVE DESTINATION ${LIB_DIR}
    )
else(WIN32)
    install (TARGETS stromx_cvobjdetect
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION ${LIB_DIR}
        ARCHIVE DESTINATION ${LIB_DIR}
    )
endif(WIN32)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/stromx/cvobjdetect
    DESTINATION include/stromx
    FILES_MATCHING PATTERN "*.h"
    PATTERN "test" EXCLUDE
)

install(FILES ${CMAKE_BINARY_DIR}/stromx/cvobjdetect/Config.h
    DESTINATION include/stromx/cvobjdetect
)

if(BUILD_TESTS)
    add_subdirectory(test)
endif(BUILD_TESTS)
