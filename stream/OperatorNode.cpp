#include "OperatorNode.h"

#include "Description.h"
#include "OperatorWrapper.h"
#include "InputNode.h"
#include "OutputNode.h"
#include "Exception.h"

namespace stream
{
    OperatorNode::OperatorNode(OperatorWrapper*const op)
    {
        for(std::vector<Description>::const_iterator iter = op->inputs().begin();
            iter != op->inputs().end();
            ++iter)
        {
            m_inputs.push_back(new InputNode(op, iter->id()));
        }
        
        for(std::vector<Description>::const_iterator iter = op->outputs().begin();
            iter != op->outputs().end();
            ++iter)
        {
            m_outputs.push_back(new OutputNode(op, iter->id()));
        }
    }

    OperatorNode::~OperatorNode()
    {
        for(std::vector<InputNode*>::const_iterator iter = m_inputs.begin();
            iter != m_inputs.end();
            ++iter)
        {
            delete *iter;
        }

        for(std::vector<OutputNode*>::const_iterator iter = m_outputs.begin();
            iter != m_outputs.end();
            ++iter)
        {
            delete *iter;
        }
    }
    
    InputNode*const OperatorNode::getInputNode(const unsigned int id)
    {
        if(id >= m_inputs.size())
            throw ArgumentException("Input with this ID does not exist.");
        
        return m_inputs[id];
    }

    OutputNode*const OperatorNode::getOutputNode(const unsigned int id)
    {
        if(id >= m_outputs.size())
            throw ArgumentException("Output with this ID does not exist.");
        
        return m_outputs[id];
    }

}