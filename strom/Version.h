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

#ifndef STROM_VERSION_H
#define STROM_VERSION_H

namespace strom
{
    /** \brief A version of an operator or data type */
    class Version
    {
    public:
        Version(const unsigned int major, const unsigned int minor)
          : m_major(major),
            m_minor(minor)
        {}
        
        const unsigned int major() const { return m_major; }
        const unsigned int minor() const { return m_minor; }
            
    private:
        unsigned int m_major;
        unsigned int m_minor;
    };
}

#endif // STROM_VERSION_H