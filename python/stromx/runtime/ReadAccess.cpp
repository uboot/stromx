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

#include "ThreadUtilities.h"

#include <stromx/runtime/ReadAccess.h>
#include <stromx/runtime/Data.h>

#include <boost/python.hpp>
#include <boost/thread.hpp>

using namespace boost::python;
using namespace stromx::python;
using namespace stromx::runtime;

namespace stromx
{
    namespace runtime
    {
        extern boost::thread_specific_ptr<Thread> gThread;
    }
}

namespace
{   
    boost::shared_ptr<ReadAccess> allocate(const DataContainer & data)
    {
        ReadAccess* access = 0;
        
        Thread* threadPtr = gThread.get();
        if (threadPtr)
            ThreadUtilities::setInterruptedFlag(threadPtr, false);
            
        Py_BEGIN_ALLOW_THREADS
        try
        {
            access = new ReadAccess(data);
        }
        catch(stromx::runtime::Interrupt&)
        {
            Py_BLOCK_THREADS
            if (threadPtr)
                ThreadUtilities::setInterruptedFlag(threadPtr, true);
            throw;
        }
        catch(stromx::runtime::Exception&)
        {
            Py_BLOCK_THREADS
            throw;
        }
        Py_END_ALLOW_THREADS
        
        return boost::shared_ptr<ReadAccess>(access);
    }
    
    boost::shared_ptr<ReadAccess> allocateWithTimeout(const DataContainer & data, const unsigned int timeout)
    {
        ReadAccess* access = 0;
        
        Thread* threadPtr = gThread.get();
        if (threadPtr)
            ThreadUtilities::setInterruptedFlag(threadPtr, false);
        
        Py_BEGIN_ALLOW_THREADS
        try
        {
            access = new ReadAccess(data, timeout);
        }
        catch(stromx::runtime::Interrupt&)
        {
            Py_BLOCK_THREADS
            if (threadPtr)
                ThreadUtilities::setInterruptedFlag(threadPtr, true);
            throw;
        }
        catch(stromx::runtime::Exception&)
        {
            Py_BLOCK_THREADS
            throw;
        }
        Py_END_ALLOW_THREADS
        
        return boost::shared_ptr<ReadAccess>(access);
    }
}

void exportReadAccess()
{       
    class_<ReadAccess>("ReadAccess", no_init)
        .def("__init__", make_constructor(&allocate))
        .def("__init__", make_constructor(&allocateWithTimeout))
        .def<const Data & (ReadAccess::*)() const>("get", &ReadAccess::get, return_internal_reference<>())
        .def("empty", &ReadAccess::empty)
        .def("release", &ReadAccess::release)
    ;
}
