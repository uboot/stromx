#include <boost/python.hpp>
#include <stromx/runtime/Registry.h>
#include <stromx/core/Core.h>
#include <stromx/core/Add.h>
#include <python/stromx/runtime/ExportOperatorKernel.h>

using namespace boost::python;
using namespace stromx::core;

BOOST_PYTHON_MODULE(libcore)
{
    def("registerCore", stromxRegisterCore);
    
    stromx::python::exportOperatorKernel<Add>("Add");
}
