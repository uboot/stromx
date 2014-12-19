/* 
*  Copyright 2012 Matthias Fuchs
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

#include <stromx/runtime/ExceptionObserver.h>
#include <stromx/runtime/Thread.h>

#include <boost/python.hpp>

using namespace boost::python;
using namespace stromx::runtime;

namespace
{
    template<typename T>
    void do_release(T*)
    {
    }
    
    struct ExceptionObserverWrap : ExceptionObserver, wrapper<ExceptionObserver>
    {
        void observe(const Phase phase, const OperatorError & ex, const Thread* const thread) const
        {
            observeWrap(phase, ex.message().c_str(), boost::shared_ptr<Thread>(const_cast<Thread*>(thread), &do_release<Thread>));
        }
        
        void observeWrap(const Phase phase, const char* message, boost::shared_ptr<Thread> thread) const
        {
            PyGILState_STATE state = PyGILState_Ensure();
            
            try
            {
                this->get_override("observe")(phase, message, thread);
            }
            catch(...)
            {
                // catch everything to make sure the GIL is released
            }
            
            PyGILState_Release(state);
        }
    };
}

void exportExceptionObserver()
{         
    scope in_ExceptionObserver = 
    class_<ExceptionObserverWrap, boost::noncopyable>("ExceptionObserver")
        .def("observe", pure_virtual(&ExceptionObserverWrap::observeWrap))
    ; 
    
    enum_<ExceptionObserver::Phase>("Phase")
        .value("EXECUTION", ExceptionObserver::EXECUTION)
        .value("ACTIVATION", ExceptionObserver::ACTIVATION)
        .value("DEACTIVATION", ExceptionObserver::DEACTIVATION)
        .value("INTERRUPT", ExceptionObserver::INTERRUPT)
        .value("INITIALIZATION", ExceptionObserver::INITIALIZATION)
        .value("DEINITIALIZATION", ExceptionObserver::DEINITIALIZATION)
        .value("PARAMETER_ACCESS", ExceptionObserver::PARAMETER_ACCESS)
        ;  
}
