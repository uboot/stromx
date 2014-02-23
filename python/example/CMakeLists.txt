file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/camera.py DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/camera.xml DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/file.py DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/file.xml DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/lenna.jpg DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/observer.py DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/simple.py DESTINATION ${CMAKE_CURRENT_BINARY_DIR})


add_test(NAME python_example_camera COMMAND python camera.py)
add_test(NAME python_example_file COMMAND python file.py)
add_test(NAME python_example_observer COMMAND python observer.py)
add_test(NAME python_example_simple COMMAND python simple.py)

set(TEST_PYTHONPATH "${CMAKE_BINARY_DIR}/python")
set_tests_properties(python_example_camera PROPERTIES ENVIRONMENT "PYTHONPATH=${TEST_PYTHONPATH}")
set_tests_properties(python_example_file PROPERTIES ENVIRONMENT "PYTHONPATH=${TEST_PYTHONPATH}")
set_tests_properties(python_example_observer PROPERTIES ENVIRONMENT "PYTHONPATH=${TEST_PYTHONPATH}")
set_tests_properties(python_example_simple PROPERTIES ENVIRONMENT "PYTHONPATH=${TEST_PYTHONPATH}")