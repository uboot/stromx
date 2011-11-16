project(example)

include_directories(
    ..
    ${Boost_INCLUDE_DIRS}
)

add_executable(simple
    simple.cpp
)

target_link_libraries(simple
    stromx_core
    stromx_base
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/file.xml DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(file
    file.cpp
)

target_link_libraries(file
    stromx_core
    stromx_base
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/operator.xml DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(operator
    operator.cpp
    math/Add.cpp
)

target_link_libraries(operator
    stromx_core
    stromx_base
)