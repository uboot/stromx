add_test(NAME python_test_exceptions COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/exceptions.py)
add_test(NAME python_test_factory COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/factory.py)
add_test(NAME python_test_providers COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/providers.py)

set(TEST_PYTHONPATH "${CMAKE_BINARY_DIR}/python")
set_tests_properties(python_test_exceptions PROPERTIES ENVIRONMENT "PYTHONPATH=${TEST_PYTHONPATH}")
set_tests_properties(python_test_factory PROPERTIES ENVIRONMENT "PYTHONPATH=${TEST_PYTHONPATH}")
set_tests_properties(python_test_providers PROPERTIES ENVIRONMENT "PYTHONPATH=${TEST_PYTHONPATH}")