project(python_stromx_core)

set (SOURCES 
    Core.cpp
)

add_library (core SHARED ${SOURCES})

add_dependencies(core stromx_core)

set_target_properties (core PROPERTIES FOLDER "python")

include_directories (
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${Boost_INCLUDE_DIRS}
    ${OpenCV2_INCLUDE_DIR}
    ${PYTHON_INCLUDE_DIRS}
)

target_link_libraries (core
    ${Boost_LIBRARIES}
    ${PYTHON_LIBRARY}
    stromx_runtime
    stromx_imgutil
    stromx_core
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/__init__.py DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(PYTHON_INSTALL_DIR)
    install(DIRECTORY . 
        DESTINATION ${PYTHON_INSTALL_DIR}/stromx/core
        FILES_MATCHING PATTERN "*.py"
    )
    install(TARGETS core
        DESTINATION ${PYTHON_INSTALL_DIR}/stromx/core
    )
endif()
