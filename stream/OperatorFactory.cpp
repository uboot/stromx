#include "OperatorFactory.h"

#include "Exception.h"
#include "OperatorKernel.h"
#include "Operator.h"


namespace stream
{
    OperatorFactory::~OperatorFactory()
    {
        for(std::vector<const OperatorKernel*>::iterator iter = m_operators.begin();
            iter != m_operators.end();
            ++iter)
        {
            delete (*iter);
        }
    }

    void OperatorFactory::registerOperator(const OperatorKernel*const op)
    {
        if(op == 0)
        {
            throw WrongArgument("Invalid argument: Null pointer.");
        }
        
        for(std::vector<const OperatorKernel*>::iterator iter = m_operators.begin();
            iter != m_operators.end();
            ++iter)
        {
            if(op->name() == (*iter)->name() && op->package() == (*iter)->package())
            {
                throw WrongArgument("Invalid argument: Operator already registered.");
            }
        }
        
        m_operators.push_back(op);
    }

    const std::vector< const OperatorKernel*>& OperatorFactory::availableOperators() const
    {
        return m_operators;
    }

    OperatorKernel*const OperatorFactory::newOperator(const std::string& package, const std::string& name) const
    {
        for(std::vector<const OperatorKernel*>::const_iterator iter = m_operators.begin();
            iter != m_operators.end();
            ++iter)
        {
            if((*iter)->name() == name && (*iter)->package() == package)
            {
                OperatorKernel* newOp = 0;
                newOp = (*iter)->clone();
                if (newOp == 0)
                { 
                    throw InternalError("Invalid argument received: Null pointer. Cloning failed");
                }

                return newOp;
            }
        }
        
       throw WrongArgument("Invalid argument: Operator unknown. Register first unknown operator.");        
    }
} 
