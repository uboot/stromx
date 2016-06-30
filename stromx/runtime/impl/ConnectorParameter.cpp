/* 
 *  Copyright 2016 Matthias Fuchs
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

#include "stromx/runtime/impl/ConnectorParameter.h"

namespace stromx
{
    namespace runtime
    {
        namespace impl
        {
            ConnectorParameter::ConnectorParameter(const Description* const connector, const UpdateBehavior behavior)
              : Parameter(connector->id(), connector->variant()),
                m_connector(connector)
            {
                setTitle(connector->title());
                setVisualization(connector->visualization());
                setUpdateBehavior(behavior);
                
                switch (connector->originalType())
                {
                case Description::INPUT:
                    setAccessMode(ACTIVATED_WRITE);
                    break;
                case Description::OUTPUT:
                    setAccessMode(INITIALIZED_READ);
                    break;
                default:
                    throw InternalError("A connector parameter can only be created from inputs or outputs");
                }
            }
        }
    }
}