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

#ifndef STROMX_CORE_DOCUMENTATION_H
#define STROMX_CORE_DOCUMENTATION_H

#include "Config.h"

#include <string>

namespace stromx
{
    namespace core
    {
        /** 
         * \brief User documentation of an object.
         *
         * This class implements a documentation of objects which targets
         * the end-user. A documentation consists of a \em title which is a one-line
         * description of the documented object and \em details which provide more 
         * information.
         */
        class STROMX_CORE_API Documentation
        {
        public:
            /** 
             * Constructs an empty documentation.
             */
            Documentation() {}
            
            /** 
             * Constructs a documentation.
             * 
             * \param title A short (at most one line) title without formatting.
             */
            Documentation(const std::string & title);
            
            /** 
             * Constructs a documentation.
             * 
             * \param title A short (at most one line) title without formatting.
             * \param details Detailed description of the object with basic HTML formatting.
             */
            Documentation(const std::string & title, const std::string & details);
            
            /** Returns the the title. */
            const std::string & title() { return m_title; }
            
            /** Returns the details. */
            const std::string & details() { return m_details; }
            
        private:
            std::string m_title;
            std::string m_details;
        };
    }
}

#endif // STROMX_CORE_DOCUMENTATION_H
