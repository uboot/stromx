#include "OperatorFactory.h"

namespace stream
{
    OperatorFactory::~OperatorFactory()
    {

    }

    void OperatorFactory::registerOperator(const stream::Operator*const op)
    {

    }

    const std::vector< const stream::Operator* >& OperatorFactory::availableOperators() const
    {

    }

    Operator*const OperatorFactory::newOperator(const std::string& package, const std::string& name) const
    {
        // try to find the operator and a copy of it using Operator::clone()
    }
} 
