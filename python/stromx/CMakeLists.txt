project(python_core)

if(BUILD_CORE)
    add_subdirectory(core)
    set (AVAILABLE_PACKAGES "\"core\"")
endif()

if(BUILD_BASE)
    add_subdirectory(base)
    set (AVAILABLE_PACKAGES "\"core\", \"base\"")
endif()

configure_file (
  "${PROJECT_SOURCE_DIR}/__init__.py.in"
  "${PROJECT_BINARY_DIR}/__init__.py"
)