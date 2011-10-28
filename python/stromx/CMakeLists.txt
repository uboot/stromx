if(BUILD_CORE)
    add_subdirectory(core)
    add_subdirectory(base)

    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/__init__.py DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif()
