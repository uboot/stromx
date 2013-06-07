#ifndef STROMX_IMGPROC_CONFIG_H
#define STROMX_IMGPROC_CONFIG_H

#define STROMX_IMGPROC_VERSION_MAJOR 0
#define STROMX_IMGPROC_VERSION_MINOR 0
#define STROMX_IMGPROC_VERSION_PATCH 1

#define STROMX_IMGPROC_PACKAGE_NAME "Imgproc"

#ifdef WIN32
    #ifdef STROMX_IMGPROC_STATIC
        #define STROMX_IMGPROC_STATIC
    #else // STROMX_IMGPROC_STATIC
        #ifdef stromx_imgproc_EXPORTS
            #define STROMX_IMGPROC_API __declspec(dllexport)
        #else // stromx_imgproc_EXPORTS
            #define STROMX_IMGPROC_API __declspec(dllimport)
        #endif // stromx_imgproc_EXPORTS
    #endif // STROMX_IMGPROC_STATIC
#else // WIN32
    #define STROMX_IMGPROC_API
#endif // WIN32

#endif // STROMX_IMGPROC_CONFIG_H