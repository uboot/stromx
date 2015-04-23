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

#include "ThreadUtilities.h"

#include <stromx/runtime/ExceptionObserver.h>
#include <stromx/runtime/Thread.h>

#include <boost/python.hpp>
#include <boost/thread.hpp>

using namespace boost::python;
using namespace stromx::python;
using namespace stromx::runtime;

namespace stromx
{
    namespace runtime
    {
        extern boost::thread_specific_ptr<Thread> gThread;
    }
}

namespace
{
    class OperatorExceptionInfo
    {
    public: 
        OperatorExceptionInfo(const std::string & message, const std::string & name,
                              const std::string & type, const std::string & package) 
          : m_message(message),
            m_name(name),
            m_type(type),
            m_package(package)
        { }
        
        const std::string & message() const { return m_message; }
        const std::string & name() const { return m_name; }
        const std::string & type() const { return m_type; }
        const std::string & package() const { return m_package; }
        
    private:
        const std::string m_message;
        const std::string m_name;
        const std::string m_type;
        const std::string m_package;
    };
    
    template<typename T>
    void do_release(T*)
    {
    }
    
    struct ExceptionObserverWrap : ExceptionObserver, wrapper<ExceptionObserver>
    {
        void observe(const Phase phase, const OperatorError & ex, const Thread* const thread) const
        {
            OperatorExceptionInfo info(ex.message(), ex.name(), ex.type(), ex.package());
            observeWrap(phase, info, boost::shared_ptr<Thread>(const_cast<Thread*>(thread), &do_release<Thread>));
        }
        
        void observeWrap(const Phase phase, const OperatorExceptionInfo & info,
                         boost::shared_ptr<Thread> thread) const
        {
            PyGILState_STATE state = PyGILState_Ensure();
            
            try
            {
                this->get_override("observe")(phase, info, thread);
            }
            catch(...)
            {
                // catch everything to make sure the GIL is released
            }
            
            PyGILState_Release(state);
            
            Thread* threadPtr = gThread.get();
            if (threadPtr && ThreadUtilities::interruptedFlag(threadPtr))
                throw Interrupt();
        }
    };
}

void exportExceptionObserver()
{         
    class_<OperatorExceptionInfo>("OperatorExceptionInfo", no_init)
        .def("message", &OperatorExceptionInfo::message, return_value_policy<copy_const_reference>())
        .def("name", &OperatorExceptionInfo::name, return_value_policy<copy_const_reference>())
        .def("type", &OperatorExceptionInfo::type, return_value_policy<copy_const_reference>())
        .def("package", &OperatorExceptionInfo::package, return_value_policy<copy_const_reference>())
    ;
    
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
