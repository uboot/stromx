#include <boost/python.hpp>
#include <stromx/runtime/Registry.h>
#include <stromx/cvimgproc/Cvimgproc.h>
#include <stromx/cvimgproc/BilateralFilter.h>
#include <stromx/cvimgproc/Blur.h>
#include <stromx/cvimgproc/BoxFilter.h>
#include <stromx/cvimgproc/Dilate.h>
#include <stromx/cvimgproc/Erode.h>
#include <stromx/cvimgproc/GaussianBlur.h>
#include <stromx/cvimgproc/MedianBlur.h>
#include <stromx/cvimgproc/MorphologyEx.h>
#include <stromx/cvimgproc/Laplacian.h>
#include <stromx/cvimgproc/PyrDown.h>
#include <stromx/cvimgproc/PyrUp.h>
#include <stromx/cvimgproc/Scharr.h>
#include <stromx/cvimgproc/Sobel.h>
#include <stromx/cvimgproc/Resize.h>
#include <stromx/cvimgproc/AdaptiveThreshold.h>
#include <stromx/cvimgproc/Threshold.h>
#include <stromx/cvimgproc/WarpAffine.h>
#include <stromx/cvimgproc/WarpPerspective.h>
#include <stromx/cvimgproc/Undistort.h>
#include <stromx/cvimgproc/UndistortPoints.h>
#include <python/stromx/runtime/ExportOperatorKernel.h>

using namespace boost::python;
using namespace stromx::cvimgproc;

BOOST_PYTHON_MODULE(libcvimgproc)
{
    def("register", stromxCvimgprocRegister);
    
    stromx::python::exportOperatorKernel<BilateralFilter>("BilateralFilter");
    stromx::python::exportOperatorKernel<Blur>("Blur");
    stromx::python::exportOperatorKernel<BoxFilter>("BoxFilter");
    stromx::python::exportOperatorKernel<Dilate>("Dilate");
    stromx::python::exportOperatorKernel<Erode>("Erode");
    stromx::python::exportOperatorKernel<GaussianBlur>("GaussianBlur");
    stromx::python::exportOperatorKernel<MedianBlur>("MedianBlur");
    stromx::python::exportOperatorKernel<MorphologyEx>("MorphologyEx");
    stromx::python::exportOperatorKernel<Laplacian>("Laplacian");
    stromx::python::exportOperatorKernel<PyrDown>("PyrDown");
    stromx::python::exportOperatorKernel<PyrUp>("PyrUp");
    stromx::python::exportOperatorKernel<Scharr>("Scharr");
    stromx::python::exportOperatorKernel<Sobel>("Sobel");
    stromx::python::exportOperatorKernel<Resize>("Resize");
    stromx::python::exportOperatorKernel<AdaptiveThreshold>("AdaptiveThreshold");
    stromx::python::exportOperatorKernel<Threshold>("Threshold");
    stromx::python::exportOperatorKernel<WarpAffine>("WarpAffine");
    stromx::python::exportOperatorKernel<WarpPerspective>("WarpPerspective");
    stromx::python::exportOperatorKernel<Undistort>("Undistort");
    stromx::python::exportOperatorKernel<UndistortPoints>("UndistortPoints");
}
