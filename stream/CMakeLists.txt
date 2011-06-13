project(stream)

set (STREAM_VERSION_MAJOR 0)
set (STREAM_VERSION_MINOR 1)

configure_file (
  "${PROJECT_SOURCE_DIR}/Config.h.in"
  "${PROJECT_BINARY_DIR}/Config.h"
)

if(BUILD_TESTS)
    add_subdirectory(test)
endif()

set(SOURCES   
    DataContainer.cpp
    DataType.cpp
    Enum.cpp
    EnumParameter.cpp
    Id2DataComposite.cpp
    Id2DataMap.cpp
    Id2DataMapper.cpp
    Id2DataPair.cpp
    ImageWrapper.cpp
    InputNode.cpp
    Network.cpp
    None.cpp
    Operator.cpp
    OperatorNode.cpp
    OperatorWrapper.cpp
    OutputNode.cpp
    Primitive.cpp
)

add_library(stream SHARED ${SOURCES})

include_directories(
    ${Boost_INCLUDE_DIRS}
    ${PROJECT_BINARY_DIR}
)

target_link_libraries(stream
    ${Boost_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
)