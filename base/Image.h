#ifndef STREAM_IMAGEIMPL_H
#define STREAM_IMAGEIMPL_H

#include <stream/Image.h>

#include <string>
#include <opencv2/opencv.hpp>


namespace base
{
    class Image : public stream::Image
    {
    public:
        
        Image(const unsigned int width, const unsigned int height, const PixelType pixelType);
        Image(const std::string & filename);
        Image(const stream::Image& image);
        virtual ~Image();
        
        virtual void resize(const unsigned int width, const unsigned int height, const PixelType pixelType);
        virtual void save(const std::string & filename) const;
        
        virtual const unsigned int width() const;
        virtual const unsigned int height() const;
        virtual const unsigned int stride() const;
        virtual const PixelType pixelType() const { return m_pixelType; }
        virtual uint8_t* const data();
        virtual const uint8_t* const data() const;
        virtual const stream::DataType & type() const { return m_dataType; }
        
    private:
        static const int numChannels(const PixelType pixelType);
        static const int depth(const PixelType pixelType);
        static const PixelType pixelType(const int depth, const int numChannels);
        static const stream::DataType dataType(const PixelType pixelType);
        
        PixelType m_pixelType;
        stream::DataType m_dataType;
        IplImage* m_image;
    };
}

#endif // STREAM_IMAGEIMPL_H