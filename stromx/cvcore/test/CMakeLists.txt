add_test(NAME stromx_cvcore_test COMMAND stromx_cvcore_test)
set_tests_properties(stromx_cvcore_test PROPERTIES ENVIRONMENT "${TEST_ENVIRONMENT}")

if(MSVC)
    add_definitions(/DSTROMX_CVCORE_STATIC)
endif(MSVC)

include_directories (
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${CPPUNIT_INCLUDE_DIR}
    ${Boost_INCLUDE_DIRS}
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/barbara.jpg ${CMAKE_CURRENT_BINARY_DIR}/barbara.jpg COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/lenna.jpg ${CMAKE_CURRENT_BINARY_DIR}/lenna.jpg COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/lenna_32f.npy ${CMAKE_CURRENT_BINARY_DIR}/lenna_32f.npy COPYONLY)

set (SOURCES 
    ../Absdiff.cpp
    ../Add.cpp
    ../AddWeighted.cpp
    ../Bitwise_and.cpp
    ../Bitwise_not.cpp
    ../Bitwise_or.cpp
    ../Bitwise_xor.cpp
    ../Mean.cpp
    ../Sum.cpp
    ../Utility.cpp
    ../Cvcore.cpp
    AbsdiffTest.cpp
    AddTest.cpp
    AddWeightedTest.cpp
    Bitwise_andTest.cpp
    Bitwise_notTest.cpp
    Bitwise_orTest.cpp
    Bitwise_xorTest.cpp
    MeanTest.cpp
    SumTest.cpp
    main.cpp
)

add_executable(stromx_cvcore_test ${SOURCES})

set_target_properties(stromx_cvcore_test PROPERTIES FOLDER "test")

target_link_libraries (stromx_cvcore_test
    ${CPPUNIT_LIBRARY}
    ${CMAKE_DL_LIBS}
    ${OpenCV_LIBS}
    stromx_runtime
    stromx_cvsupport
)

