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
#include <vector>

using namespace boost::python;

namespace stromx
{
    namespace python
    {
        template <class T>
        void exportVector(const char* const name)
        {
            class_< std::vector<T> >(name, no_init)
                .def("__len__", &std::vector<T>::size)
                .def< typename std::vector<T>::reference (std::vector<T>::*)(typename std::vector<T>::size_type)>("__getitem__", &std::vector<T>::at, return_internal_reference<>())
                .def("__iter__", iterator< std::vector<T>, return_internal_reference<> >())
            ;
        }
    }
}
