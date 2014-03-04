set (SOURCES 
    Test.cpp
)

add_library (test_package SHARED ${SOURCES})

add_dependencies(test_package stromx_test)

set_target_properties (test_package PROPERTIES FOLDER "python")
set_target_properties (test_package PROPERTIES OUTPUT_NAME "test")

include_directories (
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${Boost_INCLUDE_DIRS}
    ${OpenCV2_INCLUDE_DIR}
    ${PYTHON_INCLUDE_DIRS}
)

target_link_libraries (test_package
    ${Boost_LIBRARIES}
    ${PYTHON_LIBRARY}
    stromx_runtime
    stromx_cvsupport
    stromx_test
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/__init__.py DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(PYTHON_INSTALL_DIR)
    install(DIRECTORY . 
        DESTINATION ${PYTHON_INSTALL_DIR}/stromx/test
        FILES_MATCHING PATTERN "*.py"
    )
    install(TARGETS test_package
        DESTINATION ${PYTHON_INSTALL_DIR}/stromx/test
    )
endif()
