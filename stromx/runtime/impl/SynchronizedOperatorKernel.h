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

#ifndef STROMX_RUNTIME_IMPL_SYNCHRONIZEDOPERATORKERNEL_H
#define STROMX_RUNTIME_IMPL_SYNCHRONIZEDOPERATORKERNEL_H

#include <boost/thread/condition_variable.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/mutex.hpp>
#include "stromx/runtime/DataProvider.h"
#include "stromx/runtime/DataRef.h"
#include "stromx/runtime/OperatorKernel.h"
#include "stromx/runtime/Parameter.h"
#include "stromx/runtime/impl/Id2DataMap.h"

namespace stromx
{
    namespace runtime
    {
        class AbstractFactory;
        class Operator;
        class OperatorKernel;
        
        namespace impl
        {
            class SynchronizedOperatorKernel : public DataProvider
            {
            public:    
                enum Status
                {
                    NONE,
                    INITIALIZED,
                    ACTIVE,
                    EXECUTING
                };
                
                explicit SynchronizedOperatorKernel(OperatorKernel* const op);
                virtual ~SynchronizedOperatorKernel();
                
                const OperatorInfo* info() const { return m_op; }
                
                void initialize(const Id2DataMapObserver* const inputObserver, const Id2DataMapObserver* const outputObserver);
                void deinitialize();
                void activate();
                void deactivate();
                void interrupt();
                
                // used by Operator
                Status status() { return m_status; }
                void setParameter(unsigned int id, const Data& value, const bool waitWithTimeout, const unsigned int timeout = 0);
                DataRef getParameter(unsigned int id, const bool waitWithTimeout, const unsigned int timeout = 0);
                DataContainer getOutputData(const unsigned int id);
                void setInputData(const unsigned int id, DataContainer data);
                void clearOutputData(unsigned int id);
                const AbstractFactory* factoryPtr() const { return m_factory; }
                void setFactory(const AbstractFactory* const factory);
                
                // DataProvider implementation
                void receiveInputData(const Id2DataMapper& mapper);
                void sendOutputData(const Id2DataMapper& mapper);
                void testForInterrupt();
                void sleep(const unsigned int microseconds);
                void unlockParameters();
                void lockParameters();
                const AbstractFactory& factory() const;
                
            private:
                typedef boost::lock_guard<boost::mutex> lock_t;
                typedef boost::unique_lock<boost::mutex> unique_lock_t;
                
                // internally used members
                bool tryExecute();
                void waitForSignal(boost::condition_variable& condition, unique_lock_t& lock,
                                   const bool waitWithTimeout, const unsigned int timeout = 0);
                void validateParameterId(const unsigned int id);
                void validateWriteAccess(const unsigned int id);
                void validateReadAccess(const unsigned int id);
                void validateParameterType(const unsigned int id, const VariantInterface& type);
                void validateDataAccess();
                
                OperatorKernel* m_op;
                Status m_status;
                bool m_parametersAreLocked;
                boost::condition_variable m_parameterCond;
                boost::condition_variable m_dataCond;
                boost::mutex m_mutex;
                impl::Id2DataMap m_inputMap;
                impl::Id2DataMap m_outputMap;
                const AbstractFactory* m_factory;
            };
        }
    }
}

#endif // STROMX_RUNTIME_IMPL_SYNCHRONIZEDOPERATORKERNEL_H
