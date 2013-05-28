project(python_stromx_runtime)

if(BUILD_RUNTIME)
    add_subdirectory(runtime)
    set (AVAILABLE_PACKAGES "runtime")
endif()

if(BUILD_EXAMPLE)
    add_subdirectory(example)
    set (AVAILABLE_PACKAGES "${AVAILABLE_PACKAGES}, example")
endif()

if(BUILD_OPENCV_WRAPPER)
    add_subdirectory(imgutil)
    add_subdirectory(imgproc)
    set (AVAILABLE_PACKAGES "${AVAILABLE_PACKAGES}, imgutil, imgproc")
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