project(stream_test)

include_directories(
    ../..
    ${CPPUNIT_INCLUDE_DIR}
    ${PROJECT_BINARY_DIR}/..
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/network.xml DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

set(SOURCES
    main.cpp
    ../impl/DataContainerImpl.cpp
    ../DataContainer.cpp
    DataContainerTest.cpp
    ../DataType.cpp
    DataTypeTest.cpp
    ../impl/Id2DataMap.cpp
    ../Id2DataMapper.cpp
    ../Id2DataComposite.cpp
    Id2DataCompositeTest.cpp
    ../Id2DataPair.cpp
    Id2DataPairTest.cpp
    ../OperatorKernel.cpp
    TestOperator.cpp
    ../SynchronizedOperatorKernel.cpp
    SynchronizedOperatorKernelTest.cpp
    ../OutputNode.cpp
    OutputNodeTest.cpp
    ../InputNode.cpp
    InputNodeTest.cpp   
    ../Operator.cpp
    ../ReadAccess.cpp
    ../impl/ReadAccessImpl.cpp
    ReadAccessTest.cpp
    ../RecycleAccess.cpp
    ../impl/RecycleAccessImpl.cpp
    RecycleAccessTest.cpp
    ../Thread.cpp
    ThreadTest.cpp
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
    ../impl/WriteAccessImpl.cpp
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
        #XmlWriterTest.cpp
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

