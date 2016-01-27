if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4172")
endif(MSVC)

set(PYTHONPATH "${CMAKE_BINARY_DIR}/python")
set(PYTHON_TEST_ENVIRONMENT "PYTHONPATH=${PYTHONPATH};LD_LIBRARY_PATH=${LD_LIBRARY_PATH}")

add_subdirectory(stromx)
add_subdirectory(example)
add_subdirectory(test)