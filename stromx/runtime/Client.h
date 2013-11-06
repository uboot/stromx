/* 
 *  Copyright 2013 Matthias Fuchs
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

#ifndef STROMX_RUNTIME_CLIENT_H
#define STROMX_RUNTIME_CLIENT_H

#include "stromx/runtime/OperatorKernel.h"
#include "stromx/runtime/String.h"

namespace stromx
{
    namespace runtime
    {
        /** \brief Distributes input data to TCP clients. */
        class STROMX_RUNTIME_API Client : public OperatorKernel
        {
        public:
            enum OutputId
            {
                OUTPUT
            };
            
            enum ParameterId
            {
                URL,
                PORT
            };
            
            Client();
            
            virtual OperatorKernel* clone() const { return new Client; }
            virtual void setParameter(const unsigned int id, const Data& value);
            const DataRef getParameter(const unsigned int id) const;
            virtual void initialize();
            virtual void execute(DataProvider& provider);
            
        private:
            const static std::string LINE_DELIMITER;
            
            static const std::vector<const Description*> setupInputs();
            static const std::vector<const Parameter*> setupParameters();
            const std::vector<const Description*> setupOutputs() const;
            
            static const std::string TYPE;
            static const std::string PACKAGE;
            static const Version VERSION;
            
            static const unsigned int MIN_PORT;
            static const unsigned int MAX_PORT;
            
            String m_url;
            UInt16 m_port;
        };
    }
}

#endif // STROMX_RUNTIME_CLIENT_H
