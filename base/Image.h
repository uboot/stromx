#ifndef STREAM_IMAGEIMPL_H
#define STREAM_IMAGEIMPL_H

#include <stream/ImageWrapper.h>

#include <string>
#include <opencv2/opencv.hpp>


namespace base
{
    class Image : public stream::ImageWrapper
    {
    public:
        Image(const unsigned int width, const unsigned int height, const PixelType pixelType);
        Image(const std::string & filename);
        Image(const stream::Image& image);
        virtual ~Image();
        
        void save(const std::string& filename) const;
        void resize(const unsigned int width, const unsigned int height, const stream::Image::PixelType pixelType);
        
    private:
        static const PixelType pixelTypeFromParameters(const int depth, const int numChannels);
        static const stream::DataType dataTypeFromPixelType(const PixelType pixelType);
        
        void getDataFromCvImage();
        
        IplImage* m_image;
    };
}

#endif // STREAM_IMAGEIMPL_H