#include "ImageWrapper.h"

#include "Exception.h"

namespace stream
{
    ImageWrapper::ImageWrapper()
      : m_width(0),
        m_height(0),
        m_stride(0),
        m_pixelType(MONO_8),
        m_data(0),
        m_dataType(DataType::MONO_8_IMAGE)
    {
    }
    
    void ImageWrapper::setWidth(const unsigned int width)
    {
        m_width = width;
    }

    void ImageWrapper::setHeight(const unsigned int height)
    {
        m_height = height;
    }

    void ImageWrapper::setStride(const unsigned int stride)
    {
        m_stride = stride;
    }

    void ImageWrapper::setData(uint8_t*const data)
    {
        m_data = data;
    }

    void ImageWrapper::setPixelType(const PixelType pixelType)
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
    
    void ImageWrapper::setDataType(const stream::DataType& dataType)
    {
        m_dataType = dataType;
    }
}
