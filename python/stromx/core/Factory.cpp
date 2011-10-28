#include <stromx/core/Operator.h>
#include <stromx/core/Factory.h>

#include <boost/python.hpp>

using namespace boost::python;
using namespace stromx::core;

void exportFactory()
{          
    typedef Operator* (Factory::*newOperatorType)(const std::string & package, const std::string & name) const; 
    
    class_<Factory>("Factory")
        .def("newOperator", reinterpret_cast<newOperatorType>(&Factory::newOperator), return_value_policy<manage_new_object>())
    ;
}