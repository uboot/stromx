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

#ifndef STROMX_PYTHON_DATACAST_H
#define STROMX_PYTHON_DATACAST_H

#include <boost/python.hpp>

#include <stromx/runtime/Data.h>

void exportImage();

using namespace stromx::runtime;
using namespace boost::python;

namespace stromx
{
    namespace python
    {
        template <class data_t>
        data_t* data_cast(Data* data)
        {
            return runtime::data_cast<data_t>(data);
        }
    }
}

#endif // STROMX_PYTHON_DATACAST_H