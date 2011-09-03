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