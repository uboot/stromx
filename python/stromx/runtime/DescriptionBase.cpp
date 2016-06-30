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
#include "stromx/runtime/DescriptionBase.h"
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
        class_<DescriptionBase>("DescriptionBase", no_init)
            .def("title", &DescriptionBase::title, return_value_policy<copy_const_reference>())
            .def("visualization", &DescriptionBase::visualization, return_internal_reference<>())
            .def("id", &DescriptionBase::id)
            .def("variant", &DescriptionBase::variant, return_internal_reference<>())
            .def("min", &DescriptionBase::min, return_internal_reference<>())
            .def("max", &DescriptionBase::max, return_internal_reference<>())
            .def("descriptions", &DescriptionBase::descriptions, return_internal_reference<>())
            .def("rows", &DescriptionBase::rows)
            .def("cols", &DescriptionBase::cols)
            .def("originalType", &DescriptionBase::originalType)
            .def("currentType", &DescriptionBase::currentType)
        ;
                
        enum_<DescriptionBase::Type>("Type")
            .value("NO_TYPE", DescriptionBase::NO_TYPE)
            .value("PARAMETER", DescriptionBase::PARAMETER)
            .value("INPUT", DescriptionBase::INPUT)
            .value("OUTPUT", DescriptionBase::OUTPUT)
        ;
    }
}
