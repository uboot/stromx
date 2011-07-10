#include "OperatorFactory.h"
#include "Exception.h"

namespace stream
{
    OperatorFactory::~OperatorFactory()
    {

    }

    void OperatorFactory::registerOperator(const stream::Operator*const op)
    {
        if(op == 0)
        {
            throw ArgumentException("Invalid argument: Null pointer");
        }
        
        for(std::vector<Operator*>::iterator iter = m_operators.begin();
            iter != m_operators.end();
            ++iter)
        {
            if(op.name() == (*iter)->name() && op.package() == (*iter)->package())
            {
                throw ArgumentException("Invalid argument: Operator already registered");
            }
        }
        
        m_operators.push_back(op);
    }

    const std::vector< const stream::Operator* >& OperatorFactory::availableOperators() const
    {

    }

    Operator*const OperatorFactory::newOperator(const std::string& package, const std::string& name) const
    {
        // try to find the operator and a copy of it using Operator::clone()
    }
} 
