set (SOURCES 
    Cvcore.cpp
)

add_library (cvcore SHARED ${SOURCES})

add_dependencies(cvcore stromx_cvcore)

set_target_properties (cvcore PROPERTIES FOLDER "python")

include_directories (
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${Boost_INCLUDE_DIRS}
    ${OpenCV2_INCLUDE_DIR}
    ${PYTHON_INCLUDE_DIRS}
)

target_link_libraries (cvcore
    ${Boost_LIBRARIES}
    ${PYTHON_LIBRARY}
    stromx_runtime
    stromx_cvsupport
    stromx_cvcore
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/__init__.py DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(PYTHON_INSTALL_DIR)
    install(DIRECTORY . 
        DESTINATION ${PYTHON_INSTALL_DIR}/stromx/cvcore
        FILES_MATCHING PATTERN "*.py"
    )
    install(TARGETS cvcore
        DESTINATION ${PYTHON_INSTALL_DIR}/stromx/cvcore
    )
endif()
