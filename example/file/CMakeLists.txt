include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
)

add_executable(file
    file.cpp
)

set_target_properties(file PROPERTIES FOLDER "example")

target_link_libraries(file
    stromx_runtime
)

add_test(NAME example_file COMMAND file)
set_tests_properties(example_file PROPERTIES ENVIRONMENT "${EXAMPLE_ENVIRONMENT}")

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/file.xml ${CMAKE_CURRENT_BINARY_DIR}/file.xml COPYONLY)
