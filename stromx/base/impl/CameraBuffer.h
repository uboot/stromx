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

#ifndef BASE_IMPL_CAMERABUFFER_H
#define BASE_IMPL_CAMERABUFFER_H

#include <stromx/core/OperatorKernel.h>
#include <stromx/core/RecycleAccess.h>
#include <stromx/core/Primitive.h>

namespace core
{
    class Data;
}

namespace base
{
    namespace impl
    {
        class CameraBuffer : public core::OperatorKernel
        {
        public:
            enum InputId
            {
                INPUT
            };
            
            enum OutputId
            {
                OUTPUT,
                BUFFER,
                INDEX
            };
            
            enum ParameterId
            {
                NUM_BUFFERS,
                BUFFER_SIZE
            };
            
            CameraBuffer();
            
            virtual OperatorKernel* const clone() const { return new CameraBuffer; }
            virtual void setParameter(const unsigned int id, const core::Data& value);
            virtual const core::Data& getParameter(const unsigned int id) const;
            virtual void execute(core::DataProvider& provider);
            virtual void activate();
            virtual void deactivate();
            
        private:
            static const std::vector<const core::Description*> setupInputs();
            static const std::vector<const core::Description*> setupOutputs();
            static const std::vector<const core::Parameter*> setupParameters();
            
            static const std::string TYPE;
            static const std::string PACKAGE;
            static const core::Version VERSION;
            
            core::RecycleAccess m_buffers;
            core::UInt32 m_bufferSize;
            core::UInt32 m_numBuffers;
            unsigned int m_id;
        };
    }
}

#endif // BASE_IMPL_CAMERABUFFER_H
