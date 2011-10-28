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

#ifndef STROM_TESTOPERATOR_H
#define STROM_TESTOPERATOR_H

#include <stromx/core/OperatorKernel.h>
#include <stromx/core/Primitive.h>

namespace core
{
    class TestOperator : public OperatorKernel
    {
    public:
        static const unsigned int INPUT_1 = 0;
        static const unsigned int INPUT_2 = 1;
        static const unsigned int OUTPUT_1 = 2;
        static const unsigned int OUTPUT_2 = 3;
        static const unsigned int BUFFER_SIZE = 0;
        static const unsigned int SLEEP_TIME = 1;
        
        TestOperator();
        
        virtual OperatorKernel* const clone() const { return new TestOperator; }
        virtual void setParameter(unsigned int id, const Data& value);
        const Data& getParameter(const unsigned int id) const;
        virtual void execute(DataProvider& provider);
        virtual void initialize();
        
        const unsigned int numExecutes() { return m_numExecutes; }
        
    private:
        static const std::vector<const Description*> setupInputs();
        static const std::vector<const Description*> setupOutputs();
        static const std::vector<const Parameter*> setupInitParameters();
        static const std::vector<const Parameter*> setupParameters();
        
        static const std::string TYPE;
        static const std::string PACKAGE;
        static const Version VERSION;
        
        UInt32 m_sleepTime;
        UInt32 m_bufferSize;
        unsigned int m_numExecutes;
    };
}


#endif // STROM_TESTOPERATOR_H
