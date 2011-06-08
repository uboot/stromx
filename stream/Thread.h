#ifndef STREAM_THREAD_H
#define STREAM_THREAD_H

#include <vector>
#include <string>

#include <boost/thread/mutex.hpp>
#include <boost/thread.hpp>

namespace boost
{
    class thread;
}

namespace stream
{
    class OperatorNode;
    
    class Thread
    {
    public:
        Thread();
        ~Thread();
        
        const std::vector<OperatorNode*> & operatorSequence() const { return m_operatorSequence; }
        
        void addOperator(OperatorNode* const op);
        void insertOperator(const unsigned int position, OperatorNode* const op);
        void removeOperator(const unsigned int position);
        
        void start();
        void stop();
        void join();
               
    private:
        void loop();
        
        boost::thread* m_thread;
        std::vector<OperatorNode*> m_operatorSequence;
    };
}

#endif // STREAM_NETWORK_H
