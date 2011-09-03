#include <stream/Stream.h>
#include <stream/Operator.h>
#include <stream/Thread.h>

#include <base/Image.h>
#include <base/ConstImage.h>
#include <base/PeriodicDelay.h>


int main (int argc, char* argv[])
{
    using namespace stream;
    
    // set up a image filled with constant gray 128
    base::Image image(100, 200, Image::MONO_8);
    memset(image.data(), 128, image.size());
    
    // create an empty stream object
    Stream* stream = new Stream();
    
    // allocate and initialize the source operator
    Operator* source = new Operator(new base::ConstImage);
    source->initialize();
    
    // allocate and initialize a timer operator
    Operator* timer = new Operator(new base::PeriodicDelay);
    timer->initialize();
    
    // add both operators to the stream
    stream->addOperator(source);
    stream->addOperator(timer);
    
    // set the source image to the previously filled gray area
    source->setParameter(base::ConstImage::IMAGE, image);
    
    // the timer will allow one image every second to pass
    timer->setParameter(base::PeriodicDelay::PERIOD, UInt32(1000));
    
    // connect the source to the timer
    stream->connect(source, base::ConstImage::OUTPUT, timer, base::PeriodicDelay::INPUT);
    
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
        
        // clear the output to make the image buffer available for recycling 
        timer->clearOutputData(base::PeriodicDelay::OUTPUT);
        
        std::cout << "Received image " << i << std::endl;
    }
    
    // tell the stream to stop
    stream->stop();
    
    // wait for it to stop
    stream->join();
    
    // finally delete it
    delete stream;
}