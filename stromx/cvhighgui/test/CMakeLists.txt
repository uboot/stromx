add_test(NAME stromx_cvhighgui_test COMMAND stromx_cvhighgui_test)

if(MSVC)
    add_definitions(/DSTROMX_CVIMGPROC_STATIC)
endif(MSVC)

include_directories (
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${CPPUNIT_INCLUDE_DIR}
)

set (SOURCES 
    ../VideoCapture.cpp
    VideoCaptureTest.cpp
    main.cpp
)

add_executable(stromx_cvhighgui_test ${SOURCES})

set_target_properties(stromx_cvhighgui_test PROPERTIES FOLDER "test")

add_dependencies(stromx_cvhighgui_test stromx_cvhighgui)

target_link_libraries (stromx_cvhighgui_test
    ${CPPUNIT_LIBRARY}
    ${CMAKE_DL_LIBS}
    ${OpenCV_LIBS}
    stromx_runtime
    stromx_cvsupport
)

