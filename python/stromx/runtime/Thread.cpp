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

#include "ExportVector.h"

#include <boost/python.hpp>

#include <stromx/runtime/Thread.h>
#include <stromx/runtime/Operator.h>

#include "Equality.h"

using namespace boost::python;
using namespace stromx::runtime;

void exportThread()
{               
    stromx::python::exportVector<Input>("InputVector");
     
    {
        scope in_Thread =
        class_<Thread, boost::noncopyable>("Thread", no_init)
            .def("status", &Thread::status)
            .def("name", &Thread::name, return_value_policy<copy_const_reference>())
            .def("setName", &Thread::setName)
            .def("color", &Thread::color, return_internal_reference<>())
            .def("setColor", &Thread::setColor)
            .def("inputSequence", &Thread::inputSequence, return_internal_reference<>())
            .def("addInput", &Thread::addInput)
            .def("insertInput", &Thread::insertInput)
            .def<void (Thread::*)(unsigned int const)>("removeInput", &Thread::removeInput)
            .def<void (Thread::*)(Operator* const, unsigned int)>("removeInput", &Thread::removeInput)
            .def("__eq__", &stromx::python::eq<Thread>)
            .def("__ne__", &stromx::python::ne<Thread>)
        ;        
            
        enum_<Thread::Status>("Status")
            .value("INACTIVE", Thread::INACTIVE)
            .value("ACTIVE", Thread::ACTIVE)
            .value("DEACTIVATING", Thread::DEACTIVATING)
            ;
            
        register_ptr_to_python< boost::shared_ptr<Thread> >();
    }
}
