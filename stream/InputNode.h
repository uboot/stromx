#ifndef STREAM_INPUTNODE_H
#define STREAM_INPUTNODE_H

#include <boost/thread/mutex.hpp>

namespace stream
{
    class OutputNode;
    class DataContainer;
    class SynchronizedOperatorKernel;
    
    class InputNode
    {
    public:
        InputNode(SynchronizedOperatorKernel* const op, const unsigned int inputId);
        
        const unsigned int inputId() const { return m_inputId; }
        const SynchronizedOperatorKernel* const op() { return m_operator; }
        const bool isConnected() const { return m_source == 0; }
        const OutputNode& source() const { return *m_source; }
        void connect(OutputNode* const output);
        void disconnect();
        void setInputData();
        
    private:
        typedef boost::lock_guard<boost::mutex> lock_t;
        
        OutputNode *m_source;
        unsigned int m_inputId;
        SynchronizedOperatorKernel* m_operator;
        
        boost::mutex m_mutex;
    };
}

#endif // STREAM_INPUTNODE_H
