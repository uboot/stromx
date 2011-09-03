#include <stream/Stream.h>
#include <stream/Operator.h>
#include <stream/Thread.h>

#include <base/ConstImage.h>
#include <base/PeriodicDelay.h>


int main (int argc, char* argv[])
{
    using namespace stream;
    
    Stream* stream = new Stream();
    
    Operator* source = new Operator(new base::ConstImage);
    Operator* timer = new Operator(new base::PeriodicDelay);
    
    source->initialize();
    timer->initialize();
    
    stream->addOperator(source);
    stream->addOperator(timer);
    
    stream->connect(source, base::ConstImage::OUTPUT, timer, base::PeriodicDelay::INPUT);
    
    Thread* thread = stream->addThread();
    thread->addNode(timer, base::PeriodicDelay::INPUT);
    
    stream->start();
    stream->stop();
    stream->join();
}