set(SOURCES
    Cvhighgui.cpp
)

add_library(cvhighgui SHARED ${SOURCES})

add_dependencies(cvhighgui stromx_cvsupport)

set_target_properties(cvhighgui PROPERTIES FOLDER "python")

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${Boost_INCLUDE_DIRS}
    ${OpenCV2_INCLUDE_DIR}
    ${PYTHON_INCLUDE_DIRS}
)

target_link_libraries(cvhighgui
    ${Boost_LIBRARIES}
    ${PYTHON_LIBRARY}
    stromx_runtime
    stromx_cvsupport
    stromx_cvhighgui
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/__init__.py DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(PYTHON_INSTALL_DIR)
    install(DIRECTORY . 
        DESTINATION ${PYTHON_INSTALL_DIR}/stromx/cvhighgui
        FILES_MATCHING PATTERN "*.py"
    )
    install(TARGETS cvhighgui
        DESTINATION ${PYTHON_INSTALL_DIR}/stromx/cvhighgui
    )
endif()
