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

#ifndef STROMX_IMGUTIL_CLIP_H
#define STROMX_IMGUTIL_CLIP_H

#include "stromx/imgutil/Config.h"
#include <stromx/runtime/OperatorKernel.h>
#include <stromx/runtime/Primitive.h>
#include <stromx/runtime/RecycleAccess.h>

namespace stromx
{
    namespace imgutil
    {
        /** \brief Clips an image to a rectangular region. */
        class STROMX_IMGUTIL_API Clip : public runtime::OperatorKernel
        {
        public:
            enum InputId
            {
                INPUT
            };
            
            enum OutputId
            {
                OUTPUT
            };
            
            enum ParameterId
            {
                TOP,
                LEFT,
                WIDTH,
                HEIGHT,
                NUM_PARAMS
            };
            
            Clip();
            
            virtual OperatorKernel* clone() const { return new Clip; }
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
            
            void adjustClipRegion(const unsigned int destWidth, const unsigned int destHeight,
                                unsigned int & left, unsigned int & top,
                                unsigned int & width, unsigned int & height);                           
            
            runtime::UInt32 m_top;
            runtime::UInt32 m_left;
            runtime::UInt32 m_width;
            runtime::UInt32 m_height;
        };
    }
}

#endif // STROMX_IMGUTIL_CLIP_H
