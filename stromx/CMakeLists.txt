if(BUILD_RUNTIME)
    add_subdirectory(runtime)
endif()

if(BUILD_EXAMPLE)
   add_subdirectory(example)
endif()


if(BUILD_OPENCV_WRAPPER)
   add_subdirectory(imgutil)
   add_subdirectory(imgproc)
endif()