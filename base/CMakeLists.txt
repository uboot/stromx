project(base)

set(SOURCES
    Clip.cpp)

include_directories(..)
include_directories(${OpenCV_ROOT_DIR})
add_library(stream_base_operators SHARED ${SOURCES})
target_link_libraries(stream_base_operators ${OpenCV2_CORE_LIBRARY})