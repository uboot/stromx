add_subdirectory(runtime)

if(BUILD_OPENCV_WRAPPER)
    add_subdirectory(cvcalib3d)
    add_subdirectory(cvcore)
    add_subdirectory(cvhighgui)
    add_subdirectory(cvimgproc)
    add_subdirectory(cvsupport)
    add_subdirectory(test)
endif()

if(BUILD_RASPI)
    add_subdirectory(raspi)
endif()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/__init__.py ${CMAKE_CURRENT_BINARY_DIR}/__init__.py COPYONLY)

if(PYTHON_INSTALL_DIR)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/__init__.py
        DESTINATION ${PYTHON_INSTALL_DIR}/stromx
    )
endif()