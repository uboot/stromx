set(SOURCES
    Runtime.cpp
    AbstractFactory.cpp
    Color.cpp
    Data.cpp
    DataContainer.cpp
    Description.cpp
    Exception.cpp
    ExceptionObserver.cpp
    Factory.cpp
    File.cpp
    Image.cpp
    InputProvider.cpp
    Connector.cpp
    ConnectorObserver.cpp
    List.cpp
    Matrix.cpp
    None.cpp
    Operator.cpp
    OperatorInfo.cpp
    OperatorKernel.cpp
    OutputProvider.cpp
    Position.cpp
    Primitive.cpp
    Parameter.cpp
    ReadAccess.cpp
    Registry.cpp
    Stream.cpp
    String.cpp
    SortInputsAlgorithm.cpp
    Thread.cpp
    TriggerData.cpp
    Variant.cpp
    VariantHandle.cpp
    VariantInterface.cpp
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

add_library(runtime SHARED ${SOURCES})

set_target_properties(runtime PROPERTIES FOLDER "python")

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${Boost_INCLUDE_DIRS}
    ${PYTHON_INCLUDE_DIRS}
)

target_link_libraries(runtime
    ${Boost_LIBRARIES}
    ${PYTHON_LIBRARY}
    stromx_runtime
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/__init__.py ${CMAKE_CURRENT_BINARY_DIR}/__init__.py COPYONLY)

if(PYTHON_INSTALL_DIR)
    install(DIRECTORY . 
        DESTINATION ${PYTHON_INSTALL_DIR}/stromx/runtime
        FILES_MATCHING PATTERN "*.py"
    )
    install(TARGETS runtime 
        DESTINATION ${PYTHON_INSTALL_DIR}/stromx/runtime
    )
endif()


