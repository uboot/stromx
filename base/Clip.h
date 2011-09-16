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

#ifndef BASE_CLIP_H
#define BASE_CLIP_H

#include <strom/OperatorKernel.h>
#include <strom/Primitive.h>
#include <strom/RecycleAccess.h>


namespace base
{
    class Clip : public strom::OperatorKernel
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
        
        virtual OperatorKernel* const clone() const { return new Clip; }
        virtual void setParameter(unsigned int id, const strom::Data& value);
        virtual const strom::Data& getParameter(unsigned int id);
        virtual void execute(strom::DataProvider& provider);
        
    private:
        static const std::vector<const strom::Description*> setupInputs();
        static const std::vector<const strom::Description*> setupOutputs();
        static const std::vector<const strom::Parameter*> setupParameters();
        
        static const std::string TYPE;
        static const std::string PACKAGE;
        static const strom::Version VERSION;
        
        void adjustClipRegion(const unsigned int destWidth, const unsigned int destHeight,
                              unsigned int & left, unsigned int & top,
                              unsigned int & width, unsigned int & height);                           
        
        strom::UInt32 m_top;
        strom::UInt32 m_left;
        strom::UInt32 m_width;
        strom::UInt32 m_height;
    };
}

#endif // BASE_CLIP_H
