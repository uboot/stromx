if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4172")
endif(MSVC)

set(TEST_PYTHONPATH "${CMAKE_BINARY_DIR}/python")
set(TEST_LD_LIBRARY_PATH "\
${CMAKE_BINARY_DIR}/stromx/runtime:\
${CMAKE_BINARY_DIR}/stromx/example:\
${CMAKE_BINARY_DIR}/stromx/cvsupport:\
${CMAKE_BINARY_DIR}/stromx/cvcore:\
${CMAKE_BINARY_DIR}/stromx/cvcalib3d:\
${CMAKE_BINARY_DIR}/stromx/cvimgproc:\
${CMAKE_BINARY_DIR}/stromx/cvhighgui:\
${CMAKE_BINARY_DIR}/stromx/test\
")
set(TEST_ENVIRONMENT "PYTHONPATH=${TEST_PYTHONPATH};LD_LIBRARY_PATH=${TEST_LD_LIBRARY_PATH}")

add_subdirectory(stromx)
add_subdirectory(example)
add_subdirectory(test)