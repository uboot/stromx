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

#include <stream/Operator.h>
#include <stream/Primitive.h>

namespace stream
{
    class DataContainer;
    class Network;
    class OperatorNode;
    class Stream;
}

namespace base
{
    class Camera : public stream::Operator
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
            TRIGGER
        };
        
        enum TriggerMode
        {
            SOFTWARE,
            INTERNAL,
            EXTERNAL
        };
        
        Camera();
        virtual ~Camera();
        
        virtual Operator* const clone() const { return new Camera; }
        virtual void setParameter(unsigned int id, const stream::Data& value);
        virtual const stream::Data& getParameter(unsigned int id);
        virtual void execute(stream::DataProvider& provider);
        virtual void initialize();
        virtual void activate();
        virtual void deactivate();
        
    private:
        static const std::vector<stream::Description*> setupInputs();
        static const std::vector<stream::Description*> setupOutputs();
        static const std::vector<stream::Parameter*> setupParameters();
        
        static const std::string NAME;
        static const std::string PACKAGE;
        static const stream::Version VERSION; 
        
        stream::Stream* m_stream;
        
        stream::OperatorNode* m_input;
        stream::OperatorNode* m_clip;
        stream::OperatorNode* m_adjustRgbChannels;
        stream::OperatorNode* m_period;
        stream::OperatorNode* m_trigger;
        stream::OperatorNode* m_buffer;
        stream::OperatorNode* m_pixelType;
        stream::OperatorNode* m_imageQueue;
        stream::OperatorNode* m_indexQueue;
    };
}

#endif // BASE_CAMERA_H
