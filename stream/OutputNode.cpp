#include "OutputNode.h"

#include "Exception.h"
#include "OperatorInterface.h"

namespace stream
{
    OutputNode::OutputNode(OperatorInterface*const op, const unsigned int outputId)
      : m_operator(op),
        m_outputId(outputId),
        m_connectedInputs(0),
        m_remainingCopies(0)
    {}
    
    void OutputNode::incrementConnectedInputs()
    {
        lock_t lock(m_mutex);
        
        m_connectedInputs++;
    }

    void OutputNode::decrementConnectedInputs()
    {
        lock_t lock(m_mutex);
        
        if(! m_connectedInputs)
            throw InvalidStateException("Number of connected inputs is 0.");
        
        m_connectedInputs--;   
    }

    DataContainer*const OutputNode::getOutputData()
    {
        DataContainer* value = m_operator->getOutputData(m_outputId);
        
        // the data has been obtained
        // now make sure the connection counter is adapted in an atomic operation
        {
            lock_t lock(m_mutex);
            
            if(! m_connectedInputs)
                throw InvalidStateException("No inputs are connected to this output node.");
            
            if(! m_remainingCopies)
            {
                m_operator->clearOutputData(m_outputId);
                m_remainingCopies = m_connectedInputs;
            }
            
            m_remainingCopies--; 
        }
        
        return value;
    }

}
