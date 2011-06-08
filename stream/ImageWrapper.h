#ifndef STREAM_THINIMAGE_H
#define STREAM_THINIMAGE_H

#include "Image.h"

namespace stream
{
    class ThinImage : public Image
    {
    public:
        
        ThinImage();
        
        const unsigned int width() const { return m_width; }
        const unsigned int height() const { return m_height; }
        const unsigned int stride() const { return m_stride; }
        const PixelType pixelType() const { return m_pixelType; }
        
        uint8_t* const data() { return m_data; }
        const uint8_t* const data() const { return m_data; }
        
        const DataType & type() const { return m_dataType; }
       
    protected:
        void setWidth(const unsigned int width);
        void setHeight(const unsigned int height);
        void setStride(const unsigned int stride);
        void setPixelType(const PixelType pixelType);
        void setData(uint8_t* const data);
        void setDataType(const DataType& dataType);
        
    private:
        unsigned int m_width;
        unsigned int m_height;
        unsigned int m_stride;
        PixelType m_pixelType;
        uint8_t* m_data;
        DataType m_dataType;
    };
}

#endif // STREAM_THINIMAGE_H