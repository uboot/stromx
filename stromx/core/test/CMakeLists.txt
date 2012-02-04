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


file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/archive.zip DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/invalid.xml DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/parameters.xml DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/stream.xml DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

set(SOURCES
    ../Core.cpp
    ../Data.cpp
    ../DataContainer.cpp
    ../DataVariant.cpp
    ../DirectoryFileInput.cpp
    ../DirectoryFileOutput.cpp
    ../Dump.cpp
    ../Enum.cpp
    ../Factory.cpp
    ../Id2DataComposite.cpp
    ../Id2DataPair.cpp
    ../None.cpp
    ../Operator.cpp
    ../OperatorKernel.cpp
    ../Parameter.cpp
    ../Primitive.cpp
    ../RecycleAccess.cpp
    ../Thread.cpp
    ../Stream.cpp
    ../SortInputsAlgorithm.cpp
    ../Stream.cpp
    ../Trigger.cpp
    ../Try.cpp
    ../impl/DataContainerImpl.cpp
    ../impl/Id2DataMap.cpp
    ../impl/InputNode.cpp
    ../impl/Network.cpp
    ../impl/OutputNode.cpp
    ../impl/ReadAccessImpl.cpp
    ../impl/RecycleAccessImpl.cpp
    ../impl/SynchronizedOperatorKernel.cpp
    ../impl/ThreadImpl.cpp
    ../impl/WriteAccessImpl.cpp
#     CoreTest.cpp
#     DataContainerTest.cpp
#     DataVariantTest.cpp
#     DataTest.cpp
#     DirectoryFileInputTest.cpp
#     DirectoryFileOutputTest.cpp
#     DumpTest.cpp
#     Id2DataCompositeTest.cpp
#     Id2DataMapTest.cpp
#     Id2DataPairTest.cpp
    TestOperator.cpp
#     OperatorTest.cpp
#     OutputNodeTest.cpp
#     InputNodeTest.cpp
#     ReadAccessTest.cpp
#     RecycleAccessTest.cpp
#     ThreadImplTest.cpp
#     ThreadTest.cpp
#     NetworkTest.cpp
#     FactoryTest.cpp
#     PrimitiveTest.cpp
    TestData.cpp
#     TryTest.cpp
#     WriteAccessTest.cpp
#     SortInputsAlgorithmTest.cpp
#     StreamTest.cpp
    TestUtilities.cpp
    main.cpp
)

if(BUILD_XML_SUPPORT)
    set(SOURCES
        ${SOURCES}
        ../XmlReader.cpp
        ../XmlWriter.cpp
        ../impl/XmlReaderImpl.cpp
        ../impl/XmlUtilities.cpp
        ../impl/XmlWriterImpl.cpp
        XmlReaderTest.cpp
        XmlUtilitiesTest.cpp
        XmlWriterTest.cpp
    )
endif()

if(BUILD_ZIP_SUPPORT)
    set(SOURCES
        ${SOURCES}
        ../ZipFileInput.cpp
        ../ZipFileOutput.cpp
        ZipFileInputTest.cpp
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

