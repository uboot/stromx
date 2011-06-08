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
            RGB_24
        };
        
        virtual ~Image() {}
        
        virtual const unsigned int width() const = 0;
        virtual const unsigned int height() const = 0;
        virtual const unsigned int stride() const = 0;
        virtual const PixelType pixelType() const = 0;
        
        virtual uint8_t* const data() = 0;
        virtual const uint8_t* const data() const = 0;
        
        virtual const DataType & type() const = 0;
    };
}

#endif // STREAM_IMAGE_H