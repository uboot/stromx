if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4172")
endif(MSVC)

# setup the LD_LIBRARY_PATH for unit tests on launchpad 
set(LD_LIBRARY_PATH "${CMAKE_BINARY_DIR}/stromx/runtime:${CMAKE_BINARY_DIR}/stromx/cvsupport:${CMAKE_BINARY_DIR}/stromx/cvcore:${CMAKE_BINARY_DIR}/stromx/cvcalib3d:${CMAKE_BINARY_DIR}/stromx/cvimgproc:${CMAKE_BINARY_DIR}/stromx/cvhighgui:${CMAKE_BINARY_DIR}/stromx/test")
set(PYTHONPATH "${CMAKE_BINARY_DIR}/python")
set(PYTHON_TEST_ENVIRONMENT "PYTHONPATH=${PYTHONPATH};LD_LIBRARY_PATH=${LD_LIBRARY_PATH}")

add_subdirectory(stromx)
add_subdirectory(example)
add_subdirectory(test)