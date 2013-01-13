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

#ifndef STROMX_EXAMPLE_CONVERTPIXELTYPE_H
#define STROMX_EXAMPLE_CONVERTPIXELTYPE_H

#include "Config.h"
#include <stromx/runtime/Enum.h>
#include <stromx/runtime/Image.h>
#include <stromx/runtime/OperatorKernel.h>
#include <stromx/runtime/RecycleAccess.h>

namespace stromx
{
    namespace example
    {
        /** \brief Converts the pixel type of image. */
        class STROMX_EXAMPLE_API ConvertPixelType : public runtime::OperatorKernel
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
            virtual void setParameter(const unsigned int id, const runtime::Data& value);
            virtual const runtime::DataRef getParameter(const unsigned int id) const;
            virtual void execute(runtime::DataProvider& provider);
            
        private:
            static const std::vector<const runtime::Description*> setupInputs();
            static const std::vector<const runtime::Description*> setupOutputs();
            static const std::vector<const runtime::Parameter*> setupParameters();
            
            static const std::string TYPE;
            static const std::string PACKAGE;
            static const runtime::Version VERSION;   
            
            static int getCvConversionCode(const runtime::Image::PixelType inType, const runtime::Image::PixelType outType);
            static unsigned int getDestPixelSize(const runtime::Image::PixelType pixelType);   
            static void rgbToBayer(const runtime::Image & inImage, runtime::Image & outImage);  
            static void openCvConversion(const runtime::Image & inImage, runtime::Image & outImage);
            
            runtime::Enum m_pixelType;
        };
    }
}

#endif // STROMX_EXAMPLE_CONVERTPIXELTYPE_H
