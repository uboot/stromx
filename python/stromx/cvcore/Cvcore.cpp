#include <boost/python.hpp>
#include <stromx/runtime/Registry.h>
#include <stromx/cvcore/Cvcore.h>
#include <stromx/cvcore/Absdiff.h>
#include <stromx/cvcore/Add.h>
#include <stromx/cvcore/AddWeighted.h>
#include <stromx/cvcore/Bitwise_and.h>
#include <stromx/cvcore/Bitwise_not.h>
#include <stromx/cvcore/Bitwise_or.h>
#include <stromx/cvcore/Bitwise_xor.h>
#include <python/stromx/runtime/ExportOperatorKernel.h>

using namespace boost::python;
using namespace stromx::cvcore;

BOOST_PYTHON_MODULE(libcvcore)
{
    def("register", stromxCvcoreRegister);
    
    stromx::python::exportOperatorKernel<Absdiff>("Absdiff");
    stromx::python::exportOperatorKernel<Add>("Add");
    stromx::python::exportOperatorKernel<AddWeighted>("AddWeighted");
    stromx::python::exportOperatorKernel<Bitwise_and>("Bitwise_and");
    stromx::python::exportOperatorKernel<Bitwise_not>("Bitwise_not");
    stromx::python::exportOperatorKernel<Bitwise_or>("Bitwise_or");
    stromx::python::exportOperatorKernel<Bitwise_xor>("Bitwise_xor");
}
