#include "Utilities.h"

#include <stromx/core/Exception.h>
#include <stromx/core/Image.h>
#include <stromx/core/DataContainer.h>

#include "Image.h"

using namespace core;

namespace base
{
    cv::Mat getOpenCvMat(const core::Image& image)
    {
        int cvType = 0;
        switch(image.pixelType())
        {
        case core::Image::RGB_24:
        case core::Image::BGR_24:
            cvType = CV_8UC3;
            break;
        case core::Image::MONO_8:
        case core::Image::BAYERBG_8:
        case core::Image::BAYERGB_8:
            cvType = CV_8UC1;
            break;
        default:
            throw core::WrongArgument("Unknown pixel type.");
        }
        
        uint8_t* data = const_cast<uint8_t*>(image.data());
        
        return cv::Mat(image.height(), image.width(), cvType, data, image.stride());
    }
    
    void adjustImage(const unsigned int width, const unsigned int height, const core::Image::PixelType pixelType,
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
