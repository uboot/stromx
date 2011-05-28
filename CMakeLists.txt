project(stream)

cmake_minimum_required(VERSION 2.6)

find_package( Boost 1.36.0 )
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package( Boost 1.36.0 COMPONENTS thread)

if(Boost_FOUND)
  add_subdirectory(stream)
  add_subdirectory(client)
endif()

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)
include(UseDoxygen OPTIONAL)

