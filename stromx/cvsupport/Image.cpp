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
#include "stromx/cvsupport/Image.h"
#include "stromx/cvsupport/Utilities.h"
#include <stromx/runtime/Exception.h>
#include <stromx/runtime/InputProvider.h>
#include <stromx/runtime/OutputProvider.h>

namespace stromx
{
    namespace cvsupport
    {
        const std::string Image::TYPE = "Image";
        const std::string Image::PACKAGE = STROMX_CVSUPPORT_PACKAGE_NAME;
        const runtime::Version Image::VERSION = runtime::Version(0, 1, 0);
        
        Image::Image(const unsigned int width, const unsigned int height, const runtime::Image::PixelType pixelType)
          : m_image(new cv::Mat())
        {
            allocate(width, height, pixelType);
        }
        
        Image::Image(const stromx::runtime::Image& image)
          : m_image(new cv::Mat())
        {
            copy(image);
        }
        
        Image::Image(const stromx::cvsupport::Image& image)
          : runtime::ImageWrapper(),
            m_image(new cv::Mat())
        {
            copy(image);
        }
        
        Image::Image(const cv::Mat& cvImage)
          : m_image(new cv::Mat(cvImage))
        {
            getDataFromCvImage(pixelTypeFromCvType(m_image->type()));
        }
        
        Image::Image(const cv::Mat& cvImage, const Image::PixelType pixelType)
          : m_image(new cv::Mat(cvImage))
        {
            getDataFromCvImage(pixelType);
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
        
        Image::Image(const std::string& filename, const Conversion access)
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

        void Image::copy(const runtime::Image& image)
        {
            allocate(image.width(), image.height(), image.pixelType());
            
            try
            {
                cv::Mat cvInImage = getOpenCvMat(image);
                cvInImage.copyTo(*m_image);
            }
            catch(cv::Exception&)
            {
                throw stromx::runtime::Exception("Failed to copy construct image.");
            }
        }
        
        void Image::serialize(runtime::OutputProvider & output) const
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
                    throw runtime::Exception("Failed to encode image.");
            }
            catch(cv::Exception&)
            {
                throw stromx::runtime::Exception("Failed to encode image.");
            }
            
            std::ostream & outStream = output.openFile("png", runtime::OutputProvider::BINARY);
            for(std::vector<uchar>::const_iterator iter = data.begin(); 
                iter != data.end();
                ++iter)
            {
                outStream.put(*iter);
            }
        }

        void Image::deserialize(runtime::InputProvider & input, const runtime::Version &)
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
            
            input.openFile(runtime::InputProvider::BINARY);
            
