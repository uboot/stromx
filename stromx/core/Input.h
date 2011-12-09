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

#ifndef STROMX_CORE_INPUT_H
#define STROMX_CORE_INPUT_H

#include "Connector.h"

namespace stromx
{
    namespace core
    {
        /** \brief Identifier of an input connector of an operator. */
        class Input : public Connector
        {
        public:
            /** 
             * Constructs an input connector which points to the input
             * \c id of the operator \c op.
             */
            Input(const Operator* const op, const unsigned int id)
              : Connector(Connector::INPUT, op, id)
            {}
            
            /** Constructs an invalid input connector. */
            Input()
              : Connector(Connector::INPUT, 0, 0)
            {}
        };
    }
}

#endif // STROMX_CORE_INPUT_H
