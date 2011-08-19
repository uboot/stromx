#ifndef STREAM_IMAGEWRAPPER_H
#define STREAM_IMAGEWRAPPER_H

#include "Image.h"

namespace stream
{
    class ImageWrapper : public Image
    {
    public:
        ImageWrapper(const unsigned int size, uint8_t* const buffer);
        ImageWrapper();
        
        virtual const DataType & type() const { return m_dataType; }
        
        virtual uint8_t* const buffer() { return m_buffer; }
        
        virtual const unsigned int size() const { return m_size; };
        
        virtual const unsigned int width() const { return m_width; }
        
        virtual const unsigned int height() const { return m_height; }
        
        virtual const unsigned int stride() const { return m_stride; }
        
        virtual const PixelType pixelType() const { return m_pixelType; }
        
        virtual const unsigned int pixelSize() const;
        
        virtual uint8_t* const data() { return m_data; }
        virtual const uint8_t* const data() const { return m_data; }
        
        virtual void initialize(const unsigned int width, 
                                const unsigned int height, 
                                const unsigned int stride, 
                                uint8_t* const data, 
                                const PixelType pixelType);
        
    protected:
        static const int numChannels(const PixelType pixelType);
        static const int depth(const PixelType pixelType);
        
        virtual void setDataType(const DataType& dataType);
        
        virtual void setSize(const unsigned int size);
        virtual void setBuffer(uint8_t* const buffer);
        
    private:
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