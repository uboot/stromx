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

#include <stromx/runtime/OperatorKernel.h>
#include <stromx/runtime/Factory.h>
#include <stromx/runtime/Data.h>

#include <boost/lambda/lambda.hpp>
#include <boost/python.hpp>

using namespace boost::python;
using namespace stromx::runtime;


namespace
{
    boost::shared_ptr<OperatorKernel> newOperatorWrap(const Factory & factory, const std::string & package, const std::string & name)
    {
        return boost::shared_ptr<OperatorKernel>(factory.newOperator(package, name), boost::lambda::_1);
    }
        
    boost::shared_ptr<Data> newDataWrap(const Factory & factory, const std::string & package, const std::string & name)
    {
        return boost::shared_ptr<Data>(factory.newData(package, name), boost::lambda::_1);
    }
}


void exportFactory()
{          
    stromx::python::exportVector<const OperatorKernel*>("OperatorKernelVector");
    stromx::python::exportVector<const Data*>("DataVector");
    
    class_<Factory, bases<AbstractFactory, Registry> >("Factory")
        .def("registerOperator", &Factory::registerOperator)
        .def("registerData", &Factory::registerData)
        .def("newOperator", &newOperatorWrap)
        .def("newData", &newDataWrap)
        .def("availableOperators", &Factory::availableOperators, return_internal_reference<>())
        .def("availableData", &Factory::availableData, return_internal_reference<>())
    ;
}