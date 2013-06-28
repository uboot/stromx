#include <boost/python.hpp>
#include <stromx/runtime/Registry.h>
#include <stromx/cvcore/Cvcore.h>
#include <stromx/cvcore/Add.h>
#include <python/stromx/runtime/ExportOperatorKernel.h>

using namespace boost::python;
using namespace stromx::cvcore;

BOOST_PYTHON_MODULE(libcvcore)
{
    def("registerCvcore", stromxRegisterCvcore);
    
    stromx::python::exportOperatorKernel<Add>("Add");
}
