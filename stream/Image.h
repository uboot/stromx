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
            GRAY_8,
            RGB_24
        };
        
        Image();
        virtual ~Image();
        
    private:
        uint8_t* m_data;
    };
}

#endif // STREAM_IMAGE_H