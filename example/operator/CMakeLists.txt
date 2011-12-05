project(operator)

include_directories(../..)

add_executable(operator
    operator.cpp
    math/Add.cpp
)

target_link_libraries(operator
    stromx_core
    stromx_base
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/operator.xml DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
