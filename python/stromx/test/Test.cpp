#include <boost/python.hpp>
#include <stromx/runtime/Registry.h>
#include <stromx/test/Test.h>
#include <stromx/test/DeadlockOperator.h>
#include <stromx/test/ExceptionOperator.h>
#include <stromx/test/ParameterOperator.h>
#include <stromx/test/RandomDataOperator.h>
#include <stromx/test/TestDataOperator.h>
#include <python/stromx/runtime/ExportOperatorKernel.h>

using namespace boost::python;

BOOST_PYTHON_MODULE(libtest)
{
    def("register", stromxTestRegister);
    
    stromx::python::exportOperatorKernel<DeadlockOperator>("DeadlockOperator");
    stromx::python::exportOperatorKernel<ExceptionOperator>("ExceptionOperator");
    stromx::python::exportOperatorKernel<ParameterOperator>("ParameterOperator");
    stromx::python::exportOperatorKernel<RandomDataOperator>("RandomDataOperator");
    stromx::python::exportOperatorKernel<TestDataOperator>("TestDataOperator");
}
