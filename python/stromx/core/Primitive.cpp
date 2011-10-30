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

namespace
{   
    template <class repr_t, class val_t>
    void primitive(const char* name)
    {
        class_<val_t, bases<Data> >(name)
            .def(init<>())
            .def(init<repr_t>())
            .def("get", &val_t::get)
            .def("data_cast", &data_cast<val_t &>, return_internal_reference<1>())
            .staticmethod("data_cast")
        ;
    }
}

void exportPrimitive()
{
    primitive<bool, Bool>("Bool");
    primitive<int, Int8>("Int8");
    primitive<unsigned int, UInt8>("UInt8");
    primitive<int, Int16>("Int16");
    primitive<unsigned int, UInt16>("UInt16");
    primitive<int, Int32>("Int32");
    primitive<unsigned int, UInt32>("UInt32");
    primitive<double, Float>("Float");
    primitive<float, Double>("Double");
}
