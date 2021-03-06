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

#include <boost/python.hpp>
#include <stromx/runtime/ConnectorDescription.h>
#include <stromx/runtime/Input.h>
#include <stromx/runtime/Output.h>

using namespace boost::python;
using namespace stromx::runtime;

void exportDescription()
{       
    class_<ConnectorDescription, bases<Description> >("ConnectorDescription", no_init)
        .def("operatorThread", &ConnectorDescription::operatorThread)
        .def("defaultType", &ConnectorDescription::defaultType)
    ;
    
    class_<Input, bases<ConnectorDescription> >("Input", no_init)
    ;
    
    class_<Output, bases<ConnectorDescription> >("Output", no_init)
    ;
}
