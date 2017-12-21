if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4172")
endif(MSVC)

add_definitions(-DBOOST_PYTHON_USE_GCC_SYMBOL_VISIBILITY)

set(PYTHONPATH "${CMAKE_BINARY_DIR}/python")
set(PYTHON_TEST_ENVIRONMENT "PYTHONPATH=${PYTHONPATH};LD_LIBRARY_PATH=${LD_LIBRARY_PATH}")

add_subdirectory(stromx)
add_subdirectory(example)
add_subdirectory(test)
