add_test(NAME stromx_cvml_test COMMAND stromx_cvml_test)

if(MSVC)
    add_definitions(/DSTROMX_CVML_STATIC)
endif(MSVC)

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${CPPUNIT_INCLUDE_DIR}
    ${Boost_INCLUDE_DIRS}
)

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
)
