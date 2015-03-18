add_test(NAME stromx_cvcalib3d_test COMMAND stromx_cvcalib3d_test)

if(MSVC)
    add_definitions(/DSTROMX_CVCALIB3D_STATIC)
endif(MSVC)

include_directories (
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${CPPUNIT_INCLUDE_DIR}
    ${Boost_INCLUDE_DIRS}
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/chess.png ${CMAKE_CURRENT_BINARY_DIR}/chess.png COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/chess_corners_1.npy ${CMAKE_CURRENT_BINARY_DIR}/chess_corners_1.npy COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/chess_corners_2.npy ${CMAKE_CURRENT_BINARY_DIR}/chess_corners_2.npy COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/chess_corners_3.npy ${CMAKE_CURRENT_BINARY_DIR}/chess_corners_3.npy COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/chess_corners_4.npy ${CMAKE_CURRENT_BINARY_DIR}/chess_corners_4.npy COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/chess_corners_5.npy ${CMAKE_CURRENT_BINARY_DIR}/chess_corners_5.npy COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/chess_corners_6.npy ${CMAKE_CURRENT_BINARY_DIR}/chess_corners_6.npy COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/chess_corners_7.npy ${CMAKE_CURRENT_BINARY_DIR}/chess_corners_7.npy COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/chess_corners_3d.npy ${CMAKE_CURRENT_BINARY_DIR}/chess_corners_3d.npy COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/camera_matrix_64f.npy ${CMAKE_CURRENT_BINARY_DIR}/camera_matrix_64f.npy COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/dist_coeffs_64f.npy ${CMAKE_CURRENT_BINARY_DIR}/dist_coeffs_64f.npy COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/camera_matrix_32f.npy ${CMAKE_CURRENT_BINARY_DIR}/camera_matrix_32f.npy COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/dist_coeffs_32f.npy ${CMAKE_CURRENT_BINARY_DIR}/dist_coeffs_32f.npy COPYONLY)

set (SOURCES 
    ../CalibrateCamera.cpp
    ../FindChessboardCorners.cpp
    ../GeneratePattern.cpp
    ../SolvePnP.cpp
    ../Utility.cpp
    ../Cvcalib3d.cpp
    CalibrateCameraTest.cpp
    FindChessboardCornersTest.cpp
    GeneratePatternTest.cpp
    SolvePnPTest.cpp
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

