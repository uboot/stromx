project(stromx_core_test)
    
if(MSVC)
    add_definitions(/DSTROMX_CORE_STATIC)
endif(MSVC)

include_directories(
    ${Boost_INCLUDE_DIRS}
    ${CPPUNIT_INCLUDE_DIR}
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/stream.xml DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/invalid.xml DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

set(SOURCES
    main.cpp
    ../Data.cpp
    DataTest.cpp
    ../impl/DataContainerImpl.cpp
    ../DataContainer.cpp
    DataContainerTest.cpp
    ../DataVariant.cpp
    DataVariantTest.cpp
    ../impl/Id2DataMap.cpp
    ../Id2DataComposite.cpp
    ../None.cpp
    Id2DataCompositeTest.cpp
    Id2DataMapTest.cpp
    ../Id2DataPair.cpp
    Id2DataPairTest.cpp
    ../OperatorKernel.cpp
    TestOperator.cpp
    ../impl/SynchronizedOperatorKernel.cpp
    ../Operator.cpp
    OperatorTest.cpp
    ../impl/OutputNode.cpp
    OutputNodeTest.cpp
    ../impl/InputNode.cpp
    InputNodeTest.cpp
    ../impl/ReadAccessImpl.cpp
    ../RecycleAccess.cpp
    ReadAccessTest.cpp
    ../impl/RecycleAccessImpl.cpp
    RecycleAccessTest.cpp
    ../impl/ThreadImpl.cpp
    ThreadImplTest.cpp
    ../Thread.cpp
    ThreadTest.cpp
    ../impl/Network.cpp
    NetworkTest.cpp
    ../Factory.cpp
    FactoryTest.cpp
    ../Primitive.cpp
    PrimitiveTest.cpp
    ../Stream.cpp
    ../Enum.cpp
    TestData.cpp
    ../Try.cpp
    TryTest.cpp
    ../impl/WriteAccessImpl.cpp
    WriteAccessTest.cpp
    ../Stream.cpp
    StreamTest.cpp
    TestUtilities.cpp
    ../Core.cpp
    CoreTest.cpp
)

if(BUILD_XML_SUPPORT)
    set(SOURCES
        ${SOURCES}
        ../impl/XmlUtilities.cpp
        XmlUtilitiesTest.cpp
        ../XmlReader.cpp
        ../impl/XmlReaderImpl.cpp
        XmlReaderTest.cpp
        ../XmlWriter.cpp
        XmlWriterTest.cpp
        ../impl/XmlWriterImpl.cpp
    )

    include_directories(${XERCES_INCLUDE_DIR})
endif()

add_executable(stromx_core_test ${SOURCES})

target_link_libraries(stromx_core_test
    ${Boost_LIBRARIES}
    ${CPPUNIT_LIBRARY}
    ${CMAKE_DL_LIBS}
    ${CMAKE_THREAD_LIBS_INIT}
)

if(BUILD_XML_SUPPORT)
    target_link_libraries(stromx_core_test ${XERCES_LIBRARIES})
endif()

