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

#include <stromx/core/ExceptionObserver.h>
#include <stromx/core/Thread.h>

#include <boost/python.hpp>

using namespace boost::python;
using namespace stromx::core;

namespace
{
    struct ExceptionObserverWrap : ExceptionObserver, wrapper<ExceptionObserver>
    {
        void observe(const Phase phase, const OperatorError & ex, const Thread* const thread) const
        {
            PyGILState_STATE state = PyGILState_Ensure();
            this->get_override("observe")(phase, thread, ex);
            PyGILState_Release(state);
        }
    };
}

void exportExceptionObserver()
{          
    enum_<ExceptionObserver::Phase>("ExceptionObserverPhase")
        .value("EXECUTION", ExceptionObserver::EXECUTION)
        .value("ACTIVATION", ExceptionObserver::ACTIVATION)
        .value("DEACTIVATION", ExceptionObserver::DEACTIVATION)
        ;  
        
    class_<ExceptionObserverWrap, boost::noncopyable>("ExceptionObserver")
        .def("observe", pure_virtual(&ExceptionObserver::observe))
    ;
}
