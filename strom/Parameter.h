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

#ifndef STROM_PARAMETER_H
#define STROM_PARAMETER_H

#include "Description.h"

namespace strom
{
    /** \brief %Description of parameter */
    class Parameter : public Description
    {
    public:
        enum AccessMode
        {
            NO_ACCESS,
            NONE_READ,
            NONE_WRITE,
            INITIALIZED_READ,
            INITIALIZED_WRITE,
            ACTIVATED_WRITE
        };
        
        Parameter(const unsigned int id, const DataVariant& type)
          : Description(id, type),
            m_access(NO_ACCESS)
        {}
        
        const AccessMode accessMode() const { return m_access; }
        
        void setAccessMode(const AccessMode mode) { m_access = mode; }
        
    private:
        AccessMode m_access;
    };
}

#endif // STROM_PARAMETER_H