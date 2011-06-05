project(libstream)

if(BUILD_TESTS)
    add_subdirectory(test)
endif()

set(SOURCES   
    DataContainer.cpp
    DataType.cpp
    Id2DataComposite.cpp
    Id2DataMap.cpp
    Id2DataMapper.cpp
    Id2DataPair.cpp
    Image.cpp
    InputNode.cpp
    Network.cpp
    None.cpp
    Operator.cpp
    OperatorNode.cpp
    OperatorWrapper.cpp
    OutputNode.cpp
    Primitive.cpp
)

if(BUILD_OPERATOR)
set(SOURCES
    ${SOURCES}
    op/Clip.cpp)
endif()

add_library(stream SHARED ${SOURCES})

if(BUILD_OPERATOR)
    include_directories(${OpenCV_ROOT_DIR})
    target_link_libraries(stream ${OpenCV2_CORE_LIBRARY})
endif()

include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(stream ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})