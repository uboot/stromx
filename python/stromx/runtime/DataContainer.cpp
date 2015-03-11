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

#include <stromx/runtime/DataContainer.h>
#include <stromx/runtime/Data.h>

#include <memory>
#include <boost/python.hpp>

using namespace boost::python;
using namespace stromx::runtime;

namespace
{
    DataContainer* allocate()
    {
        return new DataContainer();
    }
    
    DataContainer* allocateFromData(std::auto_ptr<Data> data)
    {
        Data* dataPtr = data.get();
        data.release();
        
        return new DataContainer(dataPtr);
    }
}


void exportDataContainer()
{       
    class_<DataContainer>("DataContainer", no_init)
        .def("__init__", make_constructor(&allocate))
        .def("__init__", make_constructor(&allocateFromData))
        .def("empty", &DataContainer::empty)
        .def("release", &DataContainer::release)
    ;
}
