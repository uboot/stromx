add_test(NAME stromx_raspi_test COMMAND stromx_raspi_test)

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${CPPUNIT_INCLUDE_DIR}
    ${Boost_INCLUDE_DIRS}
)

add_executable(stromx_raspi_test
    ../GpioTrigger.cpp
    ../ReadGpio.cpp
    ../RaspiCam.cpp
    ../RaspiStillCam.cpp
    ../WriteGpio.cpp
    ../impl/Gpio.cpp
    ../impl/Utilities.cpp
    GpioTriggerTest.cpp
    main.cpp
    RaspiCamTest.cpp
    RaspiStillCamTest.cpp
    ReadGpioTest.cpp
    WriteGpioTest.cpp
)

set_target_properties(stromx_raspi_test PROPERTIES
    FOLDER "test"
)

target_link_libraries(stromx_raspi_test 
    ${CPPUNIT_LIBRARY}
    ${CMAKE_DL_LIBS}
    stromx_runtime
    stromx_cvsupport
    ${OpenCV_LIBS}
    vcos
    mmal_core
    mmal_util
    mmal_vc_client
    bcm_host
)
