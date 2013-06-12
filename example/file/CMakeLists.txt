include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
)

add_executable(file
    file.cpp
)

set_target_properties(file PROPERTIES FOLDER "example")

add_dependencies(file stromx_runtime)

target_link_libraries(file
    stromx_runtime
)

add_test(NAME example_file COMMAND file)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/file.xml DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
