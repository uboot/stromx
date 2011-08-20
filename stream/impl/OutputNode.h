#ifndef STREAM_OUTPUTNODE_H
#define STREAM_OUTPUTNODE_H

#include <boost/thread/mutex.hpp>
#include <set>

namespace stream
{
    class Operator;
    class DataContainer;
    class InputNode;
    
    class OutputNode
    {
    public:
        OutputNode(Operator* const op, const unsigned int outputId);
        
        const unsigned int outputId() const { return m_outputId; }
        Operator* const op() const { return m_operator; }
        DataContainer getOutputData();
        void addConnectedInput(InputNode* const input);
        void removeConnectedInput(InputNode* const input);
        const std::set<InputNode*> connectedInputs() const { return m_connectedInputs; }
        
    private:
        typedef boost::lock_guard<boost::mutex> lock_t;
        
        Operator* m_operator;
        unsigned int m_outputId;
        std::set<InputNode*> m_connectedInputs;
        unsigned int m_remainingCopies;
        
        boost::mutex m_mutex;  
    };
}

#endif // STREAM_OUTPUTNODE_H