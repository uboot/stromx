#include <boost/python.hpp>
#include <stromx/runtime/Registry.h>
#include <stromx/cvobjdetect/Cvobjdetect.h>
#include <stromx/cvobjdetect/CascadeClassifier.h>
#include <python/stromx/runtime/ExportOperatorKernel.h>

using namespace boost::python;
using namespace stromx::cvobjdetect;

BOOST_PYTHON_MODULE(libcvobjdetect)
{
    def("register", stromxCvobjdetectRegister);
    
    stromx::python::exportOperatorKernel<CascadeClassifier>("CascadeClassifier");
}
