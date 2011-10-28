#include <stromx/core/Operator.h>

#include <boost/python.hpp>

using namespace boost::python;
using namespace stromx::core;

void exportOperator()
{       
    enum_<Operator::Status>("OperatorStatus")
        .value("NONE", Operator::NONE)
        .value("INITIALIZED", Operator::INITIALIZED)
        .value("ACTIVE", Operator::ACTIVE)
        .value("EXECUTING", Operator::EXECUTING)
        ;
        
    class_<Operator>("Operator", no_init)
        .def("status", &Operator::status)
    ;
}