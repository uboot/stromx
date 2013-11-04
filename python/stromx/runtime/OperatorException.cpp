/* 
*  Copyright 2013 Matthias Fuchs
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

#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/OperatorInfo.h>

#include <boost/python.hpp>

using namespace boost::python;
using namespace stromx::runtime;

void exportOperatorException()
{                 
    class_<OperatorError, bases<Exception> >("OperatorError", no_init)
        .def("op", &OperatorError::op, return_internal_reference<>())
        .def("name", &OperatorError::name, return_internal_reference<>())
    ;   
    
    class_<InputError, bases<OperatorError> >("InputError", no_init)
        .def("inputId", &InputError::inputId)
    ; 
    
    class_<OutputError, bases<OperatorError> >("OutputError", no_init)
        .def("outputId", &OutputError::outputId)
    ;           
    
    class_<ParameterError, bases<OperatorError> >("ParameterError", no_init)
        .def("parameter", &ParameterError::parameter, return_internal_reference<>())
    ;            
    
    class_<ParameterAccessViolation, bases<ParameterError> >("ParameterAccessViolation", no_init); 
    
    class_<WrongParameterType, bases<ParameterError> >("WrongParameterType", no_init); 
    
    class_<WrongParameterValue, bases<ParameterError> >("WrongParameterValue", no_init); 
     
    class_<WrongParameterId, bases<OperatorError> >("WrongParameterId", no_init);
}
