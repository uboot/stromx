project(stromx_core)

set (STROM_VERSION_MAJOR 0)
set (STROM_VERSION_MINOR 1)

configure_file (
  "${PROJECT_SOURCE_DIR}/Config.h.in"
  "${PROJECT_BINARY_DIR}/Config.h"
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
    impl/XmlReaderImpl.cpp
    impl/XmlUtilities.cpp
    impl/XmlWriterImpl.cpp
    DataContainer.cpp
    RecycleAccess.cpp
    Data.cpp
    DataVariant.cpp
    Enum.cpp
    EnumParameter.cpp
    Id2DataComposite.cpp
    Id2DataPair.cpp
    ImageWrapper.cpp
    None.cpp
    Operator.cpp
    OperatorKernel.cpp
    Factory.cpp
    Primitive.cpp
    Stream.cpp
    Strom.cpp
    Thread.cpp
    Trigger.cpp
    Try.cpp
)

if(BUILD_XML_SUPPORT)
    set(SOURCES
        ${SOURCES}
        XmlReader.cpp
        XmlWriter.cpp
        )
endif()

add_library(stromx_core SHARED ${SOURCES})

set(VERSION_STRING "${STROM_VERSION_MAJOR}.${STROM_VERSION_MINOR}")

set_target_properties(stromx_core PROPERTIES
    VERSION ${VERSION_STRING}
    SOVERSION ${VERSION_STRING}
)

include_directories(
    ${Boost_INCLUDE_DIRS}
    ${PROJECT_BINARY_DIR}
)

if(BUILD_XML_SUPPORT)
    include_directories(${XERCES_INCLUDE_DIR})
endif()

target_link_libraries(stromx_core
    ${Boost_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
)

if(BUILD_XML_SUPPORT)
    target_link_libraries(stromx_core ${XERCES_LIBRARIES})
endif()

