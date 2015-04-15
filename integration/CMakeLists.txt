add_test(NAME integration_test COMMAND integration_test)

if(MSVC)
    add_definitions(/DSTROMX_CVCALIB3D_STATIC)
endif(MSVC)

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
    ${OpenCV_LIBS}
    stromx_runtime
    stromx_cvcalib3d
    stromx_cvsupport
)

