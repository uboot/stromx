#include "Image.h"

#include <stream/Exception.h>

namespace base
{
    Image::Image(const unsigned int width, const unsigned int height, const stream::Image::PixelType pixelType)
      : stream::Image(),
        m_dataType(stream::DataType())
    {
        
    }
    
    Image::Image(const std::string& filename)
      : stream::Image()
    {
        try
        {
            m_image = cv::imread(filename);
        }
        catch(cv::Exception& e)
        {
            throw stream::FileAccessException("Failed to load image '" + filename + "'.");
        }
    }
    
    void Image::save(const std::string& filename)
    {

    }
    
    const unsigned int Image::width() const
    {

    }

    const unsigned int Image::height() const
    {

    }

    const unsigned int Image::stride() const
    {

    }
        
    uint8_t*const Image::data()
    {

    }

    const uint8_t*const Image::data() const
    {

    }
}