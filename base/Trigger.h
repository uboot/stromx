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

#ifndef BASE_TRIGGER_H
#define BASE_TRIGGER_H

#include <strom/OperatorKernel.h>
#include <strom/Image.h>
#include <strom/Primitive.h>

#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>

namespace strom
{
    class DataContainer;
}

namespace base
{
    class Trigger : public strom::OperatorKernel
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
            TRIGGER,
            ACTIVE
        };
        
        Trigger();
        
        virtual OperatorKernel* const clone() const { return new Trigger; }
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
        
        typedef boost::unique_lock<boost::mutex> unique_lock_t;
        
        boost::condition_variable_any m_cond;
        boost::mutex m_mutex;
        strom::Bool m_active;
    };
}

#endif // BASE_TRIGGER_H
