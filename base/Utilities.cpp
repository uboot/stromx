#include "Utilities.h"

#include <stream/Exception.h>
#include <stream/Image.h>

namespace base
{
    cv::Mat getOpenCvMat(const stream::Image& image)
    {
        int cvType = 0;
        switch(image.pixelType())
        {
        case stream::Image::RGB_24:
            cvType = CV_8UC3;
            break;
        case stream::Image::MONO_8:
            cvType = CV_8UC1;
            break;
        default:
            throw stream::ArgumentException("Unknown pixel type.");
        }
        
        uint8_t* data = const_cast<uint8_t*>(image.data());
        
        return cv::Mat(image.width(), image.height(), cvType, data, image.stride());
    }
}
