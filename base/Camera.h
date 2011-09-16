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

#include <strom/Primitive.h>
#include <strom/OperatorKernel.h>

namespace strom
{
    class DataContainer;
    class Operator;
    class Stream;
    
    template<typename T>
    class NumericParameter;
}

namespace base
{
    class Camera : public strom::OperatorKernel
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
        virtual ~Camera();
        
        virtual OperatorKernel* const clone() const { return new Camera; }
        virtual void setParameter(unsigned int id, const strom::Data& value);
        virtual const strom::Data& getParameter(unsigned int id);
        virtual void execute(strom::DataProvider& provider);
        virtual void initialize();
        virtual void activate();
        virtual void deactivate();
        
    private:
        static const std::vector<const strom::Description*> setupInputs();
        static const std::vector<const strom::Description*> setupOutputs();
        const std::vector<const strom::Parameter*> setupParameters();
        void setRgbParameters();
        
        static const std::string TYPE;
        static const std::string PACKAGE;
        static const strom::Version VERSION; 
        
        static const unsigned int WHITE_BALANCE_MAX = 10;
        static const unsigned int BASE_EXPOSURE = 10;
        
        strom::Stream* m_stream;
        
        strom::Operator* m_input;
        strom::Operator* m_clip;
        strom::Operator* m_adjustRgbChannels;
        strom::Operator* m_period;
        strom::Operator* m_trigger;
        strom::Operator* m_buffer;
        strom::Operator* m_pixelType;
        strom::Operator* m_imageQueue;
        strom::Operator* m_indexQueue;
        
        strom::NumericParameter<strom::UInt32>* m_left;
        strom::NumericParameter<strom::UInt32>* m_top;
        strom::NumericParameter<strom::UInt32>* m_width;
        strom::NumericParameter<strom::UInt32>* m_height;
        
        unsigned int m_imageWidth;
        unsigned int m_imageHeight;
        unsigned int m_exposure;
        double m_wbRed;
        double m_wbGreen;
        double m_wbBlue;
    };
}

#endif // BASE_CAMERA_H
