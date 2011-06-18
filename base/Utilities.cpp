#include "Utilities.h"

#include <stream/Exception.h>
#include <stream/Image.h>
#include <stream/DataContainer.h>

using namespace stream;

namespace base
{
    cv::Mat getOpenCvMat(const stream::Image& image)
    {
        int cvType = 0;
        switch(image.pixelType())
        {
        case stream::Image::RGB_24:
        case stream::Image::BGR_24:
            cvType = CV_8UC3;
            break;
        case stream::Image::MONO_8:
        case stream::Image::BAYERBG_8:
        case stream::Image::BAYERGB_8:
            cvType = CV_8UC1;
            break;
        default:
            throw stream::ArgumentException("Unknown pixel type.");
        }
        
        uint8_t* data = const_cast<uint8_t*>(image.data());
        
        return cv::Mat(image.height(), image.width(), cvType, data, image.stride());
    }
    
    void adjustImage(const unsigned int width, const unsigned int height, const stream::Image::PixelType pixelType,
                     DataOwner* const owner, stream::DataContainer*& image)
    {
        if(! image)
        {
            Image* outImage = new Image(width, height, pixelType);
            image = new stream::DataContainer(outImage, owner);
        }
        else
        {
            Data* outData = image->getWriteAccess();
            Image* outImage = dynamic_cast<Image*>(outData);
            
            if(!outImage)
                throw ArgumentException("Expected data of type base::Image.");
            
            if(pixelType != outImage->pixelType()
                || width != outImage->width()
                || height != outImage->height())
            {
                outImage->resize(width, height, pixelType);
            }
            
            image->clearWriteAccess();
        }
    } 
}
