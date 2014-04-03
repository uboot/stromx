add_test(NAME stromx_raspi_test COMMAND stromx_raspi_test)

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${CPPUNIT_INCLUDE_DIR}
    ${Boost_INCLUDE_DIRS}
)

add_executable(stromx_raspi_test
    ../WriteGpio.cpp
    main.cpp
    WriteGpioTest.cpp
)

set_target_properties(stromx_raspi_test PROPERTIES
    FOLDER "test"
)

target_link_libraries(stromx_raspi_test 
    ${CPPUNIT_LIBRARY}
    ${CMAKE_DL_LIBS}
    stromx_runtime
)

add_dependencies(stromx_raspi_test stromx_runtime)
