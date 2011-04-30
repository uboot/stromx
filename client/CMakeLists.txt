project(client)

include_directories(..)
add_executable(client main.cpp)
target_link_libraries(client stream)
