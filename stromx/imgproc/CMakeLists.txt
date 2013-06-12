set (IMGPROC_VERSION_MAJOR 0)
set (IMGPROC_VERSION_MINOR 0)
set (IMGPROC_VERSION_PATCH 1)

configure_file (
    ${CMAKE_CURRENT_SOURCE_DIR}/Config.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/Config.h
)

include_directories (
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${Boost_INCLUDE_DIRS}
)

set (SOURCES 
    Imgproc.cpp
    Utility.cpp
    BilateralFilter.cpp
    Blur.cpp
    BoxFilter.cpp
    Dilate.cpp
    Erode.cpp
    GaussianBlur.cpp
    MedianBlur.cpp
    MorphologyEx.cpp
    Laplacian.cpp
    PyrDown.cpp
    PyrUp.cpp
    Resize.cpp
    Scharr.cpp
    Sobel.cpp
)

add_library (stromx_imgproc SHARED ${SOURCES})

add_dependencies(stromx_imgproc stromx_runtime)

set(VERSION_STRING "${IMGPROC_VERSION_MAJOR}.${IMGPROC_VERSION_MINOR}.${IMGPROC_VERSION_PATCH}")

set_target_properties (stromx_imgproc PROPERTIES
    VERSION ${VERSION_STRING}
    SOVERSION ${VERSION_STRING}
    FOLDER "library"
)

target_link_libraries (stromx_imgproc
    ${OpenCV_LIBS}
    stromx_runtime
    stromx_imgutil
)

if(WIN32)
    install (TARGETS stromx_imgproc
        RUNTIME DESTINATION .
        LIBRARY DESTINATION ${LIB_DIR}
        ARCHIVE DESTINATION ${LIB_DIR}
    )
else(WIN32)
    install (TARGETS stromx_imgproc
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION ${LIB_DIR}
        ARCHIVE DESTINATION ${LIB_DIR}
    )
endif(WIN32)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/stromx/imgproc
    DESTINATION include/stromx
    FILES_MATCHING PATTERN "*.h"
    PATTERN "test*" EXCLUDE
)

install(DIRECTORY ${CMAKE_BINARY_DIR}/stromx/imgproc
    DESTINATION include/stromx
    FILES_MATCHING PATTERN "*.h"
)

if(BUILD_TESTS)
    add_subdirectory(test)
endif(BUILD_TESTS)
