project(stream)

set (STREAM_VERSION_MAJOR 0)
set (STREAM_VERSION_MINOR 1)
set (PACKAGE_NAME "\"Base\"")

configure_file (
  "${PROJECT_SOURCE_DIR}/Config.h.in"
  "${PROJECT_BINARY_DIR}/Config.h"
)

if(BUILD_TESTS)
    add_subdirectory(test)
endif()

set(SOURCES
    DataContainer.cpp
    DataContainerImpl.cpp
    ReadAccess.cpp
    ReadAccessImpl.cpp
    RecycleAccess.cpp
    RecycleAccessImpl.cpp
    WriteAccess.cpp
    WriteAccessImpl.cpp
    DataType.cpp
    EnumParameter.cpp
    Id2DataComposite.cpp
    Id2DataMap.cpp
    Id2DataMapper.cpp
    Id2DataPair.cpp
    ImageWrapper.cpp
    InputNode.cpp
    Network.cpp
    Operator.cpp
    OperatorFactory.cpp
    OperatorNode.cpp
    OperatorWrapper.cpp
    OutputNode.cpp
    Primitive.cpp
    Try.cpp
)

add_library(stream SHARED ${SOURCES})

set(VERSION_STRING "${STREAM_VERSION_MAJOR}.${STREAM_VERSION_MINOR}")

set_target_properties(stream PROPERTIES
    VERSION ${VERSION_STRING}
    SOVERSION ${VERSION_STRING}
)

include_directories(
    ${Boost_INCLUDE_DIRS}
    ${PROJECT_BINARY_DIR}
)

target_link_libraries(stream
    ${Boost_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
)