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

#include "ThreadUtilities.h"

#include <stromx/runtime/ConnectorObserver.h>
#include <stromx/runtime/Connector.h>
#include <stromx/runtime/Exception.h>
#include <stromx/runtime/DataContainer.h>
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
    template<typename T>
    void do_release(T*)
    {
    }
    
    struct ConnectorObserverWrap : ConnectorObserver, wrapper<ConnectorObserver>
    {
        void observe(const Connector & connector, const DataContainer & data, const Thread* const thread) const
        {
            observeWrap(connector, data, boost::shared_ptr<Thread>(const_cast<Thread*>(thread), &do_release<Thread>));
        }
        
        void observeWrap(const Connector & connector, const DataContainer & data, boost::shared_ptr<Thread> thread) const
        {
            PyGILState_STATE state = PyGILState_Ensure();
            
            try
            {
                this->get_override("observe")(connector, data, thread);
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

void exportConnectorObserver()
{                 
    class_<ConnectorObserverWrap, boost::noncopyable>("ConnectorObserver")
        .def("observe", pure_virtual(&ConnectorObserverWrap::observeWrap))
    ;
}
