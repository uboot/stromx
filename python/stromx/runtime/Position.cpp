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

#include <stromx/runtime/Position.h>

#include <boost/python.hpp>

using namespace boost::python;
using namespace stromx::runtime;

void exportPosition()
{       
    class_<Position>("Position", init<float, float>())
        .def("x", &Position::x)
        .def("y", &Position::y)
        .def("setX", &Position::setX)
        .def("setY", &Position::setY)
    ;
}