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
#include <stromx/core/Parameter.h>
#include "ExportVector.h"

using namespace boost::python;
using namespace stromx::core;

void exportParameter()
{       
    class_<EnumDescription>("EnumDescription", no_init)
        .def("value", &EnumDescription::value)
        .def("title", &EnumDescription::title, return_internal_reference<>())
    ;
    
    stromx::python::exportVector<EnumDescription>("EnumDescriptionVector");
       
    {
        scope in_Parameter =
        class_<Parameter, bases<Description> >("Parameter", no_init)
            .def("accessMode", &Parameter::accessMode)
            .def("min", &Parameter::min, return_internal_reference<>())
            .def("max", &Parameter::max, return_internal_reference<>())
            .def("descriptions", &Parameter::descriptions, return_internal_reference<>())
            .def("group", reinterpret_cast<const Parameter* (Parameter::*)() const>(&Parameter::group), return_internal_reference<>())
            .def("members", &Parameter::members, return_internal_reference<>())
        ;
            
        enum_<Parameter::AccessMode>("AccessMode")
            .value("NO_ACCESS", Parameter::NO_ACCESS)
            .value("NONE_READ", Parameter::NONE_READ)
            .value("NONE_WRITE", Parameter::NONE_WRITE)
            .value("INITIALIZED_READ", Parameter::INITIALIZED_READ)
            .value("INITIALIZED_WRITE", Parameter::INITIALIZED_WRITE)
            .value("ACTIVATED_WRITE", Parameter::ACTIVATED_WRITE)
        ;
    }
    
    
}
