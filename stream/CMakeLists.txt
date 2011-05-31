project(libstream)

if(BUILD_TESTS)
    add_subdirectory(test)
endif()

set(SOURCES   
    DataContainer.cpp
    Id2DataComposite.cpp
    Id2DataMap.cpp
    Id2DataMapper.cpp
    Id2DataPair.cpp
    InputNode.cpp
    Network.cpp
    None.cpp
    Operator.cpp
    OperatorNode.cpp
    OperatorWrapper.cpp
    OutputNode.cpp
)

include_directories(${Boost_INCLUDE_DIRS})
add_library(stream SHARED ${SOURCES})
target_link_libraries(stream ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})