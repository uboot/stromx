project(stromx_core_test)
    
if(MSVC)
    add_definitions(/DSTROMX_CORE_STATIC)
endif(MSVC)

include_directories(
    ${Boost_INCLUDE_DIRS}
    ${CPPUNIT_INCLUDE_DIR}
)

if(BUILD_XML_SUPPORT)
    include_directories(${XERCES_INCLUDE_DIR})
endif()

if(BUILD_ZIP_SUPPORT)
    include_directories(${LIBZIP_INCLUDE_DIR})
endif()


file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/invalid.xml DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/parameters.xml DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/stream.xml DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

set(SOURCES
    main.cpp
    ../Data.cpp
    DataTest.cpp
    ../impl/DataContainerImpl.cpp
    ../DataContainer.cpp
    DataContainerTest.cpp
    ../DataVariant.cpp
    DataVariantTest.cpp
    ../DirectoryFileInput.cpp
    DirectoryFileInputTest.cpp
    ../DirectoryFileOutput.cpp
    DirectoryFileOutputTest.cpp
    ../impl/Id2DataMap.cpp
    ../Id2DataComposite.cpp
    ../None.cpp
    ../Dump.cpp
    DumpTest.cpp
    Id2DataCompositeTest.cpp
    Id2DataMapTest.cpp
    ../Id2DataPair.cpp
    Id2DataPairTest.cpp
    ../OperatorKernel.cpp
    TestOperator.cpp
    ../impl/SynchronizedOperatorKernel.cpp
    ../Operator.cpp
    ../Parameter.cpp
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
    ../SortInputsAlgorithm.cpp
    SortInputsAlgorithmTest.cpp
    ../Stream.cpp
    StreamTest.cpp
    TestUtilities.cpp
    ../Core.cpp
    ../Trigger.cpp
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
endif()

if(BUILD_ZIP_SUPPORT)
    set(SOURCES
        ${SOURCES}
        ../ZipFileInput.cpp
        ZipFileInputTest.cpp
        ../ZipFileOutput.cpp
        ZipFileOutputTest.cpp
    )
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

if(BUILD_XML_SUPPORT)
    target_link_libraries(stromx_core_test ${LIBZIP_LIBRARY})
endif()

