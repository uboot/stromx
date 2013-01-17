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

#include <stromx/runtime/Counter.h>
#include <stromx/runtime/Operator.h>
#include <stromx/runtime/PeriodicDelay.h>
#include <stromx/runtime/ReadAccess.h>
#include <stromx/runtime/Stream.h>
#include <stromx/runtime/Thread.h>

#include <iostream>

using namespace stromx;

int main (int, char**)
{
    runtime::Stream stream;
    
    runtime::Operator* source = new runtime::Operator(new runtime::Counter);
    source->initialize();
    stream.addOperator(source);
    
    runtime::Operator* timer = new runtime::Operator(new runtime::PeriodicDelay);
    timer->initialize();
    stream.addOperator(timer);
    
    timer->setParameter(runtime::PeriodicDelay::PERIOD, runtime::UInt32(1000));
    
    stream.connect(source, runtime::Counter::OUTPUT, timer, runtime::PeriodicDelay::INPUT);
    
    runtime::Thread* thread = stream.addThread();
    thread->addInput(timer, runtime::PeriodicDelay::INPUT);
    
    stream.start();
    
    for(unsigned int i = 0; i < 5; ++i)
    {
        runtime::DataContainer data = timer->getOutputData(runtime::PeriodicDelay::OUTPUT);
        runtime::ReadAccess<runtime::UInt32> count(data);
        std::cout << "Received " <<  (unsigned int)(count()) << std::endl;
        
        timer->clearOutputData(runtime::PeriodicDelay::OUTPUT);
    }
    
    stream.stop();
    stream.join();
}