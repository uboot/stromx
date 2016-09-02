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

#include <stromx/runtime/Input.h>
#include <stromx/runtime/OperatorKernel.h>
#include <stromx/runtime/Output.h>

#include <boost/python.hpp>

using namespace boost::python;
using namespace stromx::runtime;

namespace
{
    struct OperatorInfoWrap : public OperatorInfo, wrapper<OperatorInfo>
    {
        const std::string& type() const
        {
            return this->get_override("type")();
        }
            
        const std::string& package() const
        {
            return this->get_override("package")();
        }
            
        const Version& version() const
        {
            return this->get_override("version")();
        }
            
        const std::vector<const Input*>& inputs() const
        {
            return this->get_override("inputs")();
        }
            
        const std::vector<const Output*>& outputs() const
        {
            return this->get_override("outputs")();
        }
            
        const std::vector<const Parameter*>& parameters() const
        {
            return this->get_override("parameters")();
        }
            
        const Parameter & parameter(const unsigned int id) const
        {
            return this->get_override("parameter")(id);
        }
            
        const Input & input(const unsigned int id) const
        {
            return this->get_override("input")(id);
        }
            
        const Output & output(const unsigned int id) const
        {
            return this->get_override("output")(id);
        }
            
        const Description & description(const unsigned int id) const
        {
            return this->get_override("description")(id);
        }
    };
}
      
void exportOperatorInfo()
{      
    stromx::python::exportVector<const Input*>("InputVector");
    stromx::python::exportVector<const Output*>("OutputVector");
    stromx::python::exportVector<const Parameter*>("ParameterVector");
      
    class_<OperatorInfoWrap, boost::noncopyable>("OperatorInfo", no_init)
        .def("type", pure_virtual(&OperatorInfo::type), return_value_policy<copy_const_reference>())
        .def("package", pure_virtual(&OperatorInfo::package), return_value_policy<copy_const_reference>())
        .def("version", pure_virtual(&OperatorInfo::version), return_internal_reference<>())
        .def("inputs", pure_virtual(&OperatorInfo::inputs), return_internal_reference<>())
        .def("outputs", pure_virtual(&OperatorInfo::outputs), return_internal_reference<>())
        .def("parameters", pure_virtual(&OperatorInfo::parameters), return_internal_reference<>())
        .def("parameter", pure_virtual(&OperatorInfo::parameter), return_internal_reference<>())
        .def("input", pure_virtual(&OperatorInfo::input), return_internal_reference<>())
        .def("output", pure_virtual(&OperatorInfo::output), return_internal_reference<>())
        .def("description", pure_virtual(&OperatorInfo::description), return_internal_reference<>())
    ;
}

            

