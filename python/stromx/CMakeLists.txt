add_subdirectory(runtime)
set (AVAILABLE_PACKAGES "\"runtime\"")

if(BUILD_OPENCV_WRAPPER)
    add_subdirectory(cvcore)
    add_subdirectory(cvhighgui)
    add_subdirectory(cvimgproc)
    add_subdirectory(cvsupport)
    add_subdirectory(test)
    set (AVAILABLE_PACKAGES "${AVAILABLE_PACKAGES}, \"cvcore\", \"cvhighgui\", \"cvimgproc\", \"cvsupport\", \"test\"")
endif()

if(BUILD_RASPI)
    add_subdirectory(raspi)
    set (AVAILABLE_PACKAGES "${AVAILABLE_PACKAGES}, \"raspi\"")
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