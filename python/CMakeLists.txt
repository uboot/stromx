if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4172")
endif(MSVC)

add_subdirectory(stromx)
add_subdirectory(example)
add_subdirectory(test)