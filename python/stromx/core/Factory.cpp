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
#include <stromx/core/OperatorKernel.h>
#include <stromx/core/Factory.h>
#include <stromx/core/Data.h>

#include <boost/python.hpp>

using namespace boost::python;
using namespace stromx::core;

void exportFactory()
{          
    typedef Operator* (Factory::*newOperatorType)(const std::string & package, const std::string & name) const; 
    typedef Data* (Factory::*newDataType)(const std::string & package, const std::string & name) const; 
    
    class_<Factory, bases<Registry> >("Factory")
        .def("registerOperator", &Factory::registerOperator)
        .def("registerData", &Factory::registerData)
        .def("newOperator", reinterpret_cast<newOperatorType>(&Factory::newOperator), return_value_policy<manage_new_object>())
        .def("newData", reinterpret_cast<newDataType>(&Factory::newData), return_value_policy<manage_new_object>())
    ;
}