/* 
*  Copyright 2012 Matthias Fuchs
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

#ifndef STROMX_EXAMPLE_IMAGEFILTER_H
#define STROMX_EXAMPLE_IMAGEFILTER_H

#include "Config.h"
#include <stromx/runtime/Primitive.h>
#include <stromx/runtime/Image.h>
#include <stromx/runtime/OperatorKernel.h>
#include <stromx/runtime/RecycleAccess.h>

namespace cv
{
    class Mat;
}

namespace stromx
{
    namespace example
    {
        /** \brief Abstract base class of image filters. */
        class STROMX_EXAMPLE_API ImageFilter : public runtime::OperatorKernel
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
                IN_PLACE,
		FILTER_PARAMETERS
            };
            
            ImageFilter(const std::string & type);
            
            virtual void setParameter(const unsigned int id, const runtime::Data& value);
            virtual const runtime::DataRef getParameter(const unsigned int id) const;
            virtual void execute(runtime::DataProvider& provider);
            virtual void initialize();
            
        protected:
            virtual const std::vector<const runtime::Parameter*> setupParameters() = 0;
            virtual void applyFilter(const cv::Mat & in, cv::Mat & out) = 0;
            virtual void validateSourceImage(const runtime::Image & source) = 0;
            virtual unsigned int computeDestinationSize(const runtime::Image & source) = 0;
            
	private:
            const std::vector<const runtime::Description*> setupInputs();
            const std::vector<const runtime::Description*> setupOutputs();
            const std::vector<const runtime::Parameter*> setupInitParameters();
	    
            static const std::string PACKAGE;
            static const runtime::Version VERSION;
            
            runtime::Bool m_inPlace;
        };
    }
}

#endif // STROMX_EXAMPLE_IMAGEFILTER_H
