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

#include <stromx/runtime/Tribool.h>

#include <boost/lambda/lambda.hpp>
#include <boost/python.hpp>

using namespace boost::python;
using namespace stromx::runtime;

namespace
{   
    boost::shared_ptr<Tribool> allocateFromBool(const bool value)
    {
        return boost::shared_ptr<Tribool>(new Tribool(value));
    }
    
    boost::shared_ptr<Tribool> allocate()
    {
        return boost::shared_ptr<Tribool>(new Tribool(), boost::lambda::_1);
    }
}

void exportTribool()
{
    class_<Tribool, bases<Data>, boost::shared_ptr<Tribool> >("Tribool", no_init)
        .def("__init__", make_constructor(&allocate))
        .def("__init__", make_constructor(&allocateFromBool))
        .def("__nonzero__", &Tribool::operator bool)
        .def("undefined", &Tribool::undefined)
        .def(self == self)
        .def(self != self)
    ;
}