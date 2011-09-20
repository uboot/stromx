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

#include <strom/Factory.h>
#include <strom/XmlReader.h>
#include <strom/Stream.h>
#include <strom/Strom.h>
#include <strom/Operator.h>
#include <strom/Primitive.h>
#include <strom/ReadAccess.h>

#include <base/Base.h>

#include "math/Add.h"

#include <iostream>

int main (int argc, char* argv[])
{
    using namespace strom;
   
    Factory* factory = new Factory;
    
    registerStrom(factory);
    registerBase(factory);
    
    OperatorKernel* op = new math::Add;
    factory->registerOperator(op);
    
    Stream* stream = XmlReader(factory).read("operator.xml");
    
    stream->start();
    
    Operator* timer = stream->operators()[2];
    
    for(unsigned int i = 0; i < 5; ++i)
    {
        DataContainer data = timer->getOutputData(0);
        ReadAccess<UInt32> count(data);
        timer->clearOutputData(0);
        
        std::cout << "Received " <<  (unsigned int)(count()) << std::endl;
    }
    
    stream->stop();
    stream->join();
    
    delete stream;
}