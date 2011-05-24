#ifndef STREAM_INPUTNODE_H
#define STREAM_INPUTNODE_H

#include <boost/thread/mutex.hpp>

namespace stream
{
    class OutputNode;
    class DataContainer;
    class OperatorWrapper;
    
    class InputNode
    {
    public:
        InputNode(OperatorWrapper* const op, const unsigned int inputId);
        
        const OutputNode& source() const { return *m_source; }
        void connect(OutputNode* const output);
        void setInputData();
        
    private:
        typedef boost::lock_guard<boost::mutex> lock_t;
        
        OutputNode *m_source;
        unsigned int m_inputId;
        OperatorWrapper* m_operator;
        
        boost::mutex m_mutex;
    };
}

#endif // STREAM_INPUTNODE_H
