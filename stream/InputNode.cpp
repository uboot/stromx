#include "InputNode.h"

#include "OutputNode.h"
#include "OperatorWrapper.h"

namespace stream
{
    InputNode::InputNode(OperatorWrapper* const op, const unsigned int inputId)
      : m_source(0),
        m_inputId(inputId),
        m_operator(op)
    {

    }

    void InputNode::connect(OutputNode* const output)
    {
        lock_t lock(m_mutex);
        
        if(m_source)
            m_source->decrementConnectedInputs();
        
        m_source = output;
        m_source->incrementConnectedInputs();
    }
    
    void InputNode::setInputData()
    {
        lock_t lock(m_mutex);
            
        DataContainer* inputData = m_source->getOutputData();
        
        m_operator->setInputData(m_inputId, inputData);
    }
}