project(python_stromx_example)

set(SOURCES
    Example.cpp
    Image.cpp
)

add_library(example SHARED ${SOURCES})

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${Boost_INCLUDE_DIRS}
    ${OpenCV2_INCLUDE_DIR}
    ${PYTHON_INCLUDE_DIRS}
)

target_link_libraries(example
    ${Boost_LIBRARIES}
    ${PYTHON_LIBRARY}
    stromx_runtime
    stromx_example
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/__init__.py DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(PYTHON_INSTALL_DIR)
    install(DIRECTORY . 
        DESTINATION ${PYTHON_INSTALL_DIR}/stromx/example
        FILES_MATCHING PATTERN "*.py"
    )
    install(TARGETS example 
        DESTINATION ${PYTHON_INSTALL_DIR}/stromx/example
    )
endif()
