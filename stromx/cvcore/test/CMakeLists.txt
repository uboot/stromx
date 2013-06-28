add_test(NAME stromx_cvcore_test COMMAND stromx_cvcore_test)

if(MSVC)
    add_definitions(/DSTROMX_CVCORE_STATIC)
endif(MSVC)

include_directories (
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${CPPUNIT_INCLUDE_DIR}
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/barbara.jpg DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/lenna.jpg DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

set (SOURCES 
    ../Add.cpp
    ../Utility.cpp
    AddTest.cpp
    main.cpp
)

add_executable(stromx_cvcore_test ${SOURCES})

set_target_properties(stromx_cvcore_test PROPERTIES FOLDER "test")

add_dependencies(stromx_cvcore_test stromx_cvcore)

target_link_libraries (stromx_cvcore_test
    ${CPPUNIT_LIBRARY}
    ${CMAKE_DL_LIBS}
    ${OpenCV_LIBS}
    stromx_runtime
    stromx_cvimgutil
)

