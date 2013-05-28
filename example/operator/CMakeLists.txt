project(operator)

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
)

add_executable(operator
    operator.cpp
    math/Add.cpp
)

target_link_libraries(operator
    stromx_runtime
    stromx_example
)

add_test(NAME example_operator COMMAND operator)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/operator.xml DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
