project(python_runtime)

if(BUILD_RUNTIME)
    add_subdirectory(runtime)
    set (AVAILABLE_PACKAGES "\"runtime\"")
endif()

if(BUILD_EXAMPLE)
    add_subdirectory(example)
    set (AVAILABLE_PACKAGES "\"runtime\", \"example\"")
endif()

configure_file (
  "${PROJECT_SOURCE_DIR}/__init__.py.in"
  "${PROJECT_BINARY_DIR}/__init__.py"
)

if(PYTHON_INSTALL_DIR)
    install(FILES ${PROJECT_BINARY_DIR}/__init__.py
        DESTINATION ${PYTHON_INSTALL_DIR}/stromx
    )
endif()