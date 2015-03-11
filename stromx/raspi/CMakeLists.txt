set (RASPI_VERSION_MAJOR 0)
set (RASPI_VERSION_MINOR 0)
set (RASPI_VERSION_PATCH 1)

configure_file (
    ${CMAKE_CURRENT_SOURCE_DIR}/Config.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/Config.h
)

include_directories (
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    /opt/vc/include
    /opt/vc/include/interface/vcos/pthreads
    /opt/vc/include/interface/vmcs_host/linux
)

set (SOURCES
    GpioTrigger.cpp
    Raspi.cpp
    RaspiCam.cpp
    RaspiStillCam.cpp
    ReadGpio.cpp
    WriteGpio.cpp
    impl/Gpio.cpp
    impl/Utilities.cpp
)
    
add_library (stromx_raspi SHARED ${SOURCES})

set(VERSION_STRING "${RASPI_VERSION_MAJOR}.${RASPI_VERSION_MINOR}.${RASPI_VERSION_PATCH}")

set_target_properties (stromx_raspi PROPERTIES
    VERSION ${VERSION_STRING}
    SOVERSION ${VERSION_STRING}
    FOLDER "library"
)

link_directories(/opt/vc/lib)

target_link_libraries (stromx_raspi
    stromx_runtime
    stromx_cvsupport
)

if(WIN32)    
    install (TARGETS stromx_raspi
        RUNTIME DESTINATION .
        LIBRARY DESTINATION ${LIB_DIR}
        ARCHIVE DESTINATION ${LIB_DIR}
    )

else(WIN32)
    install (TARGETS stromx_raspi
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION ${LIB_DIR}
        ARCHIVE DESTINATION ${LIB_DIR}
    )
endif(WIN32)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/stromx/raspi
    DESTINATION include/stromx
    FILES_MATCHING PATTERN "*.h"
    PATTERN "test*" EXCLUDE
)

install(DIRECTORY ${CMAKE_BINARY_DIR}/stromx/raspi
    DESTINATION include/stromx
    FILES_MATCHING PATTERN "*.h"
)

if(BUILD_TESTS)
    add_subdirectory(test)
endif(BUILD_TESTS)
