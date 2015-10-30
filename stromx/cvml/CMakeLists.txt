set (STROMX_CVML_VERSION_MAJOR ${STROMX_VERSION_MAJOR})
set (STROMX_CVML_VERSION_MINOR ${STROMX_VERSION_MINOR})
set (STROMX_CVML_VERSION_PATCH ${STROMX_VERSION_PATCH})

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
    Cvml.cpp
    Svm.cpp
)

add_library(stromx_cvml SHARED ${SOURCES})

set(VERSION_STRING "${STROMX_CVML_VERSION_MAJOR}.${STROMX_CVML_VERSION_MINOR}.${STROMX_CVML_VERSION_PATCH}")

set_target_properties(stromx_cvml PROPERTIES
    VERSION ${VERSION_STRING}
    SOVERSION ${VERSION_STRING}
    FOLDER "library"
)

target_link_libraries(stromx_cvml
    ${OpenCV_LIBS}
    stromx_runtime
)

if(WIN32)    
    install (TARGETS stromx_cvml
        RUNTIME DESTINATION .
        LIBRARY DESTINATION ${LIB_DIR}
        ARCHIVE DESTINATION ${LIB_DIR}
    )

else(WIN32)
    install (TARGETS stromx_cvml
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION ${LIB_DIR}
        ARCHIVE DESTINATION ${LIB_DIR}
    )
endif(WIN32)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/stromx/cvml
    DESTINATION include/stromx
    FILES_MATCHING PATTERN "*.h"
    PATTERN "impl" EXCLUDE
    PATTERN "test" EXCLUDE
)

install(FILES ${CMAKE_BINARY_DIR}/stromx/cvml/Config.h
    DESTINATION include/stromx/cvml
)
