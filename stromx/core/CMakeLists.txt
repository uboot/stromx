project(stromx_core)

set (STROMX_VERSION_MAJOR 0)
set (STROMX_VERSION_MINOR 1)
set (STROMX_VERSION_PATCH 0)

configure_file (
  "${PROJECT_SOURCE_DIR}/Config.h.in"
  "${PROJECT_SOURCE_DIR}/Config.h"
)

if(BUILD_TESTS)
    add_subdirectory(test)
endif()
   
set(SOURCES
    impl/DataContainerImpl.cpp
    impl/ReadAccessImpl.cpp
    impl/RecycleAccessImpl.cpp
    impl/WriteAccessImpl.cpp
    impl/Id2DataMap.cpp
    impl/InputNode.cpp
    impl/SynchronizedOperatorKernel.cpp
    impl/OutputNode.cpp
    impl/ThreadImpl.cpp
    impl/Network.cpp
    DataContainer.cpp
    RecycleAccess.cpp
    Data.cpp
    DataVariant.cpp
    Dump.cpp
    Enum.cpp
    EnumParameter.cpp
    Exception.cpp
    Id2DataComposite.cpp
    Id2DataPair.cpp
    ImageWrapper.cpp
    None.cpp
    Operator.cpp
    OperatorKernel.cpp
    Parameter.cpp
    Factory.cpp
    Primitive.cpp
    Stream.cpp
    SortInputsAlgorithm.cpp
    Core.cpp
    Thread.cpp
    Trigger.cpp
    Try.cpp
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

add_library(stromx_core SHARED ${SOURCES})

set(VERSION_STRING "${STROMX_VERSION_MAJOR}.${STROMX_VERSION_MINOR}.${STROMX_VERSION_PATCH}")

set_target_properties(stromx_core PROPERTIES
    VERSION ${VERSION_STRING}
    SOVERSION ${VERSION_STRING}
)

include_directories(
    ${Boost_INCLUDE_DIRS}
)

if(BUILD_FILE_PERSISTENCE)
    include_directories(${XERCES_INCLUDE_DIR})
    include_directories(${LIBZIP_INCLUDE_DIR})
endif()

target_link_libraries(stromx_core
    ${Boost_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
)

if(BUILD_FILE_PERSISTENCE)
    target_link_libraries(stromx_core ${XERCES_LIBRARIES})
    target_link_libraries(stromx_core ${LIBZIP_LIBRARY})
endif()

install (TARGETS stromx_core 
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(DIRECTORY . 
    DESTINATION include/stromx/core
    FILES_MATCHING PATTERN "*.h"
    PATTERN "test*" EXCLUDE
)

