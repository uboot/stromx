#include "Factory.h"

#include "Exception.h"
#include "OperatorKernel.h"
#include "Operator.h"


namespace stream
{
    Factory::~Factory()
    {
        for(std::vector<const OperatorKernel*>::iterator iter = m_operators.begin();
            iter != m_operators.end();
            ++iter)
        {
            delete (*iter);
        }
    }

    void Factory::registerOperator(const OperatorKernel*const op)
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

    const std::vector< const OperatorKernel*>& Factory::availableOperators() const
    {
        return m_operators;
    }

    Operator*const Factory::newOperator(const std::string& package, const std::string& name) const
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

                return new Operator(newOp);
            }
        }
        
       throw WrongArgument("Invalid argument: Operator unknown. Register first unknown operator.");        
    }
    
    void Factory::registerData(const stream::Data*const data)
    {

    }

    Data*const Factory::newData(const std::string& package, const std::string& name) const
    {
        return 0;
    }
} 
