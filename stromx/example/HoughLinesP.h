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

#ifndef STROMX_EXAMPLE_HOUGHLINESP_H
#define STROMX_EXAMPLE_HOUGHLINES_H

#include <stromx/runtime/Enum.h>
#include <stromx/runtime/OperatorKernel.h>

#include "Image.h"

namespace stromx
{
    namespace example
    {
        /** \brief Computes the Hough line transform of an image. */
        class STROMX_EXAMPLE_API HoughLinesP : public runtime::OperatorKernel
        {
        public:
            enum InputId
            {
                IMAGE
            };
            
            enum OutputId
            {
                LINES
            };
            
            enum ParameterId
            {
                TRANSFORM,
                RHO,
                THETA,
                THRESHOLD,
                MIN_LINE_LENGTH,
                MAX_LINE_GAP
            };
            
            HoughLinesP();
            
            virtual OperatorKernel* clone() const { return new HoughLinesP; }
            virtual void setParameter(const unsigned int id, const runtime::Data& value);
            virtual const runtime::DataRef getParameter(const unsigned int id) const;
            virtual void execute(runtime::DataProvider& provider);
            
        private:
            virtual const std::vector<const runtime::Parameter*> setupParameters();
            const std::vector<const runtime::Description*> setupInputs();
            const std::vector<const runtime::Description*> setupOutputs();
            
            static const std::string PACKAGE;
            static const runtime::Version VERSION;
            static const std::string TYPE;
            
            runtime::Double m_rho;
            runtime::Double m_theta;
            runtime::UInt32 m_threshold;
            runtime::Double m_minLineLength;
            runtime::Double m_maxLineGap;
        };
    }
}

#endif // STROMX_EXAMPLE_HOUGHLINESP_H
