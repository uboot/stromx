project(libstream)

set(SOURCES
    Network.cpp
    DataContainer.cpp)

add_library(stream SHARED ${SOURCES})