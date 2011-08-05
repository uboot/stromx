#include "Operator.h"

#include "Description.h"
#include "OperatorInfo.h"
#include "Exception.h"

#include "impl/InputNode.h"
#include "impl/OutputNode.h"
#include "impl/SynchronizedOperatorKernel.h"

namespace stream
{
    Operator::Operator(OperatorKernel*const kernel)
        : m_kernel(new SynchronizedOperatorKernel(kernel))
    {}

    Operator::~Operator()
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
        
        delete m_kernel;
    }
    
    void Operator::initialize()
    {
        m_kernel->initialize();
        
        for(std::vector<const Description*>::const_iterator iter = m_kernel->info()->inputs().begin();
            iter != m_kernel->info()->inputs().end();
            ++iter)
        {
            if(m_inputs.count((*iter)->id()))
                throw InvalidArgument("Two inputs with the same ID.");
            
            m_inputs[(*iter)->id()] = new InputNode(m_kernel, (*iter)->id());
        }
        
        for(std::vector<const Description*>::const_iterator iter = m_kernel->info()->outputs().begin();
            iter != m_kernel->info()->outputs().end();
            ++iter)
        {
            if(m_outputs.count((*iter)->id()))
                throw InvalidArgument("Two outputs with the same ID.");
            
            m_outputs[(*iter)->id()] = new OutputNode(m_kernel, (*iter)->id());
        }
    }
    
    InputNode*const Operator::getInputNode(const unsigned int id)
    {
        if(! m_inputs.count(id))
            throw InvalidArgument("Input with this ID does not exist.");
        
        return m_inputs[id];
    }

    OutputNode*const Operator::getOutputNode(const unsigned int id)
    {
        if(! m_outputs.count(id))
            throw InvalidArgument("Output with this ID does not exist.");
        
        return m_outputs[id];
    }

}