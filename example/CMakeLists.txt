project(example)

include_directories(
    ..
)

set(SOURCES
    simple_stream.cpp
)

add_executable(simple_stream
    simple_stream.cpp
)

target_link_libraries(simple_stream
    stream
    stream_base
)

set_target_properties(stream_base PROPERTIES
    VERSION ${VERSION_STRING}
    SOVERSION ${VERSION_STRING}
)
