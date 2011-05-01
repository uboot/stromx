project(libstream)

set(SOURCES
    DataContainer.cpp
    Network.cpp
    None.cpp
    Operator.cpp
)

add_library(stream SHARED ${SOURCES})