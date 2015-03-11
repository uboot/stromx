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

#include <stromx/runtime/Data.h>
#include <stromx/runtime/Version.h>

#include <boost/python.hpp>

#ifdef __GNUG__
    #include <tr1/memory>
#else
    #include <memory>
#endif

using namespace boost::python;
using namespace stromx::runtime;
    
namespace
{       
    struct DataWrap : Data, wrapper<Data>
    {
        const Version & version() const
        {
            return this->get_override("version")();
        }
        
        const std::string & type() const
        {
            return this->get_override("type")();
        }
        
        const std::string & package() const
        {
            return this->get_override("package")();
        }
        
        const VariantHandle & variant() const
        {
            return this->get_override("variant")();
        }
        
        Data* clone() const
        {
            return this->get_override("clone")();
        }
    };
}

void exportData()
{                 
    class_<DataWrap, boost::noncopyable>("Data")
        .def("version", pure_virtual(&Data::version), return_internal_reference<>())
        .def("type", pure_virtual(&Data::type), return_value_policy<copy_const_reference>())
        .def("package", pure_virtual(&Data::package), return_value_policy<copy_const_reference>())
        .def("variant", pure_virtual(&Data::variant), return_internal_reference<>())
        .def("isVariant", &Data::isVariant)
    ;
}
