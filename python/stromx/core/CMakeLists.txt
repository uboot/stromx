project(python_core)

set(SOURCES
    Core.cpp
    Operator.cpp
    Factory.cpp
    Registry.cpp
    Stream.cpp
    Version.cpp
)

add_library(core SHARED ${SOURCES})

include_directories(
    ../../..
    ${Boost_INCLUDE_DIRS}
    ${PYTHON_INCLUDE_DIRS}
)

target_link_libraries(core
    ${Boost_LIBRARIES}
    stromx_core
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/__init__.py DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

find_file(python_base_EXISTS ${CMAKE_CURRENT_BINARY_DIR}/libcore.so)

if(python_base_EXISTS)
    file(COPY ${CMAKE_CURRENT_BINARY_DIR}/libcore.so DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif()
