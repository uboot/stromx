#ifndef STREAM_NETWORK_H
#define STREAM_NETWORK_H

#include <string>

namespace stream
{
    class Network
    {
    public:
        Network(const std::string & name);
        
        const std::string & name() { return m_name; }
               
    private:
        std::string m_name; 
    };
}

#endif // STREAM_NETWORK_H
