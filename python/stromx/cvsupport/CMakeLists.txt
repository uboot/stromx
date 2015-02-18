set(SOURCES
    Cvsupport.cpp
    Image.cpp
    Matrix.cpp
)

add_library(cvsupport SHARED ${SOURCES})

set_target_properties(cvsupport PROPERTIES FOLDER "python")

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${Boost_INCLUDE_DIRS}
    ${OpenCV2_INCLUDE_DIR}
    ${PYTHON_INCLUDE_DIRS}
)

target_link_libraries(cvsupport
    ${Boost_LIBRARIES}
    ${PYTHON_LIBRARY}
    stromx_runtime
    stromx_cvsupport
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/__init__.py ${CMAKE_CURRENT_BINARY_DIR}/__init__.py COPYONLY)

if(PYTHON_INSTALL_DIR)
    install(DIRECTORY . 
        DESTINATION ${PYTHON_INSTALL_DIR}/stromx/cvsupport
        FILES_MATCHING PATTERN "*.py"
    )
    install(TARGETS cvsupport
        DESTINATION ${PYTHON_INSTALL_DIR}/stromx/cvsupport
    )
endif()
