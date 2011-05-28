#ifndef STREAM_NETWORK_H
#define STREAM_NETWORK_H

#include <string>
#include <vector>

namespace stream
{
    class OperatorNode;
    class OperatorInterface;
    
    class Network
    {
    public:
        enum Status
        {
            INACTIVE,
            ACTIVE
        };
        
        Network(const std::string & name);
        virtual ~Network();
        
        const std::string & name() { return m_name; }
        const Status status() { return m_status; }
        const std::vector<OperatorNode*> operators() { return m_operators; }
        
        OperatorNode* const addOperator(OperatorInterface* const op);
        void removeOperator(OperatorInterface* const op);
        void activate();
        void deactivate();
               
    private:
        std::string m_name; 
        Status m_status;
        std::vector<OperatorNode*> m_operators;
    };
}

#endif // STREAM_NETWORK_H
