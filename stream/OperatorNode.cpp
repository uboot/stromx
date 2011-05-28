#include "OperatorNode.h"

#include "Description.h"
#include "OperatorInterface.h"
#include "InputNode.h"
#include "OutputNode.h"
#include "Exception.h"

namespace stream
{
    OperatorNode::OperatorNode(OperatorInterface*const op)
        : m_operator(op)
    {
        for(std::vector<Description>::const_iterator iter = op->inputs().begin();
            iter != op->inputs().end();
            ++iter)
        {
            if(m_inputs.count(iter->id()))
                throw ArgumentException("Two inputs with the same ID.");
            
            m_inputs[iter->id()] = new InputNode(op, iter->id());
        }
        
        for(std::vector<Description>::const_iterator iter = op->outputs().begin();
            iter != op->outputs().end();
            ++iter)
        {
            if(m_outputs.count(iter->id()))
                throw ArgumentException("Two outputs with the same ID.");
            
            m_outputs[iter->id()] = new OutputNode(op, iter->id());
        }
    }

    OperatorNode::~OperatorNode()
    {
        for(std::map<unsigned int, InputNode*>::iterator iter = m_inputs.begin();
            iter != m_inputs.end();
            ++iter)
        {
            delete iter->second;
        } 
        
        for(std::map<unsigned int, OutputNode*>::iterator iter = m_outputs.begin();
            iter != m_outputs.end();
            ++iter)
        {
            delete iter->second;
        }
        
        delete m_operator;
    }
    
    InputNode*const OperatorNode::getInputNode(const unsigned int id)
    {
        if(! m_inputs.count(id))
            throw ArgumentException("Input with this ID does not exist.");
        
        return m_inputs[id];
    }

    OutputNode*const OperatorNode::getOutputNode(const unsigned int id)
    {
        if(! m_outputs.count(id))
            throw ArgumentException("Output with this ID does not exist.");
        
        return m_outputs[id];
    }

}