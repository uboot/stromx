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

#ifndef STREAM_IMPL_SYNCHRONIZEDOPERATORKERNEL_H
#define STREAM_IMPL_SYNCHRONIZEDOPERATORKERNEL_H

#include "../DataProvider.h"
#include "../Parameter.h"
#include "../OperatorKernel.h"

#include "Id2DataMap.h"

#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>

namespace stream
{
    class OperatorKernel;
    class Data;
    
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
            
            SynchronizedOperatorKernel(OperatorKernel* const op);
            virtual ~SynchronizedOperatorKernel();
            
            const OperatorInfo* const info() const { return m_op; }
            
            void initialize();
            void activate();
            void deactivate();
            
            // DataProvider implementation
            void receiveInputData(const Id2DataMapper& mapper);
            void sendOutputData(const Id2DataMapper& mapper);
            void testForInterrupt();
            void sleep(const unsigned int microseconds);
            
            const Status status() { return m_status; }
            
            void setParameter(unsigned int id, const Data& value);
            const Data& getParameter(unsigned int id);
            
            DataContainer getOutputData(const unsigned int id);
            void setInputData(const unsigned int id, DataContainer data);
            void clearOutputData(unsigned int id);
            
        private:
            typedef boost::lock_guard<boost::mutex> lock_t;
            typedef boost::unique_lock<boost::mutex> unique_lock_t;
            
            bool tryExecute();
            void waitForSignal(boost::condition_variable& condition, unique_lock_t& lock);
            void validateParameterId(const unsigned int id);
            void validateWriteAccess(const unsigned int id);
            void validateReadAccess(const unsigned int id);
            void validateParameterType(const unsigned int id, const DataVariant& type);
            void validateDataAccess();
            
            OperatorKernel* m_op;
            Status m_status;
            boost::condition_variable m_statusCond;
            boost::condition_variable m_dataCond;
            boost::mutex m_mutex;
            impl::Id2DataMap m_inputMap;
            impl::Id2DataMap m_outputMap;
        };
    }
}

#endif // STREAM_IMPL_SYNCHRONIZEDOPERATORKERNEL_H
