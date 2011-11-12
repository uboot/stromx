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

#include <stromx/core/Stream.h>
#include <stromx/core/Operator.h>

#include <boost/python.hpp>
#include <vector>

using namespace boost::python;
using namespace stromx::core;

namespace
{
    class StreamWrap : public Stream
    {
    public:
        void addOperator(std::auto_ptr<Operator> op)
        {
            Stream::addOperator(op.get());
            op.release();
        }
    };
}

void exportStream()
{    
    stromx::python::exportVector<Operator*>("OperatorList");
    
    enum_<Stream::Status>("StreamStatus")
        .value("INACTIVE", Stream::INACTIVE)
        .value("ACTIVE", Stream::ACTIVE)
        .value("DEACTIVATING", Stream::DEACTIVATING)
    ;
        
    class_<StreamWrap>("Stream")
        .def("name", &StreamWrap::name, return_value_policy<copy_const_reference>())
        .def("setName", &StreamWrap::setName)
        .def("status", &StreamWrap::status)
        .def("operators", &StreamWrap::operators, return_internal_reference<>())
        .def("addOperator", &StreamWrap::addOperator)
        .def("removeOperator", &StreamWrap::removeOperator)
        .def("start", &StreamWrap::start)
        .def("stop", &StreamWrap::stop)
        .def("join", &StreamWrap::join)
    ;
}