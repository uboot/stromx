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

#include <stromx/runtime/VariantInterface.h>
#include <stromx/runtime/VariantHandle.h>

#include <boost/python.hpp>

using namespace boost::python;
using namespace stromx::runtime;

namespace
{       
    struct VariantInterfaceWrap : VariantInterface, wrapper<VariantInterface>
    {
        unsigned int id() const
        {
            return this->get_override("id")();
        }
        
        const std::string & package() const
        {
            return this->get_override("package")();
        }
        
        const std::string title() const
        {
            return this->get_override("title")();
        }
        
        bool isVariant(const VariantInterface& variant) const
        {
            return this->get_override("isVariant")(variant);
        }
    };
}

void exportVariantInterface()
{         
    scope in_VariantInterface =
    class_<VariantInterfaceWrap, boost::noncopyable>("VariantInterface", no_init)
        .def("title", pure_virtual(&VariantInterface::title))
        .def("isVariant", pure_virtual(&VariantInterface::isVariant))
        .def("id", pure_virtual(&VariantInterface::id))
        .def("package", pure_virtual(&VariantInterface::package), return_value_policy<copy_const_reference>())
        .def("compositeType", &VariantInterface::compositeType)
        .def("lhs", &VariantInterface::lhs)
        .def("rhs", &VariantInterface::rhs)
    ;
    
    enum_<VariantInterface::CompositeType>("CompositeType")
        .value("NO_COMPOSITE", VariantInterface::NO_COMPOSITE)
        .value("AND_COMPOSITE", VariantInterface::AND_COMPOSITE)
        .value("OR_COMPOSITE", VariantInterface::OR_COMPOSITE)
    ;
}
