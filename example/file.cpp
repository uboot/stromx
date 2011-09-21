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

#include <iostream>

int main (int argc, char* argv[])
{
    strom::Factory* factory = new strom::Factory;
    
    registerStrom(factory);
    registerBase(factory);
    
    strom::Stream* stream = strom::XmlReader(factory).read("file.xml");
    
    stream->start();
    
    strom::Operator* timer = stream->operators()[1];
    
    for(unsigned int i = 0; i < 5; ++i)
    {
        strom::DataContainer data = timer->getOutputData(0);
        strom::ReadAccess<strom::UInt32> count(data);
        std::cout << "Received " <<  (unsigned int)(count()) << std::endl;
        
        timer->clearOutputData(0);
    }
    
    stream->stop();
    stream->join();
    
    delete stream;
}