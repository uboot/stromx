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

#include <stromx/core/ExceptionObserver.h>
#include <stromx/core/Operator.h>
#include <stromx/core/Stream.h>
#include <stromx/core/Thread.h>

#include <boost/python.hpp>

using namespace boost::python;
using namespace stromx::core;

namespace
{
    void joinWrap(Stream & stream)
    {
        Py_BEGIN_ALLOW_THREADS
        try
        {
            stream.join();
        }
        catch(stromx::core::Exception&)
        {
            Py_BLOCK_THREADS
            throw;
        }
        Py_END_ALLOW_THREADS
    }
    
    Operator* addOperatorWrap(Stream& stream, std::auto_ptr<Operator> op)
    {
        Operator* opPtr = op.get();
        stream.addOperator(opPtr);
        op.release();
        return opPtr;
    }
}

void exportStream()
{    
    stromx::python::exportVector<Operator*>("OperatorVector");
    stromx::python::exportVector<Thread*>("ThreadVector");
        
    {
        scope in_Stream = 
        class_<Stream, boost::noncopyable>("Stream")
            .def("name", &Stream::name, return_value_policy<copy_const_reference>())
            .def("setName", &Stream::setName)
            .def("status", &Stream::status)
            .def("operators", &Stream::operators, return_internal_reference<>())
            .def("addOperator", &addOperatorWrap, return_internal_reference<>())
            .def("removeOperator", &Stream::removeOperator)
            .def("connect", &Stream::connect)
            .def("disconnect", &Stream::disconnect)
            .def("connectionSource", &Stream::connectionSource)
            .def("addThread", reinterpret_cast<Thread* (Stream::*)()>(&Stream::addThread), return_internal_reference<>())
            .def("removeThread", &Stream::removeThread)
            .def("threads", &Stream::threads, return_internal_reference<>())
            .def("addObserver", &Stream::addObserver)
            .def("removeObserver", &Stream::removeObserver)
            .def("start", &Stream::start)
            .def("stop", &Stream::stop)
            .def("join", &joinWrap)
            .def("pause", &Stream::pause)
            .def("resume", &Stream::resume)
        ;
        
        enum_<Stream::Status>("Status")
            .value("INACTIVE", Stream::INACTIVE)
            .value("ACTIVE", Stream::ACTIVE)
            .value("DEACTIVATING", Stream::DEACTIVATING)
            .value("PAUSED", Stream::PAUSED)
        ;
    }
}