project(libstream)

set(SOURCES   
    DataContainer.cpp
    Id2DataComposite.cpp
    Id2DataMap.cpp
    Id2DataPair.cpp
    Network.cpp
    None.cpp
    Operator.cpp
)

add_library(stream SHARED ${SOURCES})