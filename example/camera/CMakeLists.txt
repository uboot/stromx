project(camera)

include_directories(../..)

add_executable(camera
    camera.cpp
)

target_link_libraries(camera
    stromx_core
    stromx_base
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/camera.xml DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/lenna.jpg DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
