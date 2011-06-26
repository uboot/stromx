#ifndef STREAM_IMAGEWRAPPER_H
#define STREAM_IMAGEWRAPPER_H

#include "Image.h"

namespace stream
{
    class ImageWrapper : public Image
    {
    public:
        ImageWrapper(const unsigned int size, uint8_t* const buffer);
        
        virtual const DataType & type() const { return m_dataType; }
        virtual void setDataType(const DataType& dataType);
        
        virtual uint8_t* const buffer() { return m_buffer; }
        virtual const uint8_t* const buffer() const  { return m_buffer; }
        
        virtual const unsigned int size() const { return m_size; };
        
        virtual const unsigned int width() const { return m_width; }
        virtual void setWidth(const unsigned int width);
        
        virtual const unsigned int height() const { return m_height; }
        virtual void setHeight(const unsigned int height);
        
        virtual const unsigned int stride() const { return m_stride; }
        virtual void setStride(const unsigned int stride);
        
        virtual const PixelType pixelType() const { return m_pixelType; }
        virtual void setPixelType(const PixelType pixelType);
        
        virtual uint8_t* const data() { return m_data; }
        virtual const uint8_t* const data() const { return m_data; }
        virtual void setData(uint8_t* const value) ; 
        
    private:
        static const int numChannels(const PixelType pixelType);
        static const int depth(const PixelType pixelType);
        void validate(const unsigned int width,
                      const unsigned int height,
                      const unsigned int stride,
                      const uint8_t* const data,
                      const PixelType pixelType) const;
        
        unsigned int m_width;
        unsigned int m_height;
        unsigned int m_stride;
        unsigned int m_size;
        PixelType m_pixelType;
        uint8_t* m_data;
        uint8_t* m_buffer;
        DataType m_dataType;
    };
}

#endif // STREAM_IMAGEWRAPPER_H