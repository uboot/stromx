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
#include <stromx/runtime/DataProvider.h>

#include <boost/python.hpp>

using namespace boost::python;
using namespace stromx::runtime;

namespace
{
    struct OperatorKernelWrap : public OperatorKernel, wrapper<OperatorKernel>
    {
        virtual OperatorKernel* clone() const
        {
            return this->get_override("clone")();
        }
        
        virtual void execute(DataProvider& provider)
        {
            this->get_override("execute")(provider);
        }
    };
}
      
void exportOperatorKernel()
{      
    class_<OperatorKernelWrap, std::auto_ptr<OperatorKernel>, bases<OperatorInfo>, boost::noncopyable>("OperatorKernel", no_init)
    ;
}

            

