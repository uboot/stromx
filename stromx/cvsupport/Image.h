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

#ifndef STROMX_CVSUPPORT_IMAGE_H
#define STROMX_CVSUPPORT_IMAGE_H

#include <string>
#include "stromx/cvsupport/Config.h"
#include <stromx/runtime/ImageWrapper.h>

namespace cv
{
    class Mat;
}

namespace stromx
{
    namespace cvsupport
    {
        /** \brief %Image with support for reading and writing. */
        class STROMX_CVSUPPORT_API Image : public runtime::ImageWrapper
        {
             friend STROMX_CVSUPPORT_API cv::Mat getOpenCvMat(const runtime::Image& image);
             
        public:
            /** Conversion options when reading images. */
            enum Conversion
            {
                /** The image data is not converted. */
                UNCHANGED,
                
                /**
                 * The image data is converted to grayscale. I.e. the resulting image
                 * is always a single-channel image.
                 */
                GRAYSCALE = 0x2,
                
                /**
                 * The image data is converted to color. I.e. the resulting image is always 
                 * 3-channel image.
                 */
                COLOR = 0x4,
                
                /**
                 * The image data is converted to 16-bits per channel. I.e. the resulting
                 * image is always 16-bit or 48-bit image. If an 8-bit image is converted
                 * to 16-bit its pixel values are multiplied by 256.
                 */
                DEPTH_16 = 0x8
            };
            
            /** 
             * Saves the input \c image to \c filename. The file format is automatically 
             * determined from the file name. 
             */
            static void save(const std::string& filename, const runtime::Image & image);
            
            /** 
             * Constructs an empty image. The image width and height is 0 and no data is associated
             * with the image, i.e. data and buffer are 0. The pixel type is Image::NONE.
             */
            Image();
            
            /** Constructs an image for the given values. */
            explicit Image(const unsigned int width, const unsigned int height, const PixelType pixelType);
            
            /** Allocates an image and reads its content from the image file \c filename. */
            explicit Image(const std::string & filename);
            
            /** 
             * Allocates an image and reads its content from the image file \c filename. 
             * The image data is converted according to \c access. 
             */
            explicit Image(const std::string & filename, const Conversion access);
                        
            /** Copy constructs an image from \c image. */
            explicit Image(const stromx::runtime::Image& image);
            
            /** Copy constructs an image from \c image. */
            explicit Image(const stromx::cvsupport::Image& image);
            
            /** 
             * Creates an image from \c cvImage. The input image is \em not copied but 
             * a reference to it is held in the constructed image. Use this to convert an
             * OpenCV matrix to an stromx::cvsupport::Image object.
             */
            explicit Image(const cv::Mat & cvImage);
            
            /** 
             * Creates an image from \c cvImage. The input image is \em not copied but 
             * a reference to it is held in the constructed image. Use this to convert an
             * OpenCV matrix to an stromx::cvsupport::Image object. The pixel type of the 
             * resulting image is set to \c pixelType.
             * 
             * \throws WrongArgument If the input \c pixelType can not be set, i.e.
             *                       it would imply an incompatible pixel size in bytes.
             */
            explicit Image(const cv::Mat & cvImage, const PixelType pixelType);
            
            /** Allocates an image with a buffer of a given size in bytes. The width of the image is \c size
             * and its height is 1. Its pixel type is Image::NONE. The buffer size of the image is 
             * guaranteed to be at least \c size.
             */
            explicit Image(const unsigned int size);
            
            virtual ~Image();
            
            virtual const runtime::Version & version() const { return VERSION; }
            virtual const std::string & type() const { return TYPE; }
            virtual const std::string & package() const { return PACKAGE; }
            
            virtual Data* clone() const;
            
            virtual void serialize(runtime::OutputProvider & output) const;
            virtual void deserialize(runtime::InputProvider & input, const stromx::runtime::Version & version);
            
            virtual void initializeImage(const unsigned int width, 
                                         const unsigned int height, 
                                         const unsigned int stride, 
                                         uint8_t* data, 
                                         const PixelType pixelType);
                                         
            virtual void initializeMatrix(const unsigned int rows, 
                                          const unsigned int cols, 
                                          const unsigned int stride, 
                                          uint8_t* data, 
                                          const ValueType valueType);
            
            /** 
             * Reads the image \c filename. The data of the current image is replaced 
             * by the data of the new image.
             */ 
            void open(const std::string& filename);
            
            /** 
             * Reads the image \c filename. The data of the current image is replaced 
             * by the data of the new image. The read image data is converted according to \c access. 
             */ 
            void open(const std::string & filename, const Conversion access);
            
            /** 
             * Saves the image to the file \c filename. The file format is automatically 
             * determined from the file name.
             */
            void save(const std::string& filename) const;
            
        private:
            static const std::string TYPE;
            static const std::string PACKAGE;
            static const runtime::Version VERSION;
            
            static PixelType pixelTypeFromParameters(const int depth, const int numChannels);
            static int getCvAccessType(const Conversion access);
            static int cvTypeFromPixelType(const runtime::Image::PixelType pixelType);
            static int cvTypeFromValueType(const runtime::Matrix::ValueType valueType);
            static runtime::Image::PixelType pixelTypeFromCvType(const int cvType);
            
            void getDataFromCvImage(const PixelType pixelType);
            void copy(const stromx::runtime::Image & image);
            void allocate(const unsigned int width, const unsigned int height, const Image::PixelType pixelType);
            
            cv::Mat* m_image;
        };
    }
}

#endif // STROMX_CVSUPPORT_IMAGE_H