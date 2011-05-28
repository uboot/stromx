#include "InputNode.h"

#include "OutputNode.h"
#include "OperatorInterface.h"
#include "Exception.h"

namespace stream
{
    InputNode::InputNode(OperatorInterface* const op, const unsigned int inputId)
      : m_source(0),
        m_inputId(inputId),
        m_operator(op)
    {

    }

    void InputNode::connect(OutputNode* const output)
    {
        lock_t lock(m_mutex);
        
        if(m_source)
            throw InvalidStateException("Input node has already been connected.");
        
        m_source = output;
        m_source->incrementConnectedInputs();
    }
    
    void InputNode::setInputData()
    {
        DataContainer* inputData = m_source->getOutputData();
        
        m_operator->setInputData(m_inputId, inputData);
    }
    
    void InputNode::disconnect()
    {
        lock_t lock(m_mutex);
        
        if(m_source)
            m_source->decrementConnectedInputs();
        
        m_source = 0;
    }
}