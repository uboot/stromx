add_test(NAME stromx_cvcalib3d_test COMMAND stromx_cvcalib3d_test)

if(MSVC)
    add_definitions(/DSTROMX_CVCALIB_3D_STATIC)
endif(MSVC)

include_directories (
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${CPPUNIT_INCLUDE_DIR}
    ${Boost_INCLUDE_DIRS}
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/chess.png ${CMAKE_CURRENT_BINARY_DIR}/chess.png COPYONLY)

set (SOURCES 
    ../FindChessboardCorners.cpp
    ../Utility.cpp
    ../Cvcalib3d.cpp
    FindChessboardCornersTest.cpp
    main.cpp
)

add_executable(stromx_cvcalib3d_test ${SOURCES})

set_target_properties(stromx_cvcalib3d_test PROPERTIES FOLDER "test")

target_link_libraries (stromx_cvcalib3d_test
    ${CPPUNIT_LIBRARY}
    ${CMAKE_DL_LIBS}
    ${OpenCV_LIBS}
    stromx_runtime
    stromx_cvsupport
)

