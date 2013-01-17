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

#include <stromx/runtime/WriteAccess.h>
#include <stromx/runtime/Data.h>

#include <boost/python.hpp>

using namespace boost::python;
using namespace stromx::runtime;

namespace
{   
    std::auto_ptr< WriteAccess<> > allocate(const DataContainer & data)
    {
        WriteAccess<>* access = 0;
        
        Py_BEGIN_ALLOW_THREADS
        access = new WriteAccess<>(data);
        Py_END_ALLOW_THREADS
        
        return std::auto_ptr< WriteAccess<> >(access);
    }
    
    std::auto_ptr< WriteAccess<> > allocateWithTimeout(const DataContainer & data, const unsigned int timeout)
    {
        WriteAccess<>* access = 0;
        
        Py_BEGIN_ALLOW_THREADS
        access = new WriteAccess<>(data, timeout);
        Py_END_ALLOW_THREADS
        
        return std::auto_ptr< WriteAccess<> >(access);
    }
}

void exportWriteAccess()
{       
    class_<WriteAccess<> >("WriteAccess", no_init)
        .def("__init__", make_constructor(&allocate))
        .def("__init__", make_constructor(&allocateWithTimeout))
        .def("get", &WriteAccess<>::get, return_internal_reference<>())
    ;
}
