#ifndef STREAM_THREAD_H
#define STREAM_THREAD_H

#include <vector>
#include <string>

#include <boost/thread/mutex.hpp>
#include <boost/thread.hpp>

#include "ThreadInterface.h"

namespace stream
{
    class Thread : public ThreadInterface
    {
    public:
        
        Thread();
        ~Thread();
        
        const Status status() const { return m_status; }
        
        const std::string & name() const { return m_name; }
        void setName(const std::string& name) { m_name = name; }
        const std::vector<InputNode*> & nodeSequence() const { return m_nodeSequence; }
        
        void addNode(InputNode* const op);
        void insertNode(const unsigned int position, InputNode* const op);
        void removeNode(const unsigned int position);
        
        void start();
        void stop();
        void join();
               
    private:
        void loop();
        
        Status m_status;
        boost::thread* m_thread;
        std::vector<InputNode*> m_nodeSequence;
        std::string m_name;
    };
}

#endif // STREAM_THREAD_H
