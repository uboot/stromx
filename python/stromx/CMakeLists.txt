if(BUILD_RUNTIME)
    add_subdirectory(runtime)
    set (AVAILABLE_PACKAGES "\"runtime\"")
endif()

if(BUILD_EXAMPLE)
    add_subdirectory(example)
    set (AVAILABLE_PACKAGES "${AVAILABLE_PACKAGES}, \"example\"")
endif()

if(BUILD_OPENCV_WRAPPER)
    add_subdirectory(core)
    add_subdirectory(imgproc)
    add_subdirectory(imgutil)
    set (AVAILABLE_PACKAGES "${AVAILABLE_PACKAGES}, \"core\", \"imgutil\", \"imgproc\"")
endif()

configure_file (
  ${CMAKE_CURRENT_SOURCE_DIR}/__init__.py.in
  ${CMAKE_CURRENT_BINARY_DIR}/__init__.py
)

if(PYTHON_INSTALL_DIR)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/__init__.py
        DESTINATION ${PYTHON_INSTALL_DIR}/stromx
    )
endif()