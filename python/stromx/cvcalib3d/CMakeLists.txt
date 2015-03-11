set (SOURCES 
    Cvcalib3d.cpp
)

add_library (cvcalib3d SHARED ${SOURCES})

set_target_properties (cvcalib3d PROPERTIES FOLDER "python")

include_directories (
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${Boost_INCLUDE_DIRS}
    ${OpenCV2_INCLUDE_DIR}
    ${PYTHON_INCLUDE_DIRS}
)

target_link_libraries (cvcalib3d
    ${Boost_LIBRARIES}
    ${PYTHON_LIBRARY}
    stromx_runtime
    stromx_cvsupport
    stromx_cvcalib3d
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/__init__.py ${CMAKE_CURRENT_BINARY_DIR}/__init__.py COPYONLY)

if(PYTHON_INSTALL_DIR)
    install(DIRECTORY . 
        DESTINATION ${PYTHON_INSTALL_DIR}/stromx/cvcalib3d
        FILES_MATCHING PATTERN "*.py"
    )
    install(TARGETS cvcalib3d
        DESTINATION ${PYTHON_INSTALL_DIR}/stromx/cvcalib3d
    )
endif()
