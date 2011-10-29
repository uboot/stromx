project(python_base)

set(SOURCES
    Base.cpp
)

add_library(base SHARED ${SOURCES})

include_directories(
    ../../..
    ${Boost_INCLUDE_DIRS}
    ${PYTHON_INCLUDE_DIRS}
)

target_link_libraries(base
    ${Boost_LIBRARIES}
    stromx_core
    stromx_base
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/__init__.py DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

find_file(python_base_EXISTS ${CMAKE_CURRENT_BINARY_DIR}/libbase.so)

if(python_base_EXISTS)
    file(COPY ${CMAKE_CURRENT_BINARY_DIR}/libbase.so DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif()