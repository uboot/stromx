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

#include <stream/Factory.h>
#include <stream/XmlReader.h>
#include <stream/Stream.h>
#include <stream/Operator.h>
#include <stream/Primitive.h>
#include <stream/ReadAccess.h>

#include <base/Base.h>

#include <iostream>

int main (int argc, char* argv[])
{
    using namespace stream;
   
    Factory* factory = new Factory;
    
    registerStream(factory);
    registerBase(factory);
    
    Stream* stream = XmlReader(factory).read("file.xml");
    
    stream->start();
    
    Operator* timer = stream->operators()[1];
    
    for(unsigned int i = 0; i < 5; ++i)
    {
        DataContainer data = timer->getOutputData(0);
        ReadAccess access(data);
        const UInt32 & count = dynamic_cast<const UInt32 &>(access());
        timer->clearOutputData(0);
        
        std::cout << "Received " <<  (unsigned int)(count) << std::endl;
    }
    
    stream->stop();
    stream->join();
    
    delete stream;
}