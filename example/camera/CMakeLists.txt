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
    stromx_cvimgproc
    stromx_cvsupport
)

add_test(NAME example_camera COMMAND camera)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/camera.xml DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/lenna.jpg DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
