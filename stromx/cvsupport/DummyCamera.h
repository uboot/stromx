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

#ifndef STROMX_CVSUPPORT_DUMMYCAMERA_H
#define STROMX_CVSUPPORT_DUMMYCAMERA_H

#include "stromx/cvsupport/Config.h"
#include <stromx/runtime/DataRef.h>
#include <stromx/runtime/Enum.h>
#include <stromx/runtime/Image.h>
#include <stromx/runtime/OperatorKernel.h>
#include <stromx/runtime/Primitive.h>
#include <stromx/runtime/TriggerData.h>

namespace stromx
{
    namespace runtime
    {
        class DataContainer;
        class Operator;
        class Stream;
        
        template<typename T>
        class NumericParameter;
    }

    namespace cvsupport
    {
        /** \brief Simulates a camera input. */
        class STROMX_CVSUPPORT_API DummyCamera : public runtime::OperatorKernel
        {
        public:
            enum OutputId
            {
                OUTPUT,
                INDEX
            };
            
            enum ParameterId
            {
                OUTPUT_INDEX,
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
                PIXEL_TYPE,
                WHITE_BALANCE_GROUP,
                ROI_GROUP,
                FLICKER_AMOUNT
            };
            
            enum TriggerMode
            {
                SOFTWARE,
                INTERNAL,
                EXTERNAL
            };
            
            DummyCamera();
            virtual ~DummyCamera();
            
            virtual OperatorKernel* clone() const { return new DummyCamera; }
            virtual void setParameter(const unsigned int id, const runtime::Data& value);
            virtual const runtime::DataRef getParameter(const unsigned int id) const;
            virtual void execute(runtime::DataProvider& provider);
            virtual void initialize();
            virtual void deinitialize();
            virtual void activate();
            virtual void deactivate();
            
        private:
            static bool validateBufferSize(unsigned int bufferSize,
                                           unsigned int width,
                                           unsigned int height,
                                           unsigned int depth,
                                           const runtime::Enum outputType);
            
            const std::vector<const runtime::Parameter*> setupInitParameters();
            const std::vector<const runtime::Description*> setupInputs();
            const std::vector<const runtime::Description*> setupOutputs();
            const std::vector<const runtime::Parameter*> setupParameters();
            void setRgbParameters();
            
            static const std::string TYPE;
            static const std::string PACKAGE;
            static const runtime::Version VERSION; 
            
            static const unsigned int WHITE_BALANCE_MAX = 10;
            static const unsigned int BASE_EXPOSURE = 10;
            
            runtime::Stream* m_stream;
            
            runtime::Operator* m_input;
            runtime::Operator* m_clip;
            runtime::Operator* m_adjustRgbChannels;
            runtime::Operator* m_flicker;
            runtime::Operator* m_period;
            runtime::Operator* m_trigger;
            runtime::Operator* m_buffer;
            runtime::Operator* m_pixelType;
            runtime::Operator* m_imageQueue;
            runtime::Operator* m_indexQueue;
            
            runtime::Bool m_outputIndex;
            
            runtime::NumericParameter<runtime::UInt32>* m_left;
            runtime::NumericParameter<runtime::UInt32>* m_top;
            runtime::NumericParameter<runtime::UInt32>* m_width;
            runtime::NumericParameter<runtime::UInt32>* m_height;
            
            unsigned int m_imageWidth;
            unsigned int m_imageHeight;
            unsigned int m_imageDepth;
            runtime::UInt32 m_exposure;
            runtime::Float64 m_wbRed;
            runtime::Float64 m_wbGreen;
            runtime::Float64 m_wbBlue;
            
            bool m_isFirstInitialization;
        };
    }
}
    
#endif // STROMX_CVSUPPORT_DUMMYCAMERA_H
