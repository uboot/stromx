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

#include <stromx/core/OperatorKernel.h>

#include <boost/python.hpp>

using namespace boost::python;
using namespace stromx::core;

namespace
{
    struct OperatorKernelWrap : public OperatorKernel, wrapper<OperatorKernel>
    {
        virtual OperatorKernel* const clone() const
        {
            return this->get_override("clone")();
        }
        
        virtual void execute(DataProvider& provider)
        {
            this->get_override("execute")();
        }
    };
}
      
void exportOperatorKernel()
{      
    stromx::python::exportVector<const Description*>("DescriptionVector");
    stromx::python::exportVector<const Parameter*>("ParameterVector");
      
    class_<OperatorKernelWrap, boost::noncopyable>("OperatorKernel", no_init)
        .def("type", &OperatorKernelWrap::type, return_value_policy<copy_const_reference>())
        .def("package", &OperatorKernelWrap::package, return_value_policy<copy_const_reference>())
        .def("version", &OperatorKernelWrap::version, return_internal_reference<>())
        .def("inputs", &OperatorKernelWrap::inputs, return_internal_reference<>())
        .def("outputs", &OperatorKernelWrap::outputs, return_internal_reference<>())
        .def("parameters", &OperatorKernelWrap::parameters, return_internal_reference<>())
        .def("parameter", static_cast<const Parameter &(OperatorKernelWrap::*)(const unsigned int) const>(&OperatorKernelWrap::parameter), return_internal_reference<>())
    ;
}

            

