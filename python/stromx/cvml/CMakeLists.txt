set(SOURCES
    Cvml.cpp
)

add_library(cvml SHARED ${SOURCES})

set_target_properties(cvml PROPERTIES FOLDER "python")

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${Boost_INCLUDE_DIRS}
    ${OpenCV2_INCLUDE_DIR}
    ${PYTHON_INCLUDE_DIRS}
)

target_link_libraries(cvml
    ${Boost_LIBRARIES}
    ${PYTHON_LIBRARY}
    stromx_runtime
    stromx_cvml
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/__init__.py ${CMAKE_CURRENT_BINARY_DIR}/__init__.py COPYONLY)

if(PYTHON_INSTALL_DIR)
    install(DIRECTORY . 
        DESTINATION ${PYTHON_INSTALL_DIR}/stromx/cvml
        FILES_MATCHING PATTERN "*.py"
    )
    install(TARGETS cvml
        DESTINATION ${PYTHON_INSTALL_DIR}/stromx/cvml
    )
endif()
