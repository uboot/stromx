#include <boost/python.hpp>
#include <stromx/runtime/Registry.h>
#include <stromx/cvcore/Cvcore.h>
#include <stromx/cvcore/Absdiff.h>
#include <stromx/cvcore/Add.h>
#include <stromx/cvcore/AddWeighted.h>
#include <python/stromx/runtime/ExportOperatorKernel.h>

using namespace boost::python;
using namespace stromx::cvcore;

BOOST_PYTHON_MODULE(libcvcore)
{
    def("register", stromxCvcoreRegister);
    
    stromx::python::exportOperatorKernel<Absdiff>("Absdiff");
    stromx::python::exportOperatorKernel<Add>("Add");
    stromx::python::exportOperatorKernel<AddWeighted>("AddWeighted");
}
