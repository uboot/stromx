add_test(NAME stromx_runtime_test COMMAND stromx_runtime_test)
    
if(MSVC)
    add_definitions(/DSTROMX_RUNTIME_STATIC)
endif(MSVC)

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${Boost_INCLUDE_DIRS}
    ${CPPUNIT_INCLUDE_DIR}
)

if(BUILD_FILE_PERSISTENCE)
    include_directories(${XERCES_INCLUDE_DIR})
    include_directories(${LIBZIP_INCLUDE_DIR})
endif()

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/archive.zip DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/dtd.xml DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/invalid.xml DESTINATION ${CMAKE_CURRENT_BINARY_DIR})      
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/parameters.xml DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/parameters.zip DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/parameters.stromx DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/stream.xml DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/stream.zip DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/stream.stromx DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/double_matrix.npy DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/empty_float_matrix.npy DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/fortran_order.npy DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/uint16_matrix.npy DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

set(SOURCES
    ../Client.cpp
    ../Connector.cpp
    ../ConstDataRef.cpp
    ../Runtime.cpp
    ../Counter.cpp
    ../Data.cpp
    ../DataRef.cpp
    ../DataContainer.cpp
    ../DataVariant.cpp
    ../Description.cpp
    ../Dump.cpp
    ../Enum.cpp
    ../EnumParameter.cpp
    ../Exception.cpp
    ../Factory.cpp
    ../Fork.cpp
    ../Join.cpp
    ../Id2DataComposite.cpp
    ../Id2DataPair.cpp
    ../Image.cpp
    ../ImageWrapper.cpp
    ../Matrix.cpp
    ../MatrixWrapper.cpp
    ../None.cpp
    ../Operator.cpp
    ../OperatorKernel.cpp
    ../Parameter.cpp
    ../ParameterGroup.cpp
    ../PeriodicDelay.cpp
    ../Primitive.cpp
    ../Queue.cpp
    ../RecycleAccess.cpp
    ../Thread.cpp
    ../Server.cpp
    ../Stream.cpp
    ../SortInputsAlgorithm.cpp
    ../Stream.cpp
    ../String.cpp
    ../Trigger.cpp
    ../TriggerData.cpp
    ../Try.cpp
    ../Version.cpp
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
    BoolTest.cpp
    ClientTest.cpp
    CounterTest.cpp
    DataContainerTest.cpp
    DataRefTest.cpp
    DataVariantTest.cpp
    DataTest.cpp
    DumpTest.cpp
    EnumTest.cpp
    ExceptionTest.cpp
    Float64Test.cpp
    ForkTest.cpp
    Id2DataCompositeTest.cpp
    Id2DataMapTest.cpp
    Id2DataPairTest.cpp
    ImageWrapperTest.cpp
    InputNodeTest.cpp
    Int32Test.cpp
    JoinTest.cpp
    MatrixWrapperTest.cpp
    OperatorKernelTest.cpp
    OperatorTest.cpp
    OutputNodeTest.cpp
    ParameterTest.cpp
    PeriodicDelayTest.cpp
    QueueTest.cpp
    ReadAccessTest.cpp
    RecycleAccessTest.cpp
    TestOperator.cpp
    ThreadImplTest.cpp
    ThreadTest.cpp
    NetworkTest.cpp
    FactoryTest.cpp
    RuntimeTest.cpp
    StringTest.cpp
    TestData.cpp
    TriggerTest.cpp
    TryTest.cpp
    ServerTest.cpp
    SortInputsAlgorithmTest.cpp
    StreamTest.cpp
    TestUtilities.cpp
    UInt8Test.cpp
    VersionTest.cpp
    WriteAccessTest.cpp
    main.cpp
)

if(BUILD_FILE_PERSISTENCE)
    set(SOURCES
        ${SOURCES}
        ../DirectoryFileInput.cpp
        ../DirectoryFileOutput.cpp
        ../XmlReader.cpp
        ../XmlWriter.cpp
        ../ZipFileInput.cpp
        ../ZipFileOutput.cpp
        ../impl/XmlReaderImpl.cpp
        ../impl/XmlUtilities.cpp
        ../impl/XmlWriterImpl.cpp
        DirectoryFileInputTest.cpp
        DirectoryFileOutputTest.cpp
        XmlReaderTest.cpp
        XmlUtilitiesTest.cpp
        XmlWriterTest.cpp
        ZipFileInputTest.cpp
        ZipFileOutputTest.cpp
    )
endif(BUILD_FILE_PERSISTENCE)

add_executable(stromx_runtime_test ${SOURCES})

set_target_properties(stromx_runtime_test PROPERTIES
    FOLDER "test"
)

add_dependencies(stromx_runtime_test stromx_runtime)

target_link_libraries(stromx_runtime_test
    ${Boost_LIBRARIES}
    ${CPPUNIT_LIBRARY}
    ${CMAKE_DL_LIBS}
    ${CMAKE_THREAD_LIBS_INIT}
)

if(BUILD_FILE_PERSISTENCE)
    target_link_libraries(stromx_runtime_test ${XERCES_LIBRARIES})
    target_link_libraries(stromx_runtime_test ${LIBZIP_LIBRARY})
endif()


