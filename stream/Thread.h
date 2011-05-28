#ifndef STREAM_THREAD_H
#define STREAM_THREAD_H

#include <vector>
#include <string>

namespace stream
{
    class OperatorNode;
    
    class Thread
    {
    public:
        Thread(const std::string & name);
        ~Thread();
        
        const std::string name() const { return m_name; }
        const std::vector<OperatorNode*> & operatorSequence() const { return m_operatorSequence; }
        
        void addOperator(OperatorNode* const op);
        void insertOperator(const unsigned int position, OperatorNode* const op);
        void removeOperator(const unsigned int position);
        
        void start();
        void stop();
        void join();
               
    private:
        std::string m_name;
        std::vector<OperatorNode*> m_operatorSequence;
    };
}

#endif // STREAM_NETWORK_H
