project(python_stromx_imgproc)

set (SOURCES 
    Imgproc.cpp
)

add_library (imgproc SHARED ${SOURCES})

add_dependencies(imgproc stromx_imgproc)

include_directories (
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${Boost_INCLUDE_DIRS}
    ${OpenCV2_INCLUDE_DIR}
    ${PYTHON_INCLUDE_DIRS}
)

target_link_libraries (imgproc
    ${Boost_LIBRARIES}
    ${PYTHON_LIBRARY}
    stromx_runtime
    stromx_imgutil
    stromx_imgproc
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/__init__.py DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(PYTHON_INSTALL_DIR)
    install(DIRECTORY . 
        DESTINATION ${PYTHON_INSTALL_DIR}/stromx/imgproc
        FILES_MATCHING PATTERN "*.py"
    )
    install(TARGETS imgproc
        DESTINATION ${PYTHON_INSTALL_DIR}/stromx/imgproc
    )
endif()
