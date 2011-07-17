#include "OperatorFactory.h"
#include "Exception.h"
#include "Operator.h"


namespace stream
{
    OperatorFactory::~OperatorFactory()
    {
        for(std::vector<const Operator*>::iterator iter = m_operators.begin();
            iter != m_operators.end();
            ++iter)
        {
            delete (*iter);
        }
    }

    void OperatorFactory::registerOperator(const Operator*const op)
    {
        if(op == 0)
        {
            throw ArgumentException("Invalid argument: Null pointer.");
        }
        
        for(std::vector<const Operator*>::iterator iter = m_operators.begin();
            iter != m_operators.end();
            ++iter)
        {
            if(op->name() == (*iter)->name() && op->package() == (*iter)->package())
            {
                throw ArgumentException("Invalid argument: Operator already registered.");
            }
        }
        
        m_operators.push_back(op);
    }

    const std::vector< const Operator*>& OperatorFactory::availableOperators() const
    {
        return m_operators;
    }

    Operator*const OperatorFactory::newOperator(const std::string& package, const std::string& name) const
    {
        for(std::vector<const Operator*>::const_iterator iter = m_operators.begin();
            iter != m_operators.end();
            ++iter)
        {
            if((*iter)->name() == name && (*iter)->package() == package)
            {
                Operator* newOp = 0;
                newOp = (*iter)->clone();
                if (newOp == 0)
                { 
                    throw ArgumentException("Invalid argument received: Null pointer. Cloning failed");
                }

                return newOp;
            }
        }
        
       throw ArgumentException("Invalid argument: Operator unknown. Register first unknown operator.");        
    }
} 
