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

#include <stromx/core/Operator.h>

#include <boost/python.hpp>

using namespace boost::python;
using namespace stromx::core;

void exportOperator()
{       
    enum_<Operator::Status>("OperatorStatus")
        .value("NONE", Operator::NONE)
        .value("INITIALIZED", Operator::INITIALIZED)
        .value("ACTIVE", Operator::ACTIVE)
        .value("EXECUTING", Operator::EXECUTING)
        ;
        
    class_<Operator>("Operator", no_init)
        .def("status", &Operator::status)
        .def("name", &Operator::name, return_value_policy<copy_const_reference>())
        .def("setName", &Operator::setName)
        .def("initialize", &Operator::initialize)
        .def("getOutputData", &Operator::getOutputData)
        .def("setInputData", &Operator::setInputData)
        .def("clearOutputData", &Operator::clearOutputData)
    ;
}

            

