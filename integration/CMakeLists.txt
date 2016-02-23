add_test(NAME integration_test COMMAND integration_test)
set(INTEGRATION_TEST_ENVIRONMENT "LD_LIBRARY_PATH=${LD_LIBRARY_PATH}")
set_tests_properties(integration_test PROPERTIES ENVIRONMENT "${INTEGRATION_TEST_ENVIRONMENT}")

include_directories (
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${CPPUNIT_INCLUDE_DIR}
    ${Boost_INCLUDE_DIRS}
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/calibration.stromx ${CMAKE_CURRENT_BINARY_DIR}/calibration.stromx COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/camera.stromx ${CMAKE_CURRENT_BINARY_DIR}/camera.stromx COPYONLY)

set (SOURCES 
    IntegrationTest.cpp
    main.cpp
)

add_executable(integration_test ${SOURCES})

set_target_properties(integration_test PROPERTIES FOLDER "test")

target_link_libraries (integration_test
    ${CPPUNIT_LIBRARY}
    ${CMAKE_DL_LIBS}
    stromx_runtime
    stromx_cvcalib3d
    stromx_cvsupport
)

