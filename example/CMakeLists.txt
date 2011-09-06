project(example)

include_directories(
    ..
)

add_executable(simple
    simple.cpp
)

target_link_libraries(simple
    stream
    stream_base
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/file.xml DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(file
    file.cpp
)

target_link_libraries(file
    stream
    stream_base
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/operator.xml DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(operator
    operator.cpp
    math/Add.cpp
)

target_link_libraries(operator
    stream
    stream_base
)