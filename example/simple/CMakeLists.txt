include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
)

add_executable(simple
    simple.cpp
)

target_link_libraries(simple
    stromx_runtime
)

add_test(NAME example_simple COMMAND simple)