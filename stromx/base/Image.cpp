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

#include <boost/assert.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
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
          : m_image(new cv::Mat())
        {
            allocate(width, height, pixelType);
        }
        
        Image::Image(const core::Image& image)
          : m_image(new cv::Mat())
        {
            copy(image);
        }
        
        Image::Image(const stromx::base::Image& image)
          : m_image(new cv::Mat())
        {
            copy(image);
        }
        
        Image::Image()
          : m_image(new cv::Mat())
        {
            allocate(0, 0, NONE);
        }
        
        Image::Image(const std::string& filename)
          : m_image(new cv::Mat())
        {
            open(filename);
        }
        
        Image::Image(const std::string& filename, const FileAccess access)
          : m_image(new cv::Mat())
        {
            open(filename, access);
        }
        
        Image::Image(const unsigned int size)
          : m_image(new cv::Mat())
        {
            allocate(size, 1, MONO_8);
        } 
        
        Image::~Image()
        {
            delete m_image;
        }

        void Image::copy(const core::Image& image)
        {
            allocate(image.width(), image.height(), image.pixelType());
            
            try
            {
                cv::Mat cvInImage = getOpenCvMat(image);
                cvInImage.copyTo(*m_image);
            }
            catch(cv::Exception&)
            {
                throw stromx::core::Exception("Failed to copy construct image.");
            }
        }
        
        void Image::serialize(core::OutputProvider & output) const
        {
            output.text() << (unsigned int)(pixelType()) << " ";
            output.text() << width() << " ";
            output.text() << height();
            
            if(width() == 0 || height() == 0)
                return;
            
            std::vector<uchar> data;
            try
            {
                if(! cv::imencode(".png", *m_image, data))
                    throw core::Exception("Failed to encode image.");
            }
            catch(cv::Exception&)
            {
                throw stromx::core::Exception("Failed to encode image.");
            }
            
            std::ostream & outStream = output.openFile("png", core::OutputProvider::BINARY);
            for(std::vector<uchar>::const_iterator iter = data.begin(); 
                iter != data.end();
                ++iter)
            {
                outStream.put(*iter);
            }
        }

        void Image::deserialize(core::InputProvider & input, const core::Version & version)
        {
            int width = -1;
            int height = -1;
            int type = -1;
            
            input.text() >> type;
            input.text() >> width;
            input.text() >> height;
            
            if(width == 0 || height == 0)
            {
                allocate(width, height, PixelType(type));
                return;
            }
            
            input.openFile(core::InputProvider::BINARY);
            
            unsigned int dataSize = 0;
            const unsigned int CHUNK_SIZE = 100000;
            std::vector<char> data;
            char* dataPtr;
            while(! input.file().eof())
            {
                data.resize(data.size() + CHUNK_SIZE);
                dataPtr = &data[0] + dataSize;
                input.file().read(dataPtr, CHUNK_SIZE);
                dataSize += (unsigned int)(input.file().gcount());
            }
            data.resize(dataSize);
            
            try
            {
                cv::Mat buffer(data);
                *m_image = cv::imdecode(buffer, -1);
                PixelType pixelType = pixelTypeFromCvType(m_image->type());
                
                if(type > 0)
                    pixelType = PixelType(type);
                
                getDataFromCvImage(pixelType);
            }
            catch(cv::Exception &)
            {
                throw stromx::core::Exception("Failed to decode image.");
            }
        }
        
        void Image::open(const std::string & filename, const FileAccess access)
        {
            int cvAccessType = getCvAccessType(access);
            
            *m_image = cv::imread(filename, cvAccessType);
            if(! m_image->data)
                throw core::FileAccessFailed(filename, "Failed to load image.");
                
            getDataFromCvImage(pixelTypeFromCvType(m_image->type()));
        } 

        void Image::open(const std::string& filename)
        {
            open(filename, UNCHANGED);
        }  
        
        void Image::getDataFromCvImage(const PixelType pixelType)
        {
            setBuffer((uint8_t*)(m_image->data), m_image->step * m_image->rows);
            initializeImage(m_image->cols, m_image->rows, m_image->step, (uint8_t*)(m_image->data), pixelType);
        }
        
        void Image::resize(const unsigned int width, const unsigned int height, const core::Image::PixelType pixelType)
        {
            allocate(width, height, pixelType);
        }
        
        void Image::resize(const unsigned int size)
        {
            allocate(size, 1, core::Image::NONE);
        }
        
        void Image::save(const std::string& filename) const
        {
            cv::Mat inImage = getOpenCvMat(*this);
            
            switch(pixelType())
            {
            case core::Image::RGB_24:
            {
                Image tempImage(height(), width(), BGR_24);
                cv::cvtColor(inImage, *(tempImage.m_image), CV_RGB2BGR); 
                if(! cv::imwrite(filename.c_str(), *(tempImage.m_image)))
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
        
        core::Data*const Image::clone() const
        {
            return new Image(*this);
        }
        
        void Image::allocate(const unsigned int width, const unsigned int height, const Image::PixelType pixelType)
        {
            try
            {
                *m_image = cv::Mat(height, width, cvTypeFromPixelType(pixelType));
                getDataFromCvImage(pixelType);
            }
            catch(cv::Exception&)
            {
                throw core::OutOfMemory("Failed to allocate image.");
            }
        }
        
        const int Image::cvTypeFromPixelType(const core::Image::PixelType pixelType)
        {
            switch(pixelType)
            {
            case core::Image::NONE:
            case core::Image::MONO_8:
            case core::Image::BAYERBG_8:
            case core::Image::BAYERGB_8:
                return CV_8UC1;
            case core::Image::RGB_24:
            case core::Image::BGR_24:
                return CV_8UC3;
            default:
                throw core::WrongArgument("Unknown pixel type.");  
            }
        }
        
        const stromx::core::Image::PixelType Image::pixelTypeFromCvType(const int cvType)
        {
            switch(cvType)
            {
            case CV_8UC1:
                return core::Image::MONO_8;
            case CV_8UC3:
                return core::Image::BGR_24;
            default:
                throw core::WrongArgument("Unknown OpenCV element type.");  
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
    }
}
