configure_file (
  ${CMAKE_CURRENT_SOURCE_DIR}/Config.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/Config.h
)

if(BUILD_TESTS)
    add_subdirectory(test)
endif()
   
set(SOURCES
    impl/Client.cpp
    impl/ConnectorParameter.cpp
    impl/DataContainerImpl.cpp
    impl/ReadAccessImpl.cpp
    impl/RecycleAccessImpl.cpp
    impl/WriteAccessImpl.cpp
    impl/Id2DataMap.cpp
    impl/InputNode.cpp
    impl/SerializationHeader.cpp
    impl/SynchronizedOperatorKernel.cpp
    impl/OutputNode.cpp
    impl/Server.cpp
    impl/ThreadImpl.cpp
    impl/Network.cpp
    AssignThreadsAlgorithm.cpp
    Block.cpp
    Color.cpp
    Compare.cpp
    Connector.cpp
    ConstData.cpp
    ConstDataRef.cpp
    Counter.cpp
    DataContainer.cpp
    Data.cpp
    DataOperatorBase.cpp
    DataRef.cpp
    Description.cpp
    Dump.cpp
    Enum.cpp
    EnumParameter.cpp
    Exception.cpp
    Factory.cpp
    File.cpp
    Filter.cpp
    Fork.cpp
    Id2DataComposite.cpp
    Id2DataPair.cpp
    Image.cpp
    ImageWrapper.cpp
    IsEmpty.cpp
    IsNotEmpty.cpp
    Split.cpp
    Join.cpp
    List.cpp
    Locale.cpp
    Matrix.cpp
    MatrixPropertyBase.cpp
    MatrixParameter.cpp
    MatrixWrapper.cpp
    Merge.cpp
    None.cpp
    Operator.cpp
    OperatorKernel.cpp
    PeriodicDelay.cpp
    Parameter.cpp
    ParameterGroup.cpp
    Push.cpp
    Queue.cpp
    Primitive.cpp
    ReadAccess.cpp
    RecycleAccess.cpp
    Repeat.cpp
    Send.cpp
    Stream.cpp
    String.cpp
    SortInputsAlgorithm.cpp
    Receive.cpp
    Repeat.cpp
    Runtime.cpp
    Thread.cpp
    Tribool.cpp
    TriggerData.cpp
    Try.cpp
    Variant.cpp
    VariantComposite.cpp
    VariantHandle.cpp
    VariantInterface.cpp
    Version.cpp
    Visualization.cpp
    WriteAccess.cpp
)

if(BUILD_FILE_PERSISTENCE)
    set(SOURCES
        ${SOURCES}
        DirectoryFileInput.cpp
        DirectoryFileOutput.cpp
        XmlReader.cpp
        XmlWriter.cpp
        ZipFileInput.cpp
        ZipFileOutput.cpp
        impl/XmlReaderImpl.cpp
        impl/XmlUtilities.cpp
        impl/XmlWriterImpl.cpp
       )
endif(BUILD_FILE_PERSISTENCE)

add_library(stromx_runtime SHARED ${SOURCES})

set(VERSION_STRING "${STROMX_VERSION_MAJOR}.${STROMX_VERSION_MINOR}.${STROMX_VERSION_PATCH}")

set_target_properties(stromx_runtime PROPERTIES
    VERSION ${VERSION_STRING}
    SOVERSION ${VERSION_STRING}
    FOLDER "library"
)

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${Boost_INCLUDE_DIRS}
)

if(BUILD_FILE_PERSISTENCE)
    include_directories(${XERCES_INCLUDE_DIR})
    include_directories(${LIBZIP_INCLUDE_DIR})
endif()

target_link_libraries(stromx_runtime
    ${Boost_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
)

if(BUILD_FILE_PERSISTENCE)
    target_link_libraries(stromx_runtime ${XERCES_LIBRARIES})
    target_link_libraries(stromx_runtime ${LIBZIP_LIBRARY})
endif()

# Compiles the the *.po file and creates an install target for the *.mo file: 
# if(GETTEXT_FOUND)
#     gettext_process_po_files(de ALL INSTALL_DESTINATION ${LOCALE_SUFFIX} PO_FILES po/de/libruntime.po)
# endif()

if(WIN32)
    install (TARGETS stromx_runtime 
        RUNTIME DESTINATION .
        LIBRARY DESTINATION ${LIB_DIR}
        ARCHIVE DESTINATION ${LIB_DIR}
    )
else(WIN32)
    install (TARGETS stromx_runtime 
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION ${LIB_DIR}
        ARCHIVE DESTINATION ${LIB_DIR}
    )
endif(WIN32)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/stromx/runtime
    DESTINATION include/stromx
    FILES_MATCHING PATTERN "*.h"
    PATTERN "impl" EXCLUDE
    PATTERN "test" EXCLUDE
)

install(FILES ${CMAKE_BINARY_DIR}/stromx/runtime/Config.h
    DESTINATION include/stromx/runtime
)