            unsigned int dataSize = 0;
            const unsigned int CHUNK_SIZE = 100000;
            std::vector<uchar> data;
            uchar* dataPtr;
            while(! input.file().eof())
            {
                data.resize(data.size() + CHUNK_SIZE);
                dataPtr = &data[0] + dataSize;
                input.file().read(reinterpret_cast<char*>(dataPtr), CHUNK_SIZE);
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
                throw stromx::runtime::Exception("Failed to decode image.");
            }
        }
        
        void Image::open(const std::string & filename, const Conversion access)
        {
            int cvAccessType = getCvAccessType(access);
            
            *m_image = cv::imread(filename, cvAccessType);
            if(! m_image->data)
                throw runtime::FileAccessFailed(filename, "Failed to load image.");
            
            if(access & DEPTH_16 && m_image->depth() != CV_16U)
            {
                if(m_image->depth() != CV_8U)
                    throw runtime::Exception("Only 8-bit images can be converted to 16-bit.");
                
                if(m_image->channels() != 1 && m_image->channels() != 3)
                {
                    throw runtime::Exception(
                        "Only images with one or three channels can be converted to 16-bit.");
                }
                
                // convert the image to a 16-bit image
                int type = m_image->channels() == 1 ? CV_16UC1 : CV_16UC3;
                cv::Mat image16Bit(m_image->rows, m_image->cols, type);
                m_image->convertTo(image16Bit, type, 256);
                
                // remember the 16-bit image
                *m_image = image16Bit;
            }
                
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
        
        void Image::save(const std::string& filename, const runtime::Image & image)
        {
            cv::Mat inImage = getOpenCvMat(image);
            
            switch(image.pixelType())
            {
            case runtime::Image::RGB_24:
            {
                Image tempImage(image.height(), image.width(), BGR_24);
                cv::cvtColor(inImage, *(tempImage.m_image), CV_RGB2BGR); 
                if(! cv::imwrite(filename.c_str(), *(tempImage.m_image)))
                    throw runtime::FileAccessFailed(filename, "Failed to save image.");
                break;
            }
            case runtime::Image::RGB_48:
            {
                cv::Mat tempImage;
                cv::cvtColor(inImage, tempImage, CV_RGB2BGR); 
                if(! cv::imwrite(filename.c_str(), tempImage))
                    throw runtime::FileAccessFailed(filename, "Failed to save image.");
                break;
            }
            case runtime::Image::BGR_24:
            case runtime::Image::MONO_8:
            case runtime::Image::BAYERBG_8:
            case runtime::Image::BAYERGB_8:
            case runtime::Image::MONO_16:
            case runtime::Image::BGR_48:
            {
                if(! cv::imwrite(filename, inImage))
                    throw runtime::FileAccessFailed(filename, "Failed to save image.");
                break;
            }
            default:
                throw runtime::WrongArgument("Unknown pixel type.");    
            }
        }
        
        void Image::save(const std::string& filename) const
        {
            save(filename, *this);
        }
        
        runtime::Data* Image::clone() const
        {
            return new Image(*this);
        }
        
        void Image::initializeImage(const unsigned int width, const unsigned int height, 
                                    const unsigned int stride, uint8_t* data, const PixelType pixelType)
        {
            ImageWrapper::initializeImage(width, height, stride, data, pixelType);
            int type = cvTypeFromPixelType(pixelType);
            m_image->rows = height;
            m_image->cols = width;
            m_image->data = data;
            m_image->size.p[0] = height;
            m_image->size.p[1] = width;
            m_image->step.buf[0] = stride;
            m_image->step.buf[1] = pixelSize();
            m_image->step.p[0] = stride;
            m_image->step.p[1] = pixelSize();
            m_image->flags = (type & CV_MAT_TYPE_MASK) | cv::Mat::MAGIC_VAL;
            
            if (pixelSize() * width == stride || height == 1)
                m_image->flags |= cv::Mat::CONTINUOUS_FLAG;
            else
                m_image->flags &= ~cv::Mat::CONTINUOUS_FLAG;
        }
        
        void Image::initializeMatrix(const unsigned int rows, const unsigned int cols, 
          const unsigned int stride, uint8_t*const data, 
          const stromx::runtime::Matrix::ValueType valueType)
        {
            ImageWrapper::initializeMatrix(rows, cols, stride, data, valueType);
            int type = cvTypeFromValueType(valueType);
            m_image->rows = rows;
            m_image->cols = cols;
            m_image->data = data;
            m_image->size.p[0] = rows;
            m_image->size.p[1] = cols;
            m_image->step.buf[0] = stride;
            m_image->step.buf[1] = valueSize();
            m_image->step.p[0] = stride;
            m_image->step.p[1] = valueSize();
            m_image->flags = (type & CV_MAT_TYPE_MASK) | cv::Mat::MAGIC_VAL;
            
            if (valueSize() * cols == stride || rows == 1)
                m_image->flags |= cv::Mat::CONTINUOUS_FLAG;
            else
                m_image->flags &= ~cv::Mat::CONTINUOUS_FLAG;
        }

        void Image::allocate(const unsigned int width, const unsigned int height, const Image::PixelType pixelType)
        {
            try
            {
                m_image->create(height, width, cvTypeFromPixelType(pixelType));
                getDataFromCvImage(pixelType);
            }
            catch(cv::Exception&)
            {
                throw runtime::OutOfMemory("Failed to allocate image.");
            }
        }
        
        int Image::cvTypeFromPixelType(const runtime::Image::PixelType pixelType)
        {
            switch(pixelType)
            {
            case runtime::Image::NONE:
            case runtime::Image::MONO_8:
            case runtime::Image::BAYERBG_8:
            case runtime::Image::BAYERGB_8:
                return CV_8UC1;
            case runtime::Image::RGB_24:
            case runtime::Image::BGR_24:
                return CV_8UC3;
            case runtime::Image::MONO_16:
            case runtime::Image::BAYERBG_16:
            case runtime::Image::BAYERGB_16:
                return CV_16UC1;
            case runtime::Image::RGB_48:
            case runtime::Image::BGR_48:
                return CV_16UC3;
            default:
                throw runtime::WrongArgument("Unknown pixel type.");  
            }
        }
        
        int Image::cvTypeFromValueType(const runtime::Matrix::ValueType valueType)
        {
            switch(valueType)
            {
            case runtime::Matrix::INT_8:
                return CV_8SC1;
            case runtime::Matrix::UINT_8:
                return CV_8UC1;
            case runtime::Matrix::INT_16:
                return CV_16SC1;
            case runtime::Matrix::UINT_16:
                return CV_16UC1;
            case runtime::Matrix::INT_32:
                return CV_32FC1;
            case runtime::Matrix::UINT_32:
                return CV_32FC1;
            case runtime::Matrix::FLOAT_32:
                return CV_32FC1;
            case runtime::Matrix::FLOAT_64:
                return CV_64FC1;
            default:
                throw runtime::WrongArgument("Unknown value type.");  
            }
        }
        
        stromx::runtime::Image::PixelType Image::pixelTypeFromCvType(const int cvType)
        {
            switch(cvType)
            {
            case CV_8UC1:
                return runtime::Image::MONO_8;
            case CV_8UC3:
                return runtime::Image::BGR_24;
            case CV_16UC1:
                return runtime::Image::MONO_16;
            case CV_16UC3:
                return runtime::Image::BGR_48;
            default:
                throw runtime::WrongArgument("Unknown OpenCV pixel type.");  
            }
        }

        runtime::Image::PixelType Image::pixelTypeFromParameters(const int depth, const int numChannels)
        {
            switch(depth)
            {
            case 8:
                switch(numChannels)
                {
                case 1:
                    return runtime::Image::MONO_8;
                case 3:
                    return runtime::Image::BGR_24;
                default:
                    throw runtime::WrongArgument("Unknown combination of depth and number of channels.");
                }
            case 16:
                throw runtime::WrongArgument("Unknown combination of depth and number of channels.");      
            default:
                throw runtime::WrongArgument("Unknown combination of depth and number of channels.");    
            }         
        }
        
        int Image::getCvAccessType(const stromx::cvsupport::Image::Conversion access)
        {
            switch(access & (GRAYSCALE | COLOR))
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
