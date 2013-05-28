project(file)

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
)

add_executable(file
    file.cpp
)

target_link_libraries(file
    stromx_runtime
    stromx_example
)

add_test(NAME example_file COMMAND file)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/file.xml DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
