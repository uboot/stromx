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

#include <stromx/core/Primitive.h>

#include <boost/python.hpp>

using namespace boost::python;
using namespace stromx::core;

void exportPrimitive()
{
    class_<Bool, bases<Data> >("Bool")
        .def(init<>())
        .def(init<bool>())
        .def("get", &Bool::get)
        .def("data_cast", &data_cast<Bool &>, return_internal_reference<1>())
        .staticmethod("data_cast")
    ;
    
    class_<Int8, bases<Data> >("Int8")
        .def(init<>())
        .def(init<int>())
        .def("get", &Int8::get)
        .def("data_cast", &data_cast<Int8 &>, return_internal_reference<1>())
        .staticmethod("data_cast")
    ;
    
    class_<UInt32, bases<Data> >("UInt32")
        .def(init<>())
        .def(init<unsigned int>())
        .def("get", &UInt32::get)
        .def("data_cast", &data_cast<UInt32 &>, return_internal_reference<1>())
        .staticmethod("data_cast")
    ;
}
