#include "Image.h"

#include <stream/Exception.h>

namespace base
{
    Image::Image(const unsigned int width, const unsigned int height, const stream::Image::PixelType pixelType)
      : m_pixelType(pixelType),
        m_dataType(dataType(pixelType)),
        m_image(0)
    {
        try
        {
            m_image = cvCreateImage(cv::Size(width, height), depth(pixelType), numChannels(pixelType));
        }
        catch(cv::Exception& e)
        {
            throw stream::OutOfMemoryException("Failed to create new image.");
        }
    }
    
    Image::Image(const std::string& filename)
      : stream::Image(),
        m_dataType(stream::DataType::IMAGE)
    {
        try
        {
            m_image = cvLoadImage(filename.c_str());
            m_pixelType = pixelType(m_image->depth, m_image->nChannels);
            m_dataType = dataType(m_pixelType);
        }
        catch(cv::Exception& e)
        {
            throw stream::FileAccessException("Failed to load image '" + filename + "'.");
        }
    }
    
void Image::resize(const unsigned int width, const unsigned int height, const stream::Image::PixelType pixelType)
{
    if(m_image)
        cvReleaseImage(&m_image);
    
    try
    {
        m_image = cvCreateImage(cv::Size(width, height), depth(pixelType), numChannels(pixelType));
    }
    catch(cv::Exception& e)
    {
        throw stream::OutOfMemoryException("Failed to create new image.");
    }
}

    
    void Image::save(const std::string& filename) const
    {
        try
        {
            cvSaveImage(filename.c_str(), m_image);
        }
        catch(cv::Exception& e)
        {
            throw stream::FileAccessException("Failed to save image '" + filename + "'.");
        }
    }
    
    Image::~Image()
    {
        cvReleaseImage(&m_image);
    }

    
    const unsigned int Image::width() const
    {
        return m_image->width;
    }

    const unsigned int Image::height() const
    {
        return m_image->height;
    }

    const unsigned int Image::stride() const
    {
        return m_image->widthStep;

    }
        
    uint8_t*const Image::data()
    {
        return reinterpret_cast<uint8_t*>(m_image->imageData);
    }

    const uint8_t*const Image::data() const
    {
        return reinterpret_cast<uint8_t*>(m_image->imageData);
    }
    
    const int Image::numChannels(const stream::Image::PixelType pixelType)
    {
        switch(pixelType)
        {
        case stream::Image::MONO_8:
            return 1;
        case stream::Image::RGB_24:
            return 3;
        default:
            throw stream::ArgumentException("Unknown pixel type.");    
        }
    }
    
    const int Image::depth(const stream::Image::PixelType pixelType)
    {
        switch(pixelType)
        {
        case stream::Image::MONO_8:
            return 8;
        case stream::Image::RGB_24:
            return 8;
        default:
            throw stream::ArgumentException("Unknown pixel type.");    
        }
    }
    
    const stream::DataType Image::dataType(const stream::Image::PixelType pixelType)
    {
        switch(pixelType)
        {
        case stream::Image::MONO_8:
            return stream::DataType(stream::DataType::MONO_8_IMAGE);
        case stream::Image::RGB_24:
            return stream::DataType(stream::DataType::RGB_24_IMAGE);
        default:
            throw stream::ArgumentException("Unknown pixel type.");  
        }
    }


    const stream::Image::PixelType Image::pixelType(const int depth, const int numChannels)
    {
        switch(depth)
        {
        case 8:
            switch(numChannels)
            {
            case 1:
                return stream::Image::MONO_8;
            case 3:
                return stream::Image::RGB_24;
            default:
                throw stream::ArgumentException("Unknown combination of depth and number of channels.");
            }
        case 16:
            switch(numChannels)
            {
            default:
                throw stream::ArgumentException("Unknown combination of depth and number of channels.");      
            }
        default:
            throw stream::ArgumentException("Unknown combination of depth and number of channels.");    
        }
            
    }

}