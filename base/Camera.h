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

#ifndef BASE_CAMERA_H
#define BASE_CAMERA_H

#include <stream/Primitive.h>
#include <stream/OperatorKernel.h>

namespace stream
{
    class DataContainer;
    class Operator;
    class Stream;
}

namespace base
{
    class Camera : public stream::OperatorKernel
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
            FRAME_PERIOD,
            BUFFER_SIZE,
            NUM_BUFFERS,
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
        virtual ~Camera();
        
        virtual OperatorKernel* const clone() const { return new Camera; }
        virtual void setParameter(unsigned int id, const stream::Data& value);
        virtual const stream::Data& getParameter(unsigned int id);
        virtual void execute(stream::DataProvider& provider);
        virtual void initialize();
        virtual void activate();
        virtual void deactivate();
        
    private:
        static const std::vector<const stream::Description*> setupInputs();
        static const std::vector<const stream::Description*> setupOutputs();
        static const std::vector<const stream::Parameter*> setupParameters();
        
        static const std::string NAME;
        static const std::string PACKAGE;
        static const stream::Version VERSION; 
        
        stream::Stream* m_stream;
        
        stream::Operator* m_input;
        stream::Operator* m_clip;
        stream::Operator* m_adjustRgbChannels;
        stream::Operator* m_period;
        stream::Operator* m_trigger;
        stream::Operator* m_buffer;
        stream::Operator* m_pixelType;
        stream::Operator* m_imageQueue;
        stream::Operator* m_indexQueue;
    };
}

#endif // BASE_CAMERA_H
