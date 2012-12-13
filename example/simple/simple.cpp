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

#include <stromx/core/Counter.h>
#include <stromx/core/Operator.h>
#include <stromx/core/PeriodicDelay.h>
#include <stromx/core/ReadAccess.h>
#include <stromx/core/Stream.h>
#include <stromx/core/Thread.h>

#include <iostream>

using namespace stromx;

int main (int, char**)
{
    core::Stream stream;
    
    core::Operator* source = new core::Operator(new core::Counter);
    source->initialize();
    stream.addOperator(source);
    
    core::Operator* timer = new core::Operator(new core::PeriodicDelay);
    timer->initialize();
    stream.addOperator(timer);
    
    timer->setParameter(core::PeriodicDelay::PERIOD, core::UInt32(1000));
    
    stream.connect(source, core::Counter::OUTPUT, timer, core::PeriodicDelay::INPUT);
    
    core::Thread* thread = stream.addThread();
    thread->addInput(timer, core::PeriodicDelay::INPUT);
    
    stream.start();
    
    for(unsigned int i = 0; i < 5; ++i)
    {
        core::DataContainer data = timer->getOutputData(core::PeriodicDelay::OUTPUT);
        core::ReadAccess<core::UInt32> count(data);
        std::cout << "Received " <<  (unsigned int)(count()) << std::endl;
        
        timer->clearOutputData(core::PeriodicDelay::OUTPUT);
    }
    
    stream.stop();
    stream.join();
}