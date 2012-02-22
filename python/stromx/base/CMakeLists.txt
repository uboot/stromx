project(python_base)

set(SOURCES
    Base.cpp
    Image.cpp
)

add_library(base SHARED ${SOURCES})

include_directories(
    ../../..
    ${Boost_INCLUDE_DIRS}
	${OpenCV2_INCLUDE_DIR}
    ${PYTHON_INCLUDE_DIRS}
)

target_link_libraries(base
    ${Boost_LIBRARIES}
    ${PYTHON_LIBRARY}
    stromx_core
    stromx_base
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/__init__.py DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(PYTHON_INSTALL_DIR)
    install(DIRECTORY . 
        DESTINATION ${PYTHON_INSTALL_DIR}/stromx/base
        FILES_MATCHING PATTERN "*.py"
    )
    install(TARGETS base 
        DESTINATION ${PYTHON_INSTALL_DIR}/stromx/base
    )
endif()
