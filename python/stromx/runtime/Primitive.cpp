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

#include <stromx/runtime/Enum.h>
#include <stromx/runtime/Primitive.h>

#include <boost/python.hpp>

using namespace boost::python;
using namespace stromx::runtime;

namespace
{   
    template <class repr_t, class primitive_t>
    std::auto_ptr<primitive_t> allocate(const repr_t value)
    {
        return std::auto_ptr<primitive_t>(new primitive_t(value));
    }
    
    template <class primitive_t>
    std::auto_ptr<primitive_t> allocate()
    {
        return std::auto_ptr<primitive_t>(new primitive_t());
    }
    
    template <class repr_t, class primitive_t>
    void primitive(const char* name)
    {
        class_<primitive_t, bases<Data>, std::auto_ptr<primitive_t> >(name, no_init)
            .def("__init__", make_constructor(&allocate<primitive_t>))
            .def("__init__", make_constructor(&allocate<repr_t, primitive_t>))
            .def("get", &primitive_t::get)
        ;
        
        implicitly_convertible< std::auto_ptr<primitive_t>, std::auto_ptr<Data> >();
    }
}

void exportPrimitive()
{
    primitive<bool, Bool>("Bool");
    primitive<unsigned int, Enum>("Enum");
    primitive<int, Int8>("Int8");
    primitive<unsigned int, UInt8>("UInt8");
    primitive<int, Int16>("Int16");
    primitive<unsigned int, UInt16>("UInt16");
    primitive<int, Int32>("Int32");
    primitive<unsigned int, UInt32>("UInt32");
    primitive<int, Int64>("Int64");
    primitive<unsigned int, UInt64>("UInt64");
    primitive<double, Float32>("Float32");
    primitive<float, Float64>("Float64");
}
