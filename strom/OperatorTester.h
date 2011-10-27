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

#ifndef STROM_OPERATORTESTER_H
#define STROM_OPERATORTESTER_H

#include "Operator.h"

namespace strom
{
    /** \brief %Operator with utility functions for unit testing */
    class OperatorTester : public Operator
    {
    public:
        OperatorTester(OperatorKernel* const kernel) 
          : Operator(kernel)
        {}
        
        void activate() { Operator::activate(); }
        void deactivate()  { Operator::deactivate(); }
    };
}

#endif // STROM_OPERATORTESTER_H
