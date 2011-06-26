#include "ImageWrapper.h"

#include "Exception.h"

namespace stream
{
    ImageWrapper::ImageWrapper(const unsigned int size, uint8_t* const buffer)
      : m_size(size),
        m_buffer(buffer),
        m_width(0),
        m_height(0),
        m_stride(0),
        m_pixelType(MONO_8),
        m_data(0),
        m_dataType(DataType::MONO_8_IMAGE)
    {
    }
    
    void ImageWrapper::setWidth(const unsigned int width)
    {
        validate(width, m_height, m_stride, m_data, m_pixelType);
        m_width = width;
    }

    void ImageWrapper::setHeight(const unsigned int height)
    {
        validate(m_width, height, m_stride, m_data, m_pixelType);
        m_height = height;
    }

    void ImageWrapper::setStride(const unsigned int stride)
    {
        validate(m_width, m_height, stride, m_data, m_pixelType);
        m_stride = stride;
    }

    void ImageWrapper::setData(uint8_t*const data)
    {
        validate(m_width, m_height, m_stride, data, m_pixelType);
        m_data = data;
    }

    void ImageWrapper::setPixelType(const PixelType pixelType)
    {
        validate(m_width, m_height, m_stride, m_data, pixelType);
        m_pixelType = pixelType;
    }
    
    void ImageWrapper::setDataType(const stream::DataType& dataType)
    {
        m_dataType = dataType;
    }
    
    const int ImageWrapper::numChannels(const stream::Image::PixelType pixelType)
    {
        switch(pixelType)
        {
        case stream::Image::MONO_8:
        case stream::Image::MONO_16:
        case stream::Image::BAYERBG_8:
        case stream::Image::BAYERGB_8:
        case stream::Image::BAYERBG_16:
        case stream::Image::BAYERGB_16:
            return 1;
        case stream::Image::RGB_24:
        case stream::Image::BGR_24:
        case stream::Image::RGB_48:
        case stream::Image::BGR_48:
            return 3;
        default:
            throw stream::ArgumentException("Unknown pixel type.");    
        }
    }
    
    const int ImageWrapper::depth(const stream::Image::PixelType pixelType)
    {
        switch(pixelType)
        {
        case stream::Image::MONO_8:
        case stream::Image::RGB_24:
        case stream::Image::BGR_24:
        case stream::Image::BAYERBG_8:
        case stream::Image::BAYERGB_8:
            return 1;
        case stream::Image::MONO_16:
        case stream::Image::RGB_48:
        case stream::Image::BGR_48:
        case stream::Image::BAYERBG_16:
        case stream::Image::BAYERGB_16:
            return 2;
        default:
            throw stream::ArgumentException("Unknown pixel type.");    
        }
    }
    
    void ImageWrapper::validate(const unsigned int width,
                                const unsigned int height,
                                const unsigned int stride,
                                const uint8_t*const data,
                                const stream::Image::PixelType pixelType) const
    {
        unsigned int dataPerPixel = depth(pixelType) * numChannels(pixelType);
        
        // check row length
        if(width * dataPerPixel > stride)
            throw ArgumentException("Too small stride.");
        
        // check total data size
        unsigned int dataSize = stride * height;
        if(data + dataSize > m_buffer + m_size)
            throw ArgumentException("Too small buffer.");
    }

    
    
}
