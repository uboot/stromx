#ifndef STROM_IMAGEIMPL_H
#define STROM_IMAGEIMPL_H

#include <stromx/core/ImageWrapper.h>

#include <string>
#include <opencv2/opencv.hpp>


namespace base
{
    class Image : public core::ImageWrapper
    {
    public:
        Image();
        Image(const unsigned int width, const unsigned int height, const PixelType pixelType);
        Image(const std::string & filename);
        Image(const core::Image& image);
        Image(const unsigned int size);
        virtual ~Image();
        
        virtual const core::Version & version() const { return VERSION; }
        virtual const std::string & type() const { return TYPE; }
        virtual const std::string & package() const { return PACKAGE; }
        
        virtual Data* const clone() const { return new Image; }
        
        virtual const std::string serialize(const std::string & name, const std::string & path) const;
        virtual void deserialize(const std::string & data, const std::string & path);
        
        void open(const std::string& filename);
        void save(const std::string& filename) const;
        void resize(const unsigned int width, const unsigned int height, const core::Image::PixelType pixelType);
        void resize(const unsigned int size);
        
    private:
        static const std::string TYPE;
        static const std::string PACKAGE;
        static const core::Version VERSION;
        
        static const PixelType pixelTypeFromParameters(const int depth, const int numChannels);
        static const core::DataVariant dataTypeFromPixelType(const PixelType pixelType);
        
        void getDataFromCvImage(const PixelType pixelType);
        
        IplImage* m_image;
    };
}

#endif // STROM_IMAGEIMPL_H