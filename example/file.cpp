#include <stream/Factory.h>
#include <stream/XmlReader.h>
#include <stream/Stream.h>
#include <stream/Operator.h>
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
    
    // tell the stream to stop
    stream->stop();
    
    // wait for it to stop
    stream->join();
    
    // finally delete it
    delete stream;
}