add_test(NAME stromx_cvobjdetect_test COMMAND stromx_cvobjdetect_test)
set_tests_properties(stromx_cvobjdetect_test PROPERTIES ENVIRONMENT "${TEST_ENVIRONMENT}")

if(MSVC)
    add_definitions(/DSTROMX_CVOBJDETECT_STATIC)
endif(MSVC)

include_directories (
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${CPPUNIT_INCLUDE_DIR}
    ${Boost_INCLUDE_DIRS}
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/faces.jpg ${CMAKE_CURRENT_BINARY_DIR}/faces.jpg COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/haarcascade_frontalface_alt.txt ${CMAKE_CURRENT_BINARY_DIR}/haarcascade_frontalface_alt.txt COPYONLY)

set (SOURCES 
    ../CascadeClassifier.cpp
    ../Utility.cpp
    ../Cvobjdetect.cpp
    CascadeClassifierTest.cpp
    main.cpp
)

add_executable(stromx_cvobjdetect_test ${SOURCES})

set_target_properties(stromx_cvobjdetect_test PROPERTIES
    FOLDER "test"
)

target_link_libraries (stromx_cvobjdetect_test
    ${CPPUNIT_LIBRARY}
    ${CMAKE_DL_LIBS}
    ${OpenCV_LIBS}
    stromx_runtime
    stromx_cvsupport
)

