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

#ifndef STROMX_RUNTIME_CONNECTORPARAMETER_H
#define STROMX_RUNTIME_CONNECTORPARAMETER_H

#include "stromx/runtime/Parameter.h"

namespace stromx
{
    namespace runtime
    {
        class ConnectorParameter : public Parameter
        {
        public:
            ConnectorParameter(const DescriptionBase* const connector, 
                               const UpdateBehavior behavior, const Type type);
            
            virtual Type originalType() const { return m_originalType; }
            
            virtual const std::vector<EnumDescription> & descriptions() const { return m_connector->descriptions(); }
            
            virtual const Data& max() const { return m_connector->max(); }
            virtual const Data& min() const { return m_connector->min(); }
            virtual const Data& step() const { return m_connector->step(); }
            
            virtual unsigned int rows() const { return m_connector->rows(); }
            virtual unsigned int cols() const { return m_connector->cols(); }
            
        private:
            const DescriptionBase* m_connector;
            const Type m_originalType;
        };
    }
}

#endif // STROMX_RUNTIME_CONNECTORPARAMETER_H