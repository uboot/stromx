#ifndef STREAM_THREAD_H
#define STREAM_THREAD_H

#include <vector>

namespace stream
{
    class OperatorNode;
    
    class Thread
    {
    public:
        Thread(const std::vector<OperatorNode*> & operatorSequence);
        ~Thread();
        
        void start();
        void stop();
        void join();
               
    private:
        std::vector<OperatorNode*> m_operatorSequence;
    };
}

#endif // STREAM_NETWORK_H
