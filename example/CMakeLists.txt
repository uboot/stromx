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

set_target_properties(stream_base PROPERTIES
    VERSION ${VERSION_STRING}
    SOVERSION ${VERSION_STRING}
)
