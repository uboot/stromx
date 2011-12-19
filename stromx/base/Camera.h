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

#ifndef STROMX_BASE_CAMERA_H
#define STROMX_BASE_CAMERA_H

#include "Config.h"
#include <stromx/core/Enum.h>
#include <stromx/core/OperatorKernel.h>
#include <stromx/core/Primitive.h>
#include <stromx/core/Trigger.h>

namespace stromx
{
    namespace core
    {
        class DataContainer;
        class Operator;
        class Stream;
        
        template<typename T>
        class NumericParameter;
    }

    namespace base
    {
        /** \brief Simulates a camera input. */
        class STROMX_BASE_API Camera : public core::OperatorKernel
        {
        public:
            enum OutputId
            {
                OUTPUT,
                INDEX
            };
            
            enum ParameterId
            {
                IMAGE,
                TRIGGER_MODE,
                TRIGGER,
                EXPOSURE,
                FRAME_PERIOD,
                BUFFER_SIZE,
                NUM_BUFFERS,
                WHITE_BALANCE_RED,
                WHITE_BALANCE_GREEN,
                WHITE_BALANCE_BLUE,
                LEFT,
                TOP,
                WIDTH,
                HEIGHT,
                PIXEL_TYPE
            };
            
            enum TriggerMode
            {
                SOFTWARE,
                INTERNAL,
                EXTERNAL
            };
            
            Camera();
            
            virtual OperatorKernel* const clone() const { return new Camera; }
            virtual void setParameter(const unsigned int id, const core::Data& value);
            virtual const core::Data& getParameter(const unsigned int id) const;
            virtual void execute(core::DataProvider& provider);
            virtual void initialize();
            virtual void deinitialize();
            virtual void activate();
            virtual void deactivate();
            
        private:
            static const std::vector<const core::Description*> setupInputs();
            static const std::vector<const core::Description*> setupOutputs();
            const std::vector<const core::Parameter*> setupParameters();
            void setRgbParameters();
            
            static const std::string TYPE;
            static const std::string PACKAGE;
            static const core::Version VERSION; 
            
            static const unsigned int WHITE_BALANCE_MAX = 10;
            static const unsigned int BASE_EXPOSURE = 10;
            
            core::Stream* m_stream;
            
            core::Operator* m_input;
            core::Operator* m_clip;
            core::Operator* m_adjustRgbChannels;
            core::Operator* m_period;
            core::Operator* m_trigger;
            core::Operator* m_buffer;
            core::Operator* m_pixelType;
            core::Operator* m_imageQueue;
            core::Operator* m_indexQueue;
            
            core::NumericParameter<core::UInt32>* m_left;
            core::NumericParameter<core::UInt32>* m_top;
            core::NumericParameter<core::UInt32>* m_width;
            core::NumericParameter<core::UInt32>* m_height;
            
            core::UInt32 m_imageWidth;
            core::UInt32 m_imageHeight;
            core::UInt32 m_exposure;
            core::Double m_wbRed;
            core::Double m_wbGreen;
            core::Double m_wbBlue;

            core::Trigger m_valueTrigger;
            core::Enum m_valueInternal;
            core::Enum m_valueSoftware;
        };
    }
}
    
#endif // STROMX_BASE_CAMERA_H
