add_test(NAME stromx_core_test COMMAND stromx_core_test)

if(MSVC)
    add_definitions(/DSTROMX_CORE_STATIC)
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

add_executable(stromx_core_test ${SOURCES})

set_target_properties(stromx_core_test PROPERTIES FOLDER "test")

add_dependencies(stromx_core_test stromx_core)

target_link_libraries (stromx_core_test
    ${CPPUNIT_LIBRARY}
    ${CMAKE_DL_LIBS}
    ${OpenCV_LIBS}
    stromx_runtime
    stromx_imgutil
)

