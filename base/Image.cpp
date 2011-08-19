#include "Image.h"

#include "Config.h"
#include "Utilities.h"

#include <stream/Exception.h>

namespace base
{
    const std::string Image::NAME = "Image";
    const std::string Image::PACKAGE = PACKAGE_NAME;
    const stream::Version Image::VERSION = stream::Version(BASE_VERSION_MAJOR, BASE_VERSION_MAJOR);
    
    Image::Image(const unsigned int width, const unsigned int height, const stream::Image::PixelType pixelType)
    {
        try
        {
            m_image = cvCreateImage(cv::Size(width, height), depth(pixelType) * 8, numChannels(pixelType));
            
            getDataFromCvImage(pixelType);
            setDataType(dataTypeFromPixelType(pixelType));
        }
        catch(cv::Exception& e)
        {
            throw stream::OutOfMemory("Failed to create new image.");
        }
    }
    
    Image::Image(const stream::Image& image)
    {
        try
        {
            m_image = cvCreateImage(cv::Size(image.width(), image.height()),
                                    depth(image.pixelType()) * 8, numChannels(image.pixelType()));
            
            getDataFromCvImage(image.pixelType());
            setDataType(image.type());
        }
        catch(cv::Exception& e)
        {
            throw stream::OutOfMemory("Failed to create new image.");
        }
        
        cv::Mat cvInImage = getOpenCvMat(image);
        cv::Mat cvImage(m_image);
        cvInImage.copyTo(cvImage);
    }
    
    Image::Image()
      : m_image(0)
    {
        setSize(0);
        initialize(0, 0, 0, 0, stream::Image::NONE);
        setDataType(stream::DataType::IMAGE);
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
            
            getDataFromCvImage(stream::Image::MONO_8);
            setDataType(stream::DataType::IMAGE);
        }
        catch(cv::Exception& e)
        {
            throw stream::OutOfMemory("Failed to allocate image.");
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
        catch(stream::Exception&)
        {
            throw stream::SerializationError(*this, name, path);
        }
    }

    void Image::deserialize(const std::string& data, const std::string& path)
    {
        try
        {
            std::string filepath = path + data;
            open(filepath);
        }
        catch(stream::Exception&)
        {
            throw stream::DeserializationError(*this, data, path);
        }
    }

    void Image::open(const std::string& filename)
    {
        if(m_image)
            cvReleaseImage(&m_image);
        
        m_image = cvLoadImage(filename.c_str());
        
        if(! m_image)
            throw stream::FileAccessFailed("Failed to load image '" + filename + "'.");
            
        getDataFromCvImage(pixelTypeFromParameters(m_image->depth, m_image->nChannels));
        setDataType(dataTypeFromPixelType(pixelType()));
    }  
    
    void Image::getDataFromCvImage(const PixelType pixelType)
    {
        setBuffer((uint8_t*)(m_image->imageData));
        setSize(m_image->imageSize);
        initialize(m_image->width, m_image->height, m_image->widthStep, (uint8_t*)(m_image->imageData), pixelType);
    }
    
    void Image::resize(const unsigned int width, const unsigned int height, const stream::Image::PixelType pixelType)
    {
        if(m_image)
            cvReleaseImage(&m_image);
        
        try
        {
            m_image = cvCreateImage(cv::Size(width, height), depth(pixelType), numChannels(pixelType));
            getDataFromCvImage(pixelType);
            setDataType(dataTypeFromPixelType(pixelType));
        }
        catch(cv::Exception& e)
        {
            throw stream::OutOfMemory("Failed to create new image.");
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
            setDataType(stream::DataType::MONO_8_IMAGE);
        }
        catch(cv::Exception& e)
        {
            throw stream::OutOfMemory("Failed to allocate image.");
        }
    }
    
    void Image::save(const std::string& filename) const
    {
        cv::Mat inImage = getOpenCvMat(*this);
        
        switch(pixelType())
        {
        case stream::Image::RGB_24:
        {
            Image tempImage(width(), height(), BGR_24);
            cv::Mat cvTempImage(tempImage.m_image);
            
            cv::cvtColor(inImage, cvTempImage, CV_RGB2BGR); 
                  
            if(! cv::imwrite(filename, tempImage.m_image))
                throw stream::FileAccessFailed("Failed to save image '" + filename + "'.");
                
            break;
        }
        case stream::Image::BGR_24:
        case stream::Image::MONO_8:
        case stream::Image::BAYERBG_8:
        case stream::Image::BAYERGB_8:
        {
            if(! cv::imwrite(filename, inImage))
                throw stream::FileAccessFailed("Failed to save image '" + filename + "'.");
            break;
        }
        default:
            throw stream::WrongArgument("Unknown pixel type.");    
        }         
    }
    
    Image::~Image()
    {
        cvReleaseImage(&m_image);
    }

    const stream::DataType Image::dataTypeFromPixelType(const stream::Image::PixelType pixelType)
    {
        switch(pixelType)
        {
        case stream::Image::NONE:
            return stream::DataType(stream::DataType::IMAGE);
        case stream::Image::MONO_8:
            return stream::DataType(stream::DataType::MONO_8_IMAGE);
        case stream::Image::BAYERBG_8:
            return stream::DataType(stream::DataType::BAYERBG_8_IMAGE);
        case stream::Image::BAYERGB_8:
            return stream::DataType(stream::DataType::BAYERGB_8_IMAGE);
        case stream::Image::RGB_24:
            return stream::DataType(stream::DataType::RGB_24_IMAGE);
        case stream::Image::BGR_24:
            return stream::DataType(stream::DataType::BGR_24_IMAGE);
        default:
            throw stream::WrongArgument("Unknown pixel type.");  
        }
    }

    const stream::Image::PixelType Image::pixelTypeFromParameters(const int depth, const int numChannels)
    {
        switch(depth)
        {
        case 8:
            switch(numChannels)
            {
            case 1:
                return stream::Image::MONO_8;
            case 3:
                return stream::Image::BGR_24;
            default:
                throw stream::WrongArgument("Unknown combination of depth and number of channels.");
            }
        case 16:
            switch(numChannels)
            {
            default:
                throw stream::WrongArgument("Unknown combination of depth and number of channels.");      
            }
        default:
            throw stream::WrongArgument("Unknown combination of depth and number of channels.");    
        }         
    }
}