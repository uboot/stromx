/* 
*  Copyright 2011 Matthias Fuchs
*
*  Licensed under the Apache License, Version 2.0 (the "License");
*  you may not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS,
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
*/

#include <opencv2/opencv.hpp>
#include <boost/assert.hpp>
#include "Image.h"
#include "Utilities.h"
#include <stromx/core/Exception.h>
#include <stromx/core/InputProvider.h>
#include <stromx/core/OutputProvider.h>

namespace stromx
{
    namespace base
    {
        const std::string Image::TYPE = "Image";
        const std::string Image::PACKAGE = STROMX_BASE_PACKAGE_NAME;
        const core::Version Image::VERSION = core::Version(BASE_VERSION_MAJOR, BASE_VERSION_MINOR, BASE_VERSION_PATCH);
        
        Image::Image(const unsigned int width, const unsigned int height, const core::Image::PixelType pixelType)
          : m_image(0),
            m_isHeader(false),
            m_matImage(0)
        {
            try
            {
                m_image = cvCreateImage(cv::Size(width, height), depth(pixelType) * 8, numChannels(pixelType));
                
                getDataFromCvImage(pixelType);
                setVariant(dataTypeFromPixelType(pixelType));
            }
            catch(cv::Exception&)
            {
                throw core::OutOfMemory("Failed to create new image.");
            }
        }
        
        Image::Image(const core::Image& image)
          : m_image(0),
            m_isHeader(false),
            m_matImage(0)
        {
            try
            {
                m_image = cvCreateImage(cv::Size(image.width(), image.height()),
                                        depth(image.pixelType()) * 8, numChannels(image.pixelType()));
                
                getDataFromCvImage(image.pixelType());
                setVariant(image.variant());
            }
            catch(cv::Exception&)
            {
                throw core::OutOfMemory("Failed to create new image.");
            }
            
            cv::Mat cvInImage = getOpenCvMat(image);
            cv::Mat cvImage(m_image);
            cvInImage.copyTo(cvImage);
        }
        
        Image::Image()
          : m_image(0),
            m_isHeader(false),
            m_matImage(0)
        {
            setBufferSize(0);
            initialize(0, 0, 0, 0, core::Image::NONE);
            setVariant(core::DataVariant::IMAGE);
        }
        
        Image::Image(const std::string& filename)
          : m_image(0),
            m_isHeader(false),
            m_matImage(0)
        {
            open(filename);
        }
        
        Image::Image(const std::string& filename, const FileAccess access)
          : m_image(0),
            m_isHeader(false),
            m_matImage(0)
        {
            open(filename, access);
        }
        
        Image::Image(const unsigned int size)
          : m_image(0),
            m_isHeader(false),
            m_matImage(0)
        {
            try
            {
                m_image = cvCreateImage(cv::Size(size, 1), 8, 1);
                
                getDataFromCvImage(core::Image::MONO_8);
                setVariant(core::DataVariant::IMAGE);
            }
            catch(cv::Exception&)
            {
                throw core::OutOfMemory("Failed to allocate image.");
            }
        } 
        
        void Image::serialize(core::OutputProvider & output) const
        {
            if(width() == 0 || height() == 0)
                return;
            
            std::vector<uchar> data;
            if(! cv::imencode(".png", cv::Mat(m_image), data))
                throw core::Exception("Failed to encode image.");
            
            std::ostream & outStream = output.openFile("png", core::OutputProvider::BINARY);
            for(std::vector<uchar>::const_iterator iter = data.begin(); 
                iter != data.end();
                ++iter)
            {
                outStream.put(*iter);
            }
        }

