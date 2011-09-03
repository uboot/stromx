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
    
    // start the stream
    stream->start();
    
    Operator* timer = stream->operators()[1];
    
    for(unsigned int i = 0; i < 5; ++i)
    {
        // wait for output data
        DataContainer data = timer->getOutputData(0);
        ReadAccess access(data);
        const UInt32 & count = dynamic_cast<const UInt32 &>(access());
        
        // print the received number
        std::cout << "Received " <<  (unsigned int)(count) << std::endl;
        
        // clear the output 
        timer->clearOutputData(0);
    }
    
    // tell the stream to stop
    stream->stop();
    
    // wait for it to stop
    stream->join();
    
    // finally delete it
    delete stream;
}