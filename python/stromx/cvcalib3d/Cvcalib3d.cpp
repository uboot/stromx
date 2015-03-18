#include <boost/python.hpp>
#include <stromx/runtime/Registry.h>
#include <stromx/cvcalib3d/Cvcalib3d.h>
#include <stromx/cvcalib3d/CalibrateCamera.h>
#include <stromx/cvcalib3d/FindChessboardCorners.h>
#include <stromx/cvcalib3d/GeneratePattern.h>
#include <stromx/cvcalib3d/SolvePnP.h>
#include <python/stromx/runtime/ExportOperatorKernel.h>

using namespace boost::python;
using namespace stromx::cvcalib3d;

BOOST_PYTHON_MODULE(libcvcalib3d)
{
    def("register", stromxCvcalib3dRegister);
    
    stromx::python::exportOperatorKernel<CalibrateCamera>("CalibrateCamera");
    stromx::python::exportOperatorKernel<FindChessboardCorners>("FindChessboardCorners");
    stromx::python::exportOperatorKernel<GeneratePattern>("GeneratePattern");
    stromx::python::exportOperatorKernel<SolvePnP>("SolvePnP");
}
