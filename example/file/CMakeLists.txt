project(file)

include_directories(../..)

add_executable(file
    file.cpp
)

target_link_libraries(file
    stromx_runtime
    stromx_example
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/file.xml DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
