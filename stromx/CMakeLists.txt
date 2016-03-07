set(TEST_ENVIRONMENT "LD_LIBRARY_PATH=${LD_LIBRARY_PATH}")

add_subdirectory(runtime)

if(BUILD_OPENCV_WRAPPER)
    add_subdirectory(cvsupport)
    add_subdirectory(test)
endif()

