#include "Image.h"

#include "Config.h"
#include "Utilities.h"

#include <strom/Exception.h>

namespace base
{
    const std::string Image::TYPE = "Image";
    const std::string Image::PACKAGE = PACKAGE_NAME;
    const strom::Version Image::VERSION = strom::Version(BASE_VERSION_MAJOR, BASE_VERSION_MAJOR);
    
    Image::Image(const unsigned int width, const unsigned int height, const strom::Image::PixelType pixelType)
    {
        try
        {
            m_image = cvCreateImage(cv::Size(width, height), depth(pixelType) * 8, numChannels(pixelType));
            
            getDataFromCvImage(pixelType);
            setVariant(dataTypeFromPixelType(pixelType));
        }
        catch(cv::Exception& e)
        {
            throw strom::OutOfMemory("Failed to create new image.");
        }
    }
    
    Image::Image(const strom::Image& image)
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
            throw strom::OutOfMemory("Failed to create new image.");
        }
        
        cv::Mat cvInImage = getOpenCvMat(image);
        cv::Mat cvImage(m_image);
        cvInImage.copyTo(cvImage);
    }
    
    Image::Image()
      : m_image(0)
    {
        setSize(0);
        initialize(0, 0, 0, 0, strom::Image::NONE);
        setVariant(strom::DataVariant::IMAGE);
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
            
            getDataFromCvImage(strom::Image::MONO_8);
            setVariant(strom::DataVariant::IMAGE);
        }
        catch(cv::Exception& e)
        {
            throw strom::OutOfMemory("Failed to allocate image.");
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
        catch(strom::Exception&)
        {
            throw strom::SerializationError(*this, name, path);
        }
    }

    void Image::deserialize(const std::string& data, const std::string& path)
    {
        try
        {
            std::string filepath = path + data;
            open(filepath);
        }
        catch(strom::Exception&)
        {
            throw strom::DeserializationError(*this, data, path);
        }
    }

    void Image::open(const std::string& filename)
    {
        if(m_image)
            cvReleaseImage(&m_image);
        
        m_image = cvLoadImage(filename.c_str());
        
        if(! m_image)
            throw strom::FileAccessFailed(filename, "Failed to load image.");
            
        getDataFromCvImage(pixelTypeFromParameters(m_image->depth, m_image->nChannels));
        setVariant(dataTypeFromPixelType(pixelType()));
    }  
    
    void Image::getDataFromCvImage(const PixelType pixelType)
    {
        setBuffer((uint8_t*)(m_image->imageData));
        setSize(m_image->imageSize);
        initialize(m_image->width, m_image->height, m_image->widthStep, (uint8_t*)(m_image->imageData), pixelType);
    }
    
    void Image::resize(const unsigned int width, const unsigned int height, const strom::Image::PixelType pixelType)
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
            throw strom::OutOfMemory("Failed to create new image.");
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
            setVariant(strom::DataVariant::MONO_8_IMAGE);
        }
        catch(cv::Exception& e)
        {
            throw strom::OutOfMemory("Failed to allocate image.");
        }
    }
    
    void Image::save(const std::string& filename) const
    {
        cv::Mat inImage = getOpenCvMat(*this);
        
        switch(pixelType())
        {
        case strom::Image::RGB_24:
        {
            Image tempImage(width(), height(), BGR_24);
            cv::Mat cvTempImage(tempImage.m_image);
            
            cv::cvtColor(inImage, cvTempImage, CV_RGB2BGR); 
                  
            if(! cv::imwrite(filename, tempImage.m_image))
                throw strom::FileAccessFailed(filename, "Failed to save image.");
                
            break;
        }
        case strom::Image::BGR_24:
        case strom::Image::MONO_8:
        case strom::Image::BAYERBG_8:
        case strom::Image::BAYERGB_8:
        {
            if(! cv::imwrite(filename, inImage))
                throw strom::FileAccessFailed(filename, "Failed to save image.");
            break;
        }
        default:
            throw strom::WrongArgument("Unknown pixel type.");    
        }         
    }
    
    Image::~Image()
    {
        cvReleaseImage(&m_image);
    }

    const strom::DataVariant Image::dataTypeFromPixelType(const strom::Image::PixelType pixelType)
    {
        switch(pixelType)
        {
        case strom::Image::NONE:
            return strom::DataVariant(strom::DataVariant::IMAGE);
        case strom::Image::MONO_8:
            return strom::DataVariant(strom::DataVariant::MONO_8_IMAGE);
        case strom::Image::BAYERBG_8:
            return strom::DataVariant(strom::DataVariant::BAYERBG_8_IMAGE);
        case strom::Image::BAYERGB_8:
            return strom::DataVariant(strom::DataVariant::BAYERGB_8_IMAGE);
        case strom::Image::RGB_24:
            return strom::DataVariant(strom::DataVariant::RGB_24_IMAGE);
        case strom::Image::BGR_24:
            return strom::DataVariant(strom::DataVariant::BGR_24_IMAGE);
        default:
            throw strom::WrongArgument("Unknown pixel type.");  
        }
    }

    const strom::Image::PixelType Image::pixelTypeFromParameters(const int depth, const int numChannels)
    {
        switch(depth)
        {
        case 8:
            switch(numChannels)
            {
            case 1:
                return strom::Image::MONO_8;
            case 3:
                return strom::Image::BGR_24;
            default:
                throw strom::WrongArgument("Unknown combination of depth and number of channels.");
            }
        case 16:
            switch(numChannels)
            {
            default:
                throw strom::WrongArgument("Unknown combination of depth and number of channels.");      
            }
        default:
            throw strom::WrongArgument("Unknown combination of depth and number of channels.");    
        }         
    }
}