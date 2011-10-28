project(python)

set(SOURCES
    Core.cpp
    Operator.cpp
    Factory.cpp
    Stream.cpp
)

add_library(core SHARED ${SOURCES})

include_directories(
    ../../..
    ${Boost_INCLUDE_DIRS}
    ${PYTHON_INCLUDE_DIRS}
)

target_link_libraries(core
    stromx_core
    ${Boost_LIBRARIES}
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/__init__.py DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_BINARY_DIR}/libcore.so DESTINATION ${CMAKE_CURRENT_BINARY_DIR})