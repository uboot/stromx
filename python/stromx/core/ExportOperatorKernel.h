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

#include <boost/python.hpp>

#include <stromx/core/OperatorKernel.h>

void exportImage();

using namespace stromx::core;
using namespace boost::python;

namespace stromx
{
    namespace python
    {
        template <class operator_t>
        std::auto_ptr<OperatorKernel> allocate()
        {
            return std::auto_ptr<OperatorKernel>(new operator_t);
        }
        
        template <class operator_t>
        void exportOperatorKernel(const char* const name)
        {
            class_<operator_t, bases<OperatorKernel>, std::auto_ptr<operator_t>, boost::noncopyable>(name, no_init)
                .def("__init__", make_constructor(&allocate<operator_t>))
            ;
            
            implicitly_convertible< std::auto_ptr<operator_t>, std::auto_ptr<OperatorKernel> >();
        }
    }
}