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

#include <stromx/core/DataContainer.h>
#include <stromx/core/DataVariant.h>
#include <stromx/core/Data.h>

#include <auto_ptr.h>
#include <boost/python.hpp>

using namespace boost::python;
using namespace stromx::core;

namespace
{
    class DataContainerWrap : public DataContainer
    {
    public:
        DataContainerWrap(std::auto_ptr<Data> data)
          : DataContainer(data.get())
        {
            data.release();
        } 
    };
    
    DataContainerWrap* allocate(std::auto_ptr<Data> data)
    {
        return new DataContainerWrap(data);
    }
}


void exportDataContainer()
{       
    class_<DataContainerWrap>("DataContainer", no_init)
        .def("__init__", make_constructor(&allocate))
        .def("empty", &DataContainerWrap::empty)
    ;
    
    implicitly_convertible<DataContainerWrap, DataContainer>();
}
