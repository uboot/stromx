add_subdirectory(runtime)

if(BUILD_OPENCV_WRAPPER)
   add_subdirectory(cvcore)
   add_subdirectory(cvsupport)
   add_subdirectory(cvhighgui)
   add_subdirectory(cvimgproc)
   add_subdirectory(test)
endif()

if(BUILD_RASPI)
    add_subdirectory(raspi)
endif()

