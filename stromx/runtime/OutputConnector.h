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

#ifndef STROMX_RUNTIME_OUTPUTCONNECTOR_H
#define STROMX_RUNTIME_OUTPUTCONNECTOR_H

#include "stromx/runtime/Connector.h"

namespace stromx
{
    namespace runtime
    {
        /** \brief Identifier of an output of an operator. */
        class OutputConnector : public Connector
        {
        public:
            /** 
             * Constructs an output connector which points to the output
             * \c id of the operator \c op.
             */
            OutputConnector(const Operator* const op, const unsigned int id)
              : Connector(Connector::OUTPUT, op, id)
            {}
            
            /** Constructs an invalid output connector. */
            OutputConnector()
              : Connector(Connector::OUTPUT, 0, 0)
            {}
        };
    }
}

#endif // STROMX_RUNTIME_OUTPUTCONNECTOR_H
