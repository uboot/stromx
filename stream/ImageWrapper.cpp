#include "ImageWrapper.h"

#include "Exception.h"

namespace stream
{
    ThinImage::ThinImage()
      : m_width(0),
        m_height(0),
        m_stride(0),
        m_pixelType(MONO_8),
        m_data(0),
        m_dataType(DataType::MONO_8_IMAGE)
    {
    }
    
    void ThinImage::setWidth(const unsigned int width)
    {
        m_width = width;
    }

    void ThinImage::setHeight(const unsigned int height)
    {
        m_height = height;
    }

    void ThinImage::setStride(const unsigned int stride)
    {
        m_stride = stride;
    }

    void ThinImage::setData(uint8_t*const data)
    {
        m_data = data;
    }

    void ThinImage::setPixelType(const stream::Image::PixelType pixelType)
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
    
    void ThinImage::setDataType(const stream::DataType& dataType)
    {
        m_dataType = dataType;
    }
}
