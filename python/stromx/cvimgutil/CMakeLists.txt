set(SOURCES
    CvImgutil.cpp
    Image.cpp
    Matrix.cpp
)

add_library(cvimgutil SHARED ${SOURCES})

add_dependencies(cvimgutil stromx_imgutil)

set_target_properties(cvimgutil PROPERTIES FOLDER "python")

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${Boost_INCLUDE_DIRS}
    ${OpenCV2_INCLUDE_DIR}
    ${PYTHON_INCLUDE_DIRS}
)

target_link_libraries(cvimgutil
    ${Boost_LIBRARIES}
    ${PYTHON_LIBRARY}
    stromx_runtime
    stromx_cvimgutil
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/__init__.py DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(PYTHON_INSTALL_DIR)
    install(DIRECTORY . 
        DESTINATION ${PYTHON_INSTALL_DIR}/stromx/cvimgutil
        FILES_MATCHING PATTERN "*.py"
    )
    install(TARGETS cvimgutil
        DESTINATION ${PYTHON_INSTALL_DIR}/stromx/cvimgutil
    )
endif()
