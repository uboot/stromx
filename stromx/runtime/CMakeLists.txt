set (STROMX_RUNTIME_VERSION_MAJOR ${STROMX_VERSION_MAJOR})
set (STROMX_RUNTIME_VERSION_MINOR ${STROMX_VERSION_MINOR})
set (STROMX_RUNTIME_VERSION_PATCH ${STROMX_VERSION_PATCH})

configure_file (
  ${CMAKE_CURRENT_SOURCE_DIR}/Config.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/Config.h
)

if(BUILD_TESTS)
    add_subdirectory(test)
endif()
   
set(SOURCES
    impl/Client.cpp
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
    Block.cpp
    Color.cpp
    Connector.cpp
    ConstData.cpp
    ConstDataRef.cpp
    Counter.cpp
    DataContainer.cpp
    Data.cpp
    DataRef.cpp
    Description.cpp
    Dump.cpp
    Enum.cpp
    EnumParameter.cpp
    Exception.cpp
    Factory.cpp
    File.cpp
    Fork.cpp
    Id2DataComposite.cpp
    Id2DataPair.cpp
    Image.cpp
    ImageWrapper.cpp
    Iterate.cpp
    Join.cpp
    List.cpp
    Locale.cpp
    Matrix.cpp
    MatrixWrapper.cpp
    Merge.cpp
    None.cpp
    Operator.cpp
    OperatorKernel.cpp
    PeriodicDelay.cpp
    Parameter.cpp
    ParameterGroup.cpp
    Queue.cpp
    Primitive.cpp
    RecycleAccess.cpp
    Send.cpp
    Stream.cpp
    String.cpp
    SortInputsAlgorithm.cpp
    Receive.cpp
    Runtime.cpp
    Thread.cpp
    TriggerData.cpp
    Try.cpp
    Variant.cpp
    VariantComposite.cpp
    VariantHandle.cpp
    VariantInterface.cpp
    Version.cpp
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

set(VERSION_STRING "${STROMX_RUNTIME_VERSION_MAJOR}.${STROMX_RUNTIME_VERSION_MINOR}.${STROMX_RUNTIME_VERSION_PATCH}")

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
    PATTERN "test*" EXCLUDE
)

install(DIRECTORY ${CMAKE_BINARY_DIR}/stromx/runtime
    DESTINATION include/stromx
    FILES_MATCHING PATTERN "*.h"
)

