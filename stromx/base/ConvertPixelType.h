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

#ifndef STROMX_BASE_CONVERTPIXELTYPE_H
#define STROMX_BASE_CONVERTPIXELTYPE_H

#include "Config.h"
#include <stromx/core/Enum.h>
#include <stromx/core/Image.h>
#include <stromx/core/OperatorKernel.h>
#include <stromx/core/RecycleAccess.h>

namespace stromx
{
    namespace base
    {
        /** \brief Converts the pixel type of image. */
        class STROMX_BASE_API ConvertPixelType : public core::OperatorKernel
        {
        public:
            enum InputId
            {
                SOURCE,
                DESTINATION
            };
            
            enum OutputId
            {
                OUTPUT
            };
            
            enum ParameterId
            {
                PIXEL_TYPE
            };
            
            ConvertPixelType();
            
            virtual OperatorKernel* clone() const { return new ConvertPixelType; }
            virtual void setParameter(const unsigned int id, const core::Data& value);
            virtual const core::DataRef getParameter(const unsigned int id) const;
            virtual void execute(core::DataProvider& provider);
            
        private:
            static const std::vector<const core::Description*> setupInputs();
            static const std::vector<const core::Description*> setupOutputs();
            static const std::vector<const core::Parameter*> setupParameters();
            
            static const std::string TYPE;
            static const std::string PACKAGE;
            static const core::Version VERSION;   
            
            static const int getCvConversionCode(const core::Image::PixelType inType, const core::Image::PixelType outType);
            static const unsigned int getDestPixelSize(const core::Image::PixelType pixelType);   
            static void rgbToBayer(const core::Image & inImage, core::Image & outImage);  
            static void openCvConversion(const core::Image & inImage, core::Image & outImage);
            
            core::Enum m_pixelType;
        };
    }
}

#endif // STROMX_BASE_CONVERTPIXELTYPE_H
