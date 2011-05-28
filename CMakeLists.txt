project(stream)

cmake_minimum_required(VERSION 2.6)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)
include(UseDoxygen OPTIONAL)

add_subdirectory(stream)
add_subdirectory(client)
