/* 
*  Copyright 2012 Matthias Fuchs
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

#ifndef STROMX_PYTHON_EQUALITY_H
#define STROMX_PYTHON_EQUALITY_H

#include <boost/python.hpp>

using namespace boost::python;

namespace stromx
{
    namespace python
    {
        template <class T>
        bool eq(T* lhs, T* rhs)
        {
            return lhs == rhs;
        }
        
        template <class T>
        bool ne(T* lhs, T* rhs)
        {
            return lhs != rhs;
        }
        
    }
}

#endif // STROMX_PYTHON_ALLOWTHREADS_H
