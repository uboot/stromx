include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
)

add_executable(camera
    camera.cpp
)

set_target_properties(camera PROPERTIES FOLDER "example")

target_link_libraries(camera
    stromx_runtime
    stromx_cvsupport
)

add_test(NAME example_camera COMMAND camera)
set_tests_properties(example_camera PROPERTIES ENVIRONMENT "${EXAMPLE_ENVIRONMENT}")

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/camera.xml ${CMAKE_CURRENT_BINARY_DIR}/camera.xml COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/lenna.jpg ${CMAKE_CURRENT_BINARY_DIR}/lenna.jpg COPYONLY)