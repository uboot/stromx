set (RASPI_VERSION_MAJOR 0)
set (RASPI_VERSION_MINOR 1)
set (RASPI_VERSION_PATCH 0)

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
    
link_directories(/opt/vc/lib)

add_library (stromx_raspi SHARED ${SOURCES})

set(VERSION_STRING "${RASPI_VERSION_MAJOR}.${RASPI_VERSION_MINOR}.${RASPI_VERSION_PATCH}")

set_target_properties (stromx_raspi PROPERTIES
    VERSION ${VERSION_STRING}
    SOVERSION ${VERSION_STRING}
    FOLDER "library"
)

target_link_libraries (stromx_raspi
    stromx_runtime
    stromx_cvsupport
    vcos
    mmal_core
    mmal_util
    mmal_vc_client
    bcm_host
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
    PATTERN "impl" EXCLUDE
    PATTERN "test" EXCLUDE
)

install(FILES ${CMAKE_BINARY_DIR}/stromx/raspi/Config.h
    DESTINATION include/stromx/raspi
)

if(BUILD_TESTS)
    add_subdirectory(test)
endif(BUILD_TESTS)
