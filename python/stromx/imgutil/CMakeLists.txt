project(python_stromx_imgutil)

set(SOURCES
    Imgutil.cpp
    Image.cpp
    Matrix.cpp
)

add_library(imgutil SHARED ${SOURCES})

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${Boost_INCLUDE_DIRS}
    ${OpenCV2_INCLUDE_DIR}
    ${PYTHON_INCLUDE_DIRS}
)

target_link_libraries(imgutil
    ${Boost_LIBRARIES}
    ${PYTHON_LIBRARY}
    stromx_runtime
    stromx_imgutil
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/__init__.py DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(PYTHON_INSTALL_DIR)
    install(DIRECTORY . 
        DESTINATION ${PYTHON_INSTALL_DIR}/stromx/imgutil
        FILES_MATCHING PATTERN "*.py"
    )
    install(TARGETS imgutil 
        DESTINATION ${PYTHON_INSTALL_DIR}/stromx/imgutil
    )
endif()
