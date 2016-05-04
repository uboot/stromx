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

#include "ExportVector.h"

#include <boost/lambda/lambda.hpp>
#include <boost/python.hpp>
#include <stromx/runtime/List.h>
#include "DataCast.h"

using namespace boost::python;
using namespace stromx::runtime;


namespace
{   
    boost::shared_ptr<List> allocate()
    {
        return boost::shared_ptr<List>(new List(), boost::lambda::_1);
    }
}

void exportList()
{
    stromx::python::exportVector<Data*>("DataVector");
    
    class_<List, bases<Data>, boost::shared_ptr<List> >("List", no_init)
        .def("__init__", make_constructor(&allocate))
        .def<std::vector<Data*> & (List::*)()>("content", &List::content, return_internal_reference<>())
        .def("data_cast", &stromx::python::data_cast<List>, return_internal_reference<1>())
        .staticmethod("data_cast")
    ;
}