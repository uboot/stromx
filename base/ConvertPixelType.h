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

#ifndef BASE_CONVERTPIXELTYPE_H
#define BASE_CONVERTPIXELTYPE_H

#include <stream/OperatorKernel.h>
#include <stream/Image.h>
#include <stream/Enum.h>
#include <stream/RecycleAccess.h>


namespace base
{
    class ConvertPixelType : public stream::OperatorKernel
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
        
        virtual OperatorKernel* const clone() const { return new ConvertPixelType; }
        virtual void setParameter(unsigned int id, const stream::Data& value);
        virtual const stream::Data& getParameter(unsigned int id);
        virtual void execute(stream::DataProvider& provider);
        
    private:
        static const std::vector<const stream::Description*> setupInputs();
        static const std::vector<const stream::Description*> setupOutputs();
        static const std::vector<const stream::Parameter*> setupParameters();
        
        static const std::string NAME;
        static const std::string PACKAGE;
        static const stream::Version VERSION;   
        
        static const int getCvConversionCode(const stream::Image::PixelType inType, const stream::Image::PixelType outType);
        static const unsigned int getDestPixelSize(const stream::Image::PixelType pixelType);   
        static void rgbToBayer(const stream::Image & inImage, stream::Image & outImage);  
        static void openCvConversion(const stream::Image & inImage, stream::Image & outImage);
        
        stream::Enum m_pixelType;
    };
}

#endif // BASE_CONVERTPIXELTYPE_H
