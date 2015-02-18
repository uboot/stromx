set(SOURCES
    Raspi.cpp
)

add_library(raspi SHARED ${SOURCES})

set_target_properties(raspi PROPERTIES FOLDER "python")

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${Boost_INCLUDE_DIRS}
    ${PYTHON_INCLUDE_DIRS}
)

target_link_libraries(raspi
    ${Boost_LIBRARIES}
    ${PYTHON_LIBRARY}
    stromx_runtime
    stromx_raspi
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/__init__.py ${CMAKE_CURRENT_BINARY_DIR}/__init__.py COPYONLY)

if(PYTHON_INSTALL_DIR)
    install(DIRECTORY . 
        DESTINATION ${PYTHON_INSTALL_DIR}/stromx/raspi
        FILES_MATCHING PATTERN "*.py"
    )
    install(TARGETS raspi
        DESTINATION ${PYTHON_INSTALL_DIR}/stromx/raspi
    )
endif()
