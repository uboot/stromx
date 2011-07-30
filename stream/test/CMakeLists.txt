project(stream_test)

include_directories(
    ../..
    ${CPPUNIT_INCLUDE_DIR}
    ${PROJECT_BINARY_DIR}/..
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/network.xml DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

set(SOURCES
    main.cpp
    ../DataContainerImpl.cpp
    ../DataContainer.cpp
    DataContainerTest.cpp
    ../DataType.cpp
    DataTypeTest.cpp
    ../Id2DataMap.cpp
    ../Id2DataMapper.cpp
    ../Id2DataComposite.cpp
    Id2DataCompositeTest.cpp
    ../Id2DataPair.cpp
    Id2DataPairTest.cpp
    ../Operator.cpp
    TestOperator.cpp
    ../OperatorWrapper.cpp
    OperatorWrapperTest.cpp
    ../OutputNode.cpp
    OutputNodeTest.cpp
    ../InputNode.cpp
    InputNodeTest.cpp   
    ../OperatorNode.cpp
    ../ReadAccess.cpp
    ../ReadAccessImpl.cpp
    ReadAccessTest.cpp
    ../RecycleAccess.cpp
    ../RecycleAccessImpl.cpp
    RecycleAccessTest.cpp
    ../ThreadImpl.cpp
    ThreadImplTest.cpp
    ../Network.cpp
    NetworkTest.cpp
    ../OperatorFactory.cpp
    OperatorFactoryTest.cpp
    ../Primitive.cpp
    ../DataType.cpp
    PrimitiveTest.cpp
    ../Stream.cpp
    TestData.cpp
    ../Try.cpp
    TryTest.cpp
    ../WriteAccess.cpp
    ../WriteAccessImpl.cpp
    WriteAccessTest.cpp
    ../Stream.cpp
    StreamTest.cpp
)

if(BUILD_XML_SUPPORT)
    set(SOURCES
        ${SOURCES}
        ../XmlReader.cpp
        XmlReaderTest.cpp
        ../XmlWriter.cpp
        XmlWriterTest.cpp
    )
endif()

add_executable(stream_test ${SOURCES})

target_link_libraries(stream_test
    ${Boost_LIBRARIES}
    ${CPPUNIT_LIBRARY}
    ${CMAKE_DL_LIBS}
    ${CMAKE_THREAD_LIBS_INIT}
)

if(BUILD_XML_SUPPORT)
    target_link_libraries(stream_test ${XERCES_LIBRARIES})
endif()

