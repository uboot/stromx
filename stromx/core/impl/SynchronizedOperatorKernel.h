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

#ifndef STROMX_CORE_IMPL_SYNCHRONIZEDOPERATORKERNEL_H
#define STROMX_CORE_IMPL_SYNCHRONIZEDOPERATORKERNEL_H

#include <boost/thread/condition_variable.hpp>
#include <boost/thread/mutex.hpp>
#include "Id2DataMap.h"
#include "../DataProvider.h"
#include "../OperatorKernel.h"
#include "../Parameter.h"

namespace stromx
{
    namespace core
    {
        class Operator;
        class OperatorKernel;
        class Data;
        
        namespace impl
        {
            class SynchronizedOperatorKernel : public DataProvider
            {
                friend class stromx::core::Operator;
                
            public:    
                enum Status
                {
                    NONE,
                    INITIALIZED,
                    ACTIVE,
                    EXECUTING
                };
                
                SynchronizedOperatorKernel(OperatorKernel* const op);
                virtual ~SynchronizedOperatorKernel();
                
                const OperatorInfo* const info() const { return m_op; }
                
                void initialize(const Id2DataMapObserver* const inputObserver, const Id2DataMapObserver* const outputObserver);
                void deinitialize();
                void activate();
                void deactivate();
                
                // DataProvider implementation
                void receiveInputData(const Id2DataMapper& mapper);
                void sendOutputData(const Id2DataMapper& mapper);
                void testForInterrupt();
                void sleep(const unsigned int microseconds);
                void unlockParameters();
                void lockParameters();
                
            private:
                typedef boost::lock_guard<boost::mutex> lock_t;
                typedef boost::unique_lock<boost::mutex> unique_lock_t;
                
                // used by Operator
                const Status status() { return m_status; }
                void setParameter(unsigned int id, const Data& value, const bool waitWithTimeout, const unsigned int timeout = 0);
                const Data& getParameter(unsigned int id, const bool waitWithTimeout, const unsigned int timeout = 0);
                DataContainer getOutputData(const unsigned int id, const bool waitWithTimeout, const unsigned int timeout = 0);
                void setInputData(const unsigned int id, DataContainer data, const bool waitWithTimeout, const unsigned int timeout = 0);
                void clearOutputData(unsigned int id);
                
                // internally used members
                bool tryExecute();
                void waitForSignal(boost::condition_variable& condition, unique_lock_t& lock,
                                   const bool waitWithTimeout, const unsigned int timeout = 0);
                void validateParameterId(const unsigned int id);
                void validateWriteAccess(const unsigned int id);
                void validateReadAccess(const unsigned int id);
                void validateParameterType(const unsigned int id, const DataVariant& type);
                void validateDataAccess();
                
                OperatorKernel* m_op;
                Status m_status;
                bool m_parametersAreLocked;
                boost::condition_variable m_parameterCond;
                boost::condition_variable m_dataCond;
                boost::mutex m_mutex;
                impl::Id2DataMap m_inputMap;
                impl::Id2DataMap m_outputMap;
            };
        }
    }
}

#endif // STROMX_CORE_IMPL_SYNCHRONIZEDOPERATORKERNEL_H
