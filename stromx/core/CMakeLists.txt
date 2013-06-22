set (CORE_VERSION_MAJOR 0)
set (CORE_VERSION_MINOR 0)
set (CORE_VERSION_PATCH 1)

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
    Core.cpp
    Utility.cpp
    Add.cpp
)

add_library (stromx_core SHARED ${SOURCES})

add_dependencies(stromx_core stromx_runtime)

set(VERSION_STRING "${CORE_VERSION_MAJOR}.${CORE_VERSION_MINOR}.${CORE_VERSION_PATCH}")

set_target_properties (stromx_core PROPERTIES
    VERSION ${VERSION_STRING}
    SOVERSION ${VERSION_STRING}
    FOLDER "library"
)

target_link_libraries (stromx_core
    ${OpenCV_LIBS}
    stromx_runtime
    stromx_imgutil
)

if(WIN32)
    install (TARGETS stromx_core
        RUNTIME DESTINATION .
        LIBRARY DESTINATION ${LIB_DIR}
        ARCHIVE DESTINATION ${LIB_DIR}
    )
else(WIN32)
    install (TARGETS stromx_core
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION ${LIB_DIR}
        ARCHIVE DESTINATION ${LIB_DIR}
    )
endif(WIN32)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/stromx/core
    DESTINATION include/stromx
    FILES_MATCHING PATTERN "*.h"
    PATTERN "test*" EXCLUDE
)

install(DIRECTORY ${CMAKE_BINARY_DIR}/stromx/core
    DESTINATION include/stromx
    FILES_MATCHING PATTERN "*.h"
)

if(BUILD_TESTS)
    add_subdirectory(test)
endif(BUILD_TESTS)
