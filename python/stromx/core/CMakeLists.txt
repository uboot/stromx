project(python_core)

set(SOURCES
    Core.cpp
    Data.cpp
    DataContainer.cpp
    DataVariant.cpp
    Description.cpp
    Exception.cpp
    ExceptionObserver.cpp
    Factory.cpp
    Image.cpp
    Connector.cpp
    ConnectorObserver.cpp
    None.cpp
    Operator.cpp
    OperatorInfo.cpp
    OperatorKernel.cpp
    Primitive.cpp
    Parameter.cpp
    ReadAccess.cpp
    Registry.cpp
    Stream.cpp
    Thread.cpp
    Version.cpp
    WriteAccess.cpp
    XmlReader.cpp
    XmlWriter.cpp
)

add_library(core SHARED ${SOURCES})

include_directories(
    ../../..
    ${Boost_INCLUDE_DIRS}
    ${PYTHON_INCLUDE_DIRS}
)

target_link_libraries(core
    ${Boost_LIBRARIES}
    ${PYTHON_LIBRARY}
    stromx_core
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/__init__.py DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

find_file(python_core_EXISTS ${CMAKE_CURRENT_BINARY_DIR}/libcore.so)

if(python_core_EXISTS)
    file(COPY ${CMAKE_CURRENT_BINARY_DIR}/libcore.so DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif()
