set(LD_LIBRARY_PATH "${CMAKE_BINARY_DIR}/stromx/runtime:${CMAKE_BINARY_DIR}/stromx/cvsupport:${CMAKE_BINARY_DIR}")
set(TEST_ENVIRONMENT "LD_LIBRARY_PATH=${LD_LIBRARY_PATH}")

add_subdirectory(runtime)

if(BUILD_OPENCV_WRAPPER)
    add_subdirectory(cvcalib3d)
    add_subdirectory(cvcore)
    add_subdirectory(cvsupport)
    add_subdirectory(cvhighgui)
    add_subdirectory(cvimgproc)
    add_subdirectory(cvobjdetect)
    add_subdirectory(cvml)
    add_subdirectory(test)
endif()

if(BUILD_RASPI)
    add_subdirectory(raspi)
endif()

