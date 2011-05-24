#ifndef STREAM_OPERATORNODE_H
#define STREAM_OPERATORNODE_H

#include <boost/thread/mutex.hpp>
#include <vector>

namespace stream
{
    class OperatorWrapper;
    class InputNode;
    class OutputNode;
    
    class OperatorNode
    {
    public:
        OperatorNode(OperatorWrapper* const op);
        ~OperatorNode();
        
        InputNode* const getInputNode(const unsigned int id);
        OutputNode* const getOutputNode(const unsigned int id);
        
    private:
        std::vector<OutputNode*> m_outputs;
        std::vector<InputNode*> m_inputs;
    };
}

#endif // STREAM_OPERATORNODE_H