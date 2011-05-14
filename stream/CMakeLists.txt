project(libstream)

set(SOURCES   
    DataContainer.cpp
    Id2DataComposite.cpp
    Id2DataMap.cpp
    Id2DataMapper.cpp
    Id2DataPair.cpp
    Network.cpp
    None.cpp
    Operator.cpp
    OperatorWrapper.cpp
)

add_library(stream SHARED ${SOURCES})
target_link_libraries(stream boost_thread)