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

#ifndef STROMX_PYTHON_THREADUTILITIES_H
#define STROMX_PYTHON_THREADUTILITIES_H

#include <stromx/runtime/Thread.h>
#include <boost/assert.hpp>

namespace stromx
{
    namespace python
    {
        class ThreadUtilities
        {
        public:
            static bool interruptedFlag(const stromx::runtime::Thread* thread)
            {
                BOOST_ASSERT(thread);
                return thread->interruptedFlag();
            }
            
            static void setInterruptedFlag(stromx::runtime::Thread* thread, const bool value)
            {
                BOOST_ASSERT(thread);
                thread->setInterruptedFlag(value);
            }
        };
    }
}

#endif // STROMX_PYTHON_THREADUTILITIES_H