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
    class InputNode;
    
    class Thread
    {
    public:
        Thread();
        ~Thread();
        
        const std::vector<InputNode*> & nodeSequence() const { return m_nodeSequence; }
        
        void addNode(InputNode* const op);
        void insertNode(const unsigned int position, InputNode* const op);
        void removeNode(const unsigned int position);
        
        void start();
        void stop();
        void join();
               
    private:
        void loop();
        
        boost::thread* m_thread;
        std::vector<InputNode*> m_nodeSequence;
    };
}

#endif // STREAM_NETWORK_H
