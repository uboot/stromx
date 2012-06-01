/* 
*  Copyright 2012 Matthias Fuchs
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

#include <stromx/core/Documentation.h>

#include <boost/python.hpp>

using namespace boost::python;
using namespace stromx::core;

void exportDocumentation()
{         
    class_<Documentation>("Documentation", no_init)
        .def("title", &Documentation::title, return_value_policy<copy_const_reference>())
        .def("details", &Documentation::details, return_value_policy<copy_const_reference>())
    ;
}