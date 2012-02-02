# Sets the following variables:
#  LIBZIP_FOUND       - Set to false, or undefined, if libzip isn't found.
#  LIBZIP_INCLUDE_DIR - The stromx include directory.
#  LIBZIP_LIBRARY     - The stromx library to link against.


find_path(LIBZIP_INCLUDE_DIR 
    NAMES zip.h
)

find_library(LIBZIP_LIBRARY
    NAMES zip
)


if (LIBZIP_INCLUDE_DIR AND LIBZIP_LIBRARY)
   set(LIBZIP_FOUND TRUE)
endif (LIBZIP_INCLUDE_DIR AND LIBZIP_LIBRARY)

if (LIBZIP_FOUND)
   if (NOT libzip_FIND_QUIETLY)
      message(STATUS "Found libzip: ${LIBZIP_LIBRARY}")
   endif (NOT libzip_FIND_QUIETLY)
else (LIBZIP_FOUND)
   if (libzip_FIND_REQUIRED)
      message(FATAL_ERROR "Could not find libzip")
   endif (libzip_FIND_REQUIRED)
endif (LIBZIP_FOUND)
