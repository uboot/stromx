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

#include <stromx/runtime/ConnectorObserver.h>
#include <stromx/runtime/Connector.h>
#include <stromx/runtime/DataContainer.h>

#include <boost/python.hpp>

using namespace boost::python;
using namespace stromx::runtime;

namespace
{
    struct ConnectorObserverWrap : ConnectorObserver, wrapper<ConnectorObserver>
    {
        void observe(const Connector & connector, const DataContainer & data) const
        {
            PyGILState_STATE state = PyGILState_Ensure();
            
            try
            {
                this->get_override("observe")(connector, data);
            }
            catch(...)
            {
                // catch everything to make sure the GIL is released
            }
            
            PyGILState_Release(state);
        }
    };
}

void exportConnectorObserver()
{                 
    class_<ConnectorObserverWrap, boost::noncopyable>("ConnectorObserver")
        .def("observe", pure_virtual(&ConnectorObserver::observe))
    ;
}
