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
            
        const std::vector<const Description*>& inputs() const
        {
            return this->get_override("inputs")();
        }
            
        const std::vector<const Description*>& outputs() const
        {
            return this->get_override("outputs")();
        }
            
        const std::vector<const Parameter*>& parameters() const
        {
            return this->get_override("parameters")();
        }
            
        const Parameter & parameter(const unsigned int id) const
        {
            return this->get_override("parameter")();
        }
    };
}
      
void exportOperatorInfo()
{      
    stromx::python::exportConstPtrVector<Description>("DescriptionVector");
    stromx::python::exportConstPtrVector<Parameter>("ParameterVector");
      
    class_<OperatorInfoWrap, boost::noncopyable>("OperatorInfo", no_init)
        .def("type", pure_virtual(&OperatorInfo::type), return_value_policy<copy_const_reference>())
        .def("package", pure_virtual(&OperatorInfo::package), return_value_policy<copy_const_reference>())
        .def("version", pure_virtual(&OperatorInfo::version), return_internal_reference<>())
        .def("inputs", pure_virtual(&OperatorInfo::inputs), return_internal_reference<>())
        .def("outputs", pure_virtual(&OperatorInfo::outputs), return_internal_reference<>())
        .def("parameters", pure_virtual(&OperatorInfo::parameters), return_internal_reference<>())
        .def("parameter", pure_virtual(&OperatorInfo::parameter), return_internal_reference<>())
    ;
}

            

