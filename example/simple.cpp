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

#include <stream/Stream.h>
#include <stream/Operator.h>
#include <stream/Thread.h>
#include <stream/ReadAccess.h>

#include <base/Counter.h>
#include <base/PeriodicDelay.h>

#include <iostream>


int main (int argc, char* argv[])
{
    using namespace stream;
    
    Stream stream;
    
    Operator* source = new Operator(new base::Counter);
    source->initialize();
    stream.addOperator(source);
    
    Operator* timer = new Operator(new base::PeriodicDelay);
    timer->initialize();
    stream.addOperator(timer);
    
    timer->setParameter(base::PeriodicDelay::PERIOD, UInt32(1000));
    
    stream.connect(source, base::Counter::OUTPUT, timer, base::PeriodicDelay::INPUT);
    
    Thread* thread = stream.addThread();
    thread->addNode(timer, base::PeriodicDelay::INPUT);
    
    stream.start();
    
    for(unsigned int i = 0; i < 5; ++i)
    {
        DataContainer data = timer->getOutputData(base::PeriodicDelay::OUTPUT);
        ReadAccess<UInt32> count(data);
        timer->clearOutputData(base::PeriodicDelay::OUTPUT);
        
        std::cout << "Received " <<  (unsigned int)(count()) << std::endl;
    }
    
    stream.stop();
    stream.join();
}