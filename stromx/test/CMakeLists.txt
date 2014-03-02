set (TEST_VERSION_MAJOR 0)
set (TEST_VERSION_MINOR 0)
set (TEST_VERSION_PATCH 1)

configure_file (
    ${CMAKE_CURRENT_SOURCE_DIR}/Config.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/Config.h
)

include_directories (
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
)

set (SOURCES
    DeadlockOperator.cpp
    ExceptionOperator.cpp
    ParameterOperator.cpp
    RandomDataOperator.cpp
    Test.cpp
    TestDataOperator.cpp
)
    
add_library (stromx_test SHARED ${SOURCES})

add_dependencies(stromx_test stromx_runtime)

set(VERSION_STRING "${TEST_VERSION_MAJOR}.${TEST_VERSION_MINOR}.${TEST_VERSION_PATCH}")

set_target_properties (stromx_test PROPERTIES
    VERSION ${VERSION_STRING}
    SOVERSION ${VERSION_STRING}
    FOLDER "library"
)

target_link_libraries (stromx_test
    stromx_runtime
    stromx_cvsupport
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/stromx/test
    DESTINATION include/stromx
    FILES_MATCHING PATTERN "*.h"
    PATTERN "test*" EXCLUDE
)

install(DIRECTORY ${CMAKE_BINARY_DIR}/stromx/test
    DESTINATION include/stromx
    FILES_MATCHING PATTERN "*.h"
)