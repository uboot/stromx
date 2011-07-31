#ifndef STREAM_OPERATORNODE_H
#define STREAM_OPERATORNODE_H

#include "Operator.h"
#include "SynchronizedOperatorKernel.h"

#include <boost/thread/mutex.hpp>
#include <map>

namespace stream
{
    class InputNode;
    class OutputNode;
    
    class OperatorNode : public Operator
    {
    public:
        OperatorNode(SynchronizedOperatorKernel* const op);
        ~OperatorNode();
        
        // implementation of Operator
        const OperatorInfo* const info() const { return m_kernel->info(); }
        const Status status() { return Status(m_kernel->status()); }
        void setParameter(unsigned int id, const Data& value) { m_kernel->setParameter(id, value); }
        const Data& getParameter(unsigned int id) { return m_kernel->getParameter(id); }
        DataContainer getOutputData(const unsigned int id) { return m_kernel->getOutputData(id); }
        void setInputData(const unsigned int id, DataContainer data) { m_kernel->setInputData(id, data); }
        void clearOutputData(unsigned int id) { m_kernel->clearOutputData(id); }
        
        const std::string & name() const { return m_name; }
        void setName(const std::string & name) { m_name = name; }
        InputNode* const getInputNode(const unsigned int id);
        OutputNode* const getOutputNode(const unsigned int id);
        SynchronizedOperatorKernel* const op() { return m_kernel; }
        SynchronizedOperatorKernel* const kernel() { return m_kernel; }
        
    private:
        std::string m_name;
        SynchronizedOperatorKernel* m_kernel;
        std::map<unsigned int, OutputNode*> m_outputs;
        std::map<unsigned int, InputNode*> m_inputs;
    };
}

#endif // STREAM_OPERATORNODE_H