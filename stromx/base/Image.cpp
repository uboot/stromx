#include "Image.h"

#include "Config.h"
#include "Utilities.h"

#include <stromx/core/Exception.h>

namespace stromx
{
    namespace base
    {
        const std::string Image::TYPE = "Image";
        const std::string Image::PACKAGE = PACKAGE_NAME;
        const core::Version Image::VERSION = core::Version(BASE_VERSION_MAJOR, BASE_VERSION_MINOR);
        
        Image::Image(const unsigned int width, const unsigned int height, const core::Image::PixelType pixelType)
        {
            try
            {
                m_image = cvCreateImage(cv::Size(width, height), depth(pixelType) * 8, numChannels(pixelType));
                
                getDataFromCvImage(pixelType);
                setVariant(dataTypeFromPixelType(pixelType));
            }
            catch(cv::Exception& e)
            {
                throw core::OutOfMemory("Failed to create new image.");
            }
        }
        
        Image::Image(const core::Image& image)
        {
            try
            {
                m_image = cvCreateImage(cv::Size(image.width(), image.height()),
                                        depth(image.pixelType()) * 8, numChannels(image.pixelType()));
                
                getDataFromCvImage(image.pixelType());
                setVariant(image.variant());
            }
            catch(cv::Exception& e)
            {
                throw core::OutOfMemory("Failed to create new image.");
            }
            
            cv::Mat cvInImage = getOpenCvMat(image);
            cv::Mat cvImage(m_image);
            cvInImage.copyTo(cvImage);
        }
        
        Image::Image()
        : m_image(0)
        {
            setSize(0);
            initialize(0, 0, 0, 0, core::Image::NONE);
            setVariant(core::DataVariant::IMAGE);
        }
        
        Image::Image(const std::string& filename)
        : m_image(0)
        {
            open(filename);
        }
        
        Image::Image(const unsigned int size)
        {
            try
            {
                m_image = cvCreateImage(cv::Size(size, 1), 8, 1);
                
                getDataFromCvImage(core::Image::MONO_8);
                setVariant(core::DataVariant::IMAGE);
            }
            catch(cv::Exception& e)
            {
                throw core::OutOfMemory("Failed to allocate image.");
            }
        } 
        
        const std::string Image::serialize(const std::string& name, const std::string& path) const
        {
            try
            {
                std::string filename = name + ".png";
                std::string filepath = path + filename;
                save(filepath);
                return filename;
            }
            catch(core::Exception&)
            {
                throw core::SerializationError(*this, name, path);
            }
        }

        void Image::deserialize(const std::string& data, const std::string& path)
        {
            try
            {
                std::string filepath = path + data;
                open(filepath);
            }
            catch(core::Exception&)
            {
                throw core::DeserializationError(*this, data, path);
            }
        }

        void Image::open(const std::string& filename)
        {
            if(m_image)
                cvReleaseImage(&m_image);
            
            m_image = cvLoadImage(filename.c_str());
            
            if(! m_image)
                throw core::FileAccessFailed(filename, "Failed to load image.");
                
            getDataFromCvImage(pixelTypeFromParameters(m_image->depth, m_image->nChannels));
            setVariant(dataTypeFromPixelType(pixelType()));
        }  
        
        void Image::getDataFromCvImage(const PixelType pixelType)
        {
            setBuffer((uint8_t*)(m_image->imageData));
            setSize(m_image->imageSize);
            initialize(m_image->width, m_image->height, m_image->widthStep, (uint8_t*)(m_image->imageData), pixelType);
        }
        
        void Image::resize(const unsigned int width, const unsigned int height, const core::Image::PixelType pixelType)
        {
            if(m_image)
                cvReleaseImage(&m_image);
            
            try
            {
                m_image = cvCreateImage(cv::Size(width, height), depth(pixelType), numChannels(pixelType));
                getDataFromCvImage(pixelType);
                setVariant(dataTypeFromPixelType(pixelType));
            }
            catch(cv::Exception& e)
            {
                throw core::OutOfMemory("Failed to create new image.");
            }
        }
        
        void Image::resize(const unsigned int size)
        {
            if(m_image)
                cvReleaseImage(&m_image);
            
            try
            {
                m_image = cvCreateImage(cv::Size(size, 1), 8, 1);
                
                getDataFromCvImage(pixelTypeFromParameters(m_image->depth, m_image->nChannels));
                setVariant(core::DataVariant::MONO_8_IMAGE);
            }
            catch(cv::Exception& e)
            {
                throw core::OutOfMemory("Failed to allocate image.");
            }
        }
        
        void Image::save(const std::string& filename) const
        {
            cv::Mat inImage = getOpenCvMat(*this);
            
            switch(pixelType())
            {
            case core::Image::RGB_24:
            {
                Image tempImage(width(), height(), BGR_24);
                cv::Mat cvTempImage(tempImage.m_image);
                
                cv::cvtColor(inImage, cvTempImage, CV_RGB2BGR); 
                    
                if(! cv::imwrite(filename, cv::Mat(tempImage.m_image)))
                    throw core::FileAccessFailed(filename, "Failed to save image.");
                
                break;
            }
            case core::Image::BGR_24:
            case core::Image::MONO_8:
            case core::Image::BAYERBG_8:
            case core::Image::BAYERGB_8:
            {
                if(! cv::imwrite(filename, inImage))
                    throw core::FileAccessFailed(filename, "Failed to save image.");
                break;
            }
            default:
                throw core::WrongArgument("Unknown pixel type.");    
            }         
        }
        
        Image::~Image()
        {
            cvReleaseImage(&m_image);
        }

        const core::DataVariant Image::dataTypeFromPixelType(const core::Image::PixelType pixelType)
        {
            switch(pixelType)
            {
            case core::Image::NONE:
                return core::DataVariant(core::DataVariant::IMAGE);
            case core::Image::MONO_8:
                return core::DataVariant(core::DataVariant::MONO_8_IMAGE);
            case core::Image::BAYERBG_8:
                return core::DataVariant(core::DataVariant::BAYERBG_8_IMAGE);
            case core::Image::BAYERGB_8:
                return core::DataVariant(core::DataVariant::BAYERGB_8_IMAGE);
            case core::Image::RGB_24:
                return core::DataVariant(core::DataVariant::RGB_24_IMAGE);
            case core::Image::BGR_24:
                return core::DataVariant(core::DataVariant::BGR_24_IMAGE);
            default:
                throw core::WrongArgument("Unknown pixel type.");  
            }
        }

        const core::Image::PixelType Image::pixelTypeFromParameters(const int depth, const int numChannels)
        {
            switch(depth)
            {
            case 8:
                switch(numChannels)
                {
                case 1:
                    return core::Image::MONO_8;
                case 3:
                    return core::Image::BGR_24;
                default:
                    throw core::WrongArgument("Unknown combination of depth and number of channels.");
                }
            case 16:
                switch(numChannels)
                {
                default:
                    throw core::WrongArgument("Unknown combination of depth and number of channels.");      
                }
            default:
                throw core::WrongArgument("Unknown combination of depth and number of channels.");    
            }         
        }
    }
}
