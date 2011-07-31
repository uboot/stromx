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

#ifndef STREAM_OPERATOR_H
#define STREAM_OPERATOR_H

#include "DataContainer.h"

namespace stream
{
    class OperatorInfo;
    class Id2DataMapper;
    class Data;
    
    class Operator
    {
    public:
        enum Status
        {
            NONE,
            INITIALIZED,
            ACTIVE,
            EXECUTING
        };
        
        virtual ~Operator() {}
        
        virtual const OperatorInfo* const info() const = 0;
        virtual const Status status() = 0;
        
        virtual void setParameter(unsigned int id, const Data& value) = 0;
        virtual const Data& getParameter(unsigned int id) = 0;
        
        virtual DataContainer getOutputData(const unsigned int id) = 0;
        virtual void setInputData(const unsigned int id, DataContainer data) = 0;
        virtual void clearOutputData(unsigned int id) = 0;
    };
}

#endif // STREAM_OPERATOR_H
