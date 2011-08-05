#include "XmlReader.h"

#include "Exception.h"

#include <xercesc/dom/DOM.hpp>

namespace stream
{
    using namespace xercesc;
    
    Stream*const XmlReader::read(const std::string & filename, const OperatorFactory* const factory)
    {        
        try
        {
            XMLPlatformUtils::Initialize();  // Initialize Xerces infrastructure
        }
        catch(XMLException&)
        {
            throw InternalError("Failed to initialize Xerces-C.");
        }
        
        try
        {
            XMLPlatformUtils::Terminate();  // Terminate after release of memory
        }
        catch(XMLException&)
        {
            throw InternalError("Failed to terminate Xerces-C.");
        }
    }
}
