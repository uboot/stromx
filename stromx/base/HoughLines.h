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

#ifndef STROMX_BASE_HOUGHLINES_H
#define STROMX_BASE_HOUGHLINES_H

#include <stromx/core/Enum.h>
#include <stromx/core/OperatorKernel.h>

#include "Image.h"

namespace stromx
{
    namespace base
    {
        /** \brief Computes the Hough line transform of an image. */
        class STROMX_BASE_API HoughLines : public core::OperatorKernel
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
            
            enum Transform
            {
                HOUGH,
                PROBALISTIC_HOUGH
            };
            
            HoughLines();
            
            virtual OperatorKernel*const clone() const { return new HoughLines; }
            virtual void setParameter(const unsigned int id, const core::Data& value);
            virtual const core::Data& getParameter(const unsigned int id) const;
            virtual void execute(core::DataProvider& provider);
            
        private:
            virtual const std::vector<const core::Parameter*> setupParameters();
            const std::vector<const core::Description*> setupInputs();
            const std::vector<const core::Description*> setupOutputs();
            
            static const std::string PACKAGE;
            static const core::Version VERSION;
            static const std::string TYPE;
            
            core::Enum m_transform;
            core::Double m_rho;
            core::Double m_theta;
            core::UInt32 m_threshold;
            core::Double m_minLineLength;
            core::Double m_maxLineGap;
        };
    }
}

#endif // STROMX_BASE_HOUGHLINES_H
