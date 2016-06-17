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

#ifndef STROMX_RUNTIME_SEND_H
#define STROMX_RUNTIME_SEND_H

#include "stromx/runtime/OperatorKernel.h"

namespace stromx
{
    namespace runtime
    {
        namespace impl
        {
            class Server;
        }
        
        /** \brief Distributes input data to TCP clients. */
        class STROMX_RUNTIME_API Send : public OperatorKernel
        {
        public:
            enum DataId
            {
                INPUT,
                PORT
            };
            
            Send ();
            virtual ~Send();
            
            virtual OperatorKernel* clone() const { return new Send; }
            virtual void setParameter(const unsigned int id, const Data& value);
            const DataRef getParameter(const unsigned int id) const;
            virtual void initialize();
            virtual void activate();
            virtual void deactivate();
            virtual void execute(DataProvider& provider);
            
        private:
            const static std::string LINE_DELIMITER;
            
            static const std::vector<const Description*> setupInputs();
            static const std::vector<const Parameter*> setupParameters();
            static const std::vector<const Description*> setupOutputs();
            static const OperatorProperties setupProperties();
            
            static const std::string TYPE;
            static const std::string PACKAGE;
            static const Version VERSION;
            
            static const unsigned int MIN_PORT;
            static const unsigned int MAX_PORT;
            
            UInt16 m_port;
            
            impl::Server* m_server;
        };
    }
}

#endif // STROMX_RUNTIME_SEND_H
