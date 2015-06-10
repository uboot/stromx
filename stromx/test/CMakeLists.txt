set (TEST_VERSION_MAJOR 0)
set (TEST_VERSION_MINOR 1)
set (TEST_VERSION_PATCH 0)

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

if(WIN32)
    install (TARGETS stromx_test 
        RUNTIME DESTINATION .
        LIBRARY DESTINATION ${LIB_DIR}
        ARCHIVE DESTINATION ${LIB_DIR}
    )
else(WIN32)
    install (TARGETS stromx_test 
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION ${LIB_DIR}
        ARCHIVE DESTINATION ${LIB_DIR}
    )
endif(WIN32)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/stromx/test
    DESTINATION include/stromx
    FILES_MATCHING PATTERN "*.h"
)

install(FILES ${CMAKE_BINARY_DIR}/stromx/test/Config.h
    DESTINATION include/stromx/test
)