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

#include <stromx/core/Input.h>
#include <stromx/core/Output.h>
#include <stromx/core/Operator.h>

#include <boost/python.hpp>

using namespace boost::python;
using namespace stromx::core;

void exportConnector()
{
    {
        scope in_Connector = 
        class_<Connector>("Connector")
            .def("id", reinterpret_cast<unsigned int (Connector::*)() const>(&Connector::id))
            .def("op", reinterpret_cast<Operator* (Connector::*)() const>(&Connector::op), return_internal_reference<>())
            .def("valid", reinterpret_cast<bool (Connector::*)() const>(&Connector::valid))
            .def("type", reinterpret_cast<Connector::Type (Connector::*)() const>(&Connector::type))
        ;
        
        enum_<Connector::Type>("Type")
            .value("INPUT", Connector::INPUT)
            .value("OUTPUT", Connector::OUTPUT)
            .export_values()
        ;
    }
    
    class_<Output, bases<Connector> >("Output", init<Operator*, unsigned int>())
    ;
    
    class_<Input, bases<Connector> >("Input", init<Operator*, unsigned int>())
    ;
}