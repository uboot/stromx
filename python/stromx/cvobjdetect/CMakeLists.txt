set (SOURCES 
    Cvobjdetect.cpp
)

add_library (cvobjdetect SHARED ${SOURCES})

set_target_properties (cvobjdetect PROPERTIES FOLDER "python")

include_directories (
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${Boost_INCLUDE_DIRS}
    ${OpenCV2_INCLUDE_DIR}
    ${PYTHON_INCLUDE_DIRS}
)

target_link_libraries (cvobjdetect
    ${Boost_LIBRARIES}
    ${PYTHON_LIBRARY}
    stromx_runtime
    stromx_cvsupport
    stromx_cvobjdetect
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/__init__.py ${CMAKE_CURRENT_BINARY_DIR}/__init__.py COPYONLY)

if(PYTHON_INSTALL_DIR)
    install(DIRECTORY . 
        DESTINATION ${PYTHON_INSTALL_DIR}/stromx/cvobjdetect
        FILES_MATCHING PATTERN "*.py"
    )
    install(TARGETS cvobjdetect
        DESTINATION ${PYTHON_INSTALL_DIR}/stromx/cvobjdetect
    )
endif()
