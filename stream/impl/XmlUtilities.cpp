#include "XmlUtilities.h"

    
namespace stream
{
    namespace impl
    {
        using namespace xercesc;
        
        Xml2Str::Xml2Str(const XMLCh* const str)
        {
            m_str = XMLString::transcode(str);
        }

        Xml2Str::~Xml2Str()
        {
            XMLString::release(&m_str);
        }

        Xml2Str::operator const char*()
        {
            return m_str;
        }
        
        Str2Xml::Str2Xml(const char* const str)
        {
            m_xmlStr = XMLString::transcode(str);
        }

        Str2Xml::~Str2Xml()
        {
            XMLString::release(&m_xmlStr);
        }

        Str2Xml::operator const XMLCh*()
        {
            return m_xmlStr;
        }
    }
}