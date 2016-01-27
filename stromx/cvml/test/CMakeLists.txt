add_test(NAME stromx_cvml_test COMMAND stromx_cvml_test)
set_tests_properties(stromx_cvml_test PROPERTIES ENVIRONMENT "${TEST_ENVIRONMENT}")

if(MSVC)
    add_definitions(/DSTROMX_CVML_STATIC)
endif(MSVC)

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${CPPUNIT_INCLUDE_DIR}
    ${Boost_INCLUDE_DIRS}
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/svm_model.xml ${CMAKE_CURRENT_BINARY_DIR}/svm_model.xml COPYONLY)

add_executable(stromx_cvml_test
    ../Svm.cpp
    SvmTest.cpp
    main.cpp
)

set_target_properties(stromx_cvml_test PROPERTIES
    FOLDER "test"
)

target_link_libraries(stromx_cvml_test 
    ${CPPUNIT_LIBRARY}
    ${CMAKE_DL_LIBS}
    ${OpenCV_LIBS}
    stromx_runtime
    stromx_cvsupport
)
