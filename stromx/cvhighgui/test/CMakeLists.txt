add_test(NAME stromx_cvhighgui_test COMMAND stromx_cvhighgui_test)
set_tests_properties(stromx_cvhighgui_test PROPERTIES ENVIRONMENT "${TEST_ENVIRONMENT}")

if(MSVC)
    add_definitions(/DSTROMX_CVHIGHGUI_STATIC)
endif(MSVC)

include_directories (
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${CPPUNIT_INCLUDE_DIR}
)

set (SOURCES 
    ../VideoCapture.cpp
#     VideoCaptureTest.cpp
    main.cpp
)

add_executable(stromx_cvhighgui_test ${SOURCES})

set_target_properties(stromx_cvhighgui_test PROPERTIES FOLDER "test")

target_link_libraries (stromx_cvhighgui_test
    ${CPPUNIT_LIBRARY}
    ${CMAKE_DL_LIBS}
    ${OpenCV_LIBS}
    stromx_runtime
    stromx_cvsupport
)

