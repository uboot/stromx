/* 
*  Copyright 2015 Matthias Fuchs
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
#include "stromx/runtime/Description.h"
#include "ExportVector.h"

using namespace boost::python;
using namespace stromx::runtime;

void exportDescriptionBase()
{         
    class_<EnumDescription>("EnumDescription", no_init)
        .def("value", &EnumDescription::value)
        .def("title", &EnumDescription::title, return_value_policy<copy_const_reference>())
    ;
    stromx::python::exportVector<EnumDescription>("EnumDescriptionVector");
    
    {
        scope in_DescriptionBase =
        class_<Description>("Description", no_init)
            .def("title", &Description::title, return_value_policy<copy_const_reference>())
            .def("visualization", &Description::visualization, return_internal_reference<>())
            .def("id", &Description::id)
            .def("variant", &Description::variant, return_internal_reference<>())
            .def("min", &Description::min, return_internal_reference<>())
            .def("max", &Description::max, return_internal_reference<>())
            .def("descriptions", &Description::descriptions, return_internal_reference<>())
            .def("rows", &Description::rows)
            .def("cols", &Description::cols)
            .def("originalType", &Description::originalType)
            .def("currentType", &Description::currentType)
        ;
                
        enum_<Description::Type>("Type")
            .value("NO_TYPE", Description::NO_TYPE)
            .value("PARAMETER", Description::PARAMETER)
            .value("INPUT", Description::INPUT)
            .value("OUTPUT", Description::OUTPUT)
        ;
        
        enum_<Description::UpdateBehavior>("UpdateBehavior")
            .value("PERSISTENT", Description::PERSISTENT)
            .value("PUSH", Description::PUSH)
            .value("PULL", Description::PULL)
            .value("INTERNAL", Description::INTERNAL)
        ;
    }
}