        void Image::deserialize(core::InputProvider & input)
        {
            releaseImage();
            
            if(! input.hasFile())
            {
                setBufferSize(0);
                initialize(0, 0, 0, 0, core::Image::NONE);
                setVariant(core::DataVariant::IMAGE);
                
                return;
            }
            
            input.openFile(core::InputProvider::BINARY);
            
            unsigned int dataSize = 0;
            const unsigned int CHUNK_SIZE = 100000;
            std::vector<char> data;
            while(! input.file().eof())
            {
                data.resize(data.size() + CHUNK_SIZE);
                input.file().read(&data[0], CHUNK_SIZE);
                dataSize += (unsigned int)(input.file().gcount());
            }
            
            cv::Mat buffer(data);
            m_matImage = new cv::Mat(cv::imdecode(buffer, -1));
            
            // construct an IplImage header
            m_image = new IplImage(*m_matImage);
            m_isHeader = true;
            
            getDataFromCvImage(pixelTypeFromParameters(m_image->depth, m_image->nChannels));
            setVariant(dataTypeFromPixelType(pixelType()));
        }
        
        void Image::open(const std::string & filename, const FileAccess access)
        {
            releaseImage();
            
            int cvAccessType = getCvAccessType(access);
            m_image = cvLoadImage(filename.c_str(), cvAccessType);
            
            if(! m_image)
                throw core::FileAccessFailed(filename, "Failed to load image.");
                
            getDataFromCvImage(pixelTypeFromParameters(m_image->depth, m_image->nChannels));
            setVariant(dataTypeFromPixelType(pixelType()));
        } 

        void Image::open(const std::string& filename)
        {
            open(filename, UNCHANGED);
        }  
        
        void Image::getDataFromCvImage(const PixelType pixelType)
        {
            setBuffer((uint8_t*)(m_image->imageData));
            setBufferSize(m_image->imageSize);
            initialize(m_image->width, m_image->height, m_image->widthStep, (uint8_t*)(m_image->imageData), pixelType);
        }
        
        void Image::resize(const unsigned int width, const unsigned int height, const core::Image::PixelType pixelType)
        {
            releaseImage();
            
            try
            {
                m_image = cvCreateImage(cv::Size(width, height), depth(pixelType) * 8, numChannels(pixelType));
                getDataFromCvImage(pixelType);
                setVariant(dataTypeFromPixelType(pixelType));
            }
            catch(cv::Exception&)
            {
                throw core::OutOfMemory("Failed to create new image.");
            }
        }
        
        void Image::resize(const unsigned int size)
        {
            releaseImage();
            
            try
            {
                m_image = cvCreateImage(cv::Size(size, 1), 8, 1);
                
                getDataFromCvImage(pixelTypeFromParameters(m_image->depth, m_image->nChannels));
                setVariant(core::DataVariant::MONO_8_IMAGE);
            }
            catch(cv::Exception&)
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
                
                IplImage iplTempImg(cvTempImage);
                if(! cvSaveImage(filename.c_str(), &iplTempImg))
                    throw core::FileAccessFailed(filename, "Failed to save image.");
                
                break;
            }
            case core::Image::BGR_24:
            case core::Image::MONO_8:
            case core::Image::BAYERBG_8:
            case core::Image::BAYERGB_8:
            {
                IplImage iplTempImg(inImage);
                if(! cvSaveImage(filename.c_str(), &iplTempImg))
                    throw core::FileAccessFailed(filename, "Failed to save image.");
                break;
            }
            default:
                throw core::WrongArgument("Unknown pixel type.");    
            }         
        }
        
        Image::~Image()
        {
            releaseImage();
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
                throw core::WrongArgument("Unknown combination of depth and number of channels.");      
            default:
                throw core::WrongArgument("Unknown combination of depth and number of channels.");    
            }         
        }
        
        const int Image::getCvAccessType(const stromx::base::Image::FileAccess access)
        {
            switch(access)
            {
                case UNCHANGED:
                    return CV_LOAD_IMAGE_UNCHANGED;
                case GRAYSCALE:
                    return CV_LOAD_IMAGE_GRAYSCALE;
                case COLOR:
                    return CV_LOAD_IMAGE_COLOR;
                default:
                    BOOST_ASSERT(false);
                    return CV_LOAD_IMAGE_UNCHANGED;
            }
        }
        
        void Image::releaseImage()
        {
            if(m_isHeader)
                delete m_image;
            else
                cvReleaseImage(&m_image);
            
            if(m_matImage)
                delete m_matImage;
            
            m_isHeader = false;
        }
    }
}
