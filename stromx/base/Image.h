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

#ifndef STROMX_BASE_IMAGEIMPL_H
#define STROMX_BASE_IMAGEIMPL_H

#include <string>
#include "Config.h"
#include <stromx/core/ImageWrapper.h>
#include <stromx/core/Input.h>

struct _IplImage;

namespace cv
{
    class Mat;
}

namespace stromx
{
    namespace base
    {
        /** \brief %Image with support for reading and writing. */
        class STROMX_BASE_API Image : public core::ImageWrapper
        {
        public:
            enum FileAccess
            {
                UNCHANGED,
                GRAYSCALE,
                COLOR
            };
            
            Image();
            explicit Image(const unsigned int width, const unsigned int height, const PixelType pixelType);
            explicit Image(const std::string & filename);
            explicit Image(const std::string & filename, const FileAccess access);
            explicit Image(const core::Image& image);
            explicit Image(const stromx::base::Image& image);
            explicit Image(const unsigned int size);
            
            virtual ~Image();
            
            virtual const core::Version & version() const { return VERSION; }
            virtual const std::string & type() const { return TYPE; }
            virtual const std::string & package() const { return PACKAGE; }
            
            virtual Data* const clone() const;
            
            virtual void serialize(core::OutputProvider & output) const;
            virtual void deserialize(core::InputProvider & input, const stromx::core::Version & version);
            
            void open(const std::string& filename);
            void open(const std::string & filename, const FileAccess access);
            void save(const std::string& filename) const;
            void resize(const unsigned int width, const unsigned int height, const PixelType pixelType);
            void resize(const unsigned int size);
            
        private:
            static const std::string TYPE;
            static const std::string PACKAGE;
            static const core::Version VERSION;
            
            static const PixelType pixelTypeFromParameters(const int depth, const int numChannels);
            static const int getCvAccessType(const FileAccess access);
            static const int cvTypeFromPixelType(const core::Image::PixelType pixelType);
            static const core::Image::PixelType pixelTypeFromCvType(const int cvType);
            
            void getDataFromCvImage(const PixelType pixelType);
            void copy(const stromx::core::Image & image);
            void allocate(const unsigned int width, const unsigned int height, const Image::PixelType pixelType);
            
            cv::Mat* m_image;
        };
    }
}

#endif // STROMX_BASE_IMAGEIMPL_H