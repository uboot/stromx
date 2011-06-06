#include "Image.h"

#include "Exception.h"

namespace stream
{
    Image::Image()
      : m_width(0),
        m_height(0),
        m_stride(0),
        m_pixelType(MONO_8),
        m_data(0),
        m_dataType(DataType::MONO_8_IMAGE)
    {
    }
    
    Image::~Image()
    {
    }
    
    void Image::setWidth(const unsigned int width)
    {
        m_width = width;
    }

    void Image::setHeight(const unsigned int height)
    {
        m_height = height;
    }

    void Image::setStride(const unsigned int stride)
    {
        m_stride = stride;
    }

    void Image::setData(uint8_t*const data)
    {
        m_data = data;
    }

    void Image::setPixelType(const stream::Image::PixelType pixelType)
    {
        m_pixelType = pixelType;
        
        switch(m_pixelType)
        {
        case MONO_8:
            m_dataType = DataType(DataType::MONO_8_IMAGE);
            break;
        case RGB_24:
            m_dataType = DataType(DataType::RGB_24_IMAGE);
            break;
        default:
            throw ArgumentException("This pixel type is not supported.");
        }
    }

}
