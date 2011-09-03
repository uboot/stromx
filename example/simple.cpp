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
    
    // create an empty stream object
    Stream* stream = new Stream();
    
    // allocate and initialize the source operator
    Operator* source = new Operator(new base::Counter);
    source->initialize();
    
    // allocate and initialize a timer operator
    Operator* timer = new Operator(new base::PeriodicDelay);
    timer->initialize();
    
    // add both operators to the stream
    stream->addOperator(source);
    stream->addOperator(timer);
    
    // the timer will allow one number per second to pass
    timer->setParameter(base::PeriodicDelay::PERIOD, UInt32(1000));
    
    // connect the source to the timer
    stream->connect(source, base::Counter::OUTPUT, timer, base::PeriodicDelay::INPUT);
    
    // add a thread to the stream
    Thread* thread = stream->addThread();
    
    // the thread serves the input of the timer, i.e.
    // it transports data from source to the timer
    thread->addNode(timer, base::PeriodicDelay::INPUT);
    
    // start the stream
    stream->start();
    
    for(unsigned int i = 0; i < 5; ++i)
    {
        // wait for output data
        DataContainer data = timer->getOutputData(base::PeriodicDelay::OUTPUT);
        ReadAccess access(data);
        const UInt32 & count = dynamic_cast<const UInt32 &>(access());
        
        // print the received number
        std::cout << "Received " <<  (unsigned int)(count) << std::endl;
        
        // clear the output 
        timer->clearOutputData(base::PeriodicDelay::OUTPUT);
    }
    
    // tell the stream to stop
    stream->stop();
    
    // wait for it to stop
    stream->join();
    
    // finally delete it
    delete stream;
}