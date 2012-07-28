project(python_core)

set(SOURCES
    Core.cpp
    Data.cpp
    DataContainer.cpp
    DataVariant.cpp
    Description.cpp
    Documentation.cpp
    Exception.cpp
    ExceptionObserver.cpp
    Factory.cpp
    Image.cpp
    Connector.cpp
    ConnectorObserver.cpp
    Matrix.cpp
    None.cpp
    Operator.cpp
    OperatorInfo.cpp
    OperatorKernel.cpp
    Primitive.cpp
    Parameter.cpp
    ReadAccess.cpp
    Registry.cpp
    Stream.cpp
    String.cpp
    SortInputsAlgorithm.cpp
    Thread.cpp
    Version.cpp
    WriteAccess.cpp
)

if(BUILD_FILE_PERSISTENCE)
    set(SOURCES
        ${SOURCES}
        XmlReader.cpp
        XmlWriter.cpp
    )
endif(BUILD_FILE_PERSISTENCE)

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

if(PYTHON_INSTALL_DIR)
    install(DIRECTORY . 
        DESTINATION ${PYTHON_INSTALL_DIR}/stromx/core
        FILES_MATCHING PATTERN "*.py"
    )
    install(TARGETS core 
        DESTINATION ${PYTHON_INSTALL_DIR}/stromx/core
    )
endif()


