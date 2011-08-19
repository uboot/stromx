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
    
    ImageWrapper::ImageWrapper()
      : m_size(0),
        m_buffer(0),
        m_width(0),
        m_height(0),
        m_stride(0),
        m_pixelType(MONO_8),
        m_data(0),
        m_dataType(DataType::MONO_8_IMAGE)
    {
    }
     
    void ImageWrapper::setDataType(const stream::DataType& dataType)
    {
        m_dataType = dataType;
    }
    
    void ImageWrapper::setSize(const unsigned int size)
    {
        m_size = size;
    }

    void ImageWrapper::setBuffer(uint8_t*const buffer)
    {
        m_buffer = buffer;
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
            throw stream::WrongArgument("Unknown pixel type.");    
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
            throw stream::WrongArgument("Unknown pixel type.");    
        }
    }
    
    const unsigned int ImageWrapper::pixelSize() const
    {
        return depth(m_pixelType) * numChannels(m_pixelType);
    }
    
    
        
    void ImageWrapper::initialize(const unsigned int width, 
                            const unsigned int height, 
                            const unsigned int stride,
                            uint8_t*const data, 
                            const PixelType pixelType)
    {
        validate(width, height, stride, data, pixelType);
        
        m_width = width;
        m_height = height;
        m_stride = stride;
        m_data = data;
        m_pixelType = pixelType;
    }
    
    void ImageWrapper::validate(const unsigned int width,
                                const unsigned int height,
                                const unsigned int stride,
                                const uint8_t*const data,
                                const stream::Image::PixelType pixelType) const
    {
        if(width == 0 || height == 0)
            return;
        
        // check row length
        if(width * depth(pixelType) * numChannels(pixelType) > stride)
            throw WrongArgument("Too small stride.");
        
        // check total data size
        unsigned int dataSize = stride * (height - 1) + width;
        if(data + dataSize > m_buffer + m_size)
            throw WrongArgument("Too small buffer.");
    }
}
