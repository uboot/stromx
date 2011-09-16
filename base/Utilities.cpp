#include "Utilities.h"

#include <strom/Exception.h>
#include <strom/Image.h>
#include <base/Image.h>
#include <strom/DataContainer.h>

using namespace strom;

namespace base
{
    cv::Mat getOpenCvMat(const strom::Image& image)
    {
        int cvType = 0;
        switch(image.pixelType())
        {
        case strom::Image::RGB_24:
        case strom::Image::BGR_24:
            cvType = CV_8UC3;
            break;
        case strom::Image::MONO_8:
        case strom::Image::BAYERBG_8:
        case strom::Image::BAYERGB_8:
            cvType = CV_8UC1;
            break;
        default:
            throw strom::WrongArgument("Unknown pixel type.");
        }
        
        uint8_t* data = const_cast<uint8_t*>(image.data());
        
        return cv::Mat(image.height(), image.width(), cvType, data, image.stride());
    }
    
    void adjustImage(const unsigned int width, const unsigned int height, const strom::Image::PixelType pixelType,
                     base::Image*& image)
    {
        if(! image)
        {
            image = new base::Image(width, height, pixelType);
        }
        else
        {
            if(pixelType != image->pixelType()
                || width != image->width()
                || height != image->height())
            {
                image->resize(width, height, pixelType);
            }
        }
    } 
}
