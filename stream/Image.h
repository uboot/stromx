#ifndef STREAM_IMAGE_H
#define STREAM_IMAGE_H

#include "Data.h"

#include <stdint.h>

namespace stream
{
    class Image : public Data
    {
    public:
        enum PixelType
        {
            MONO_8,
            MONO_16,
            RGB_24,
            RGB_48,
            BGR_24,
            BGR_48,
            BAYERBG_8,
            BAYERBG_16,
            BAYERGB_8,
            BAYERGB_16
        };
        
        virtual ~Image() {}
                                
        virtual const DataType & type() const = 0;
        virtual void setDataType(const DataType& dataType) = 0;
        
        virtual uint8_t* const buffer() = 0;
        
        virtual const unsigned int size() const = 0;
        
        virtual const unsigned int width() const = 0;
        
        virtual const unsigned int height() const = 0;
        
        virtual const unsigned int stride() const = 0;
        
        virtual const PixelType pixelType() const = 0;
        
        virtual const unsigned int pixelSize() const = 0;
        
        virtual uint8_t* const data() = 0;
        virtual const uint8_t* const data() const = 0;
        
        virtual void initialize(const unsigned int width, 
                                const unsigned int height, 
                                const unsigned int stride, 
                                uint8_t* const data, 
                                const PixelType pixelType) = 0; 
    };
}

#endif // STREAM_IMAGE_H