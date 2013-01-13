project(simple)

include_directories(../..)

add_executable(simple
    simple.cpp
)

target_link_libraries(simple
    stromx_runtime
)

add_test(NAME example_simple COMMAND simple)