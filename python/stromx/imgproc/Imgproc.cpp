#include <boost/python.hpp>
#include <stromx/runtime/Registry.h>
#include <stromx/imgproc/Imgproc.h>
#include <stromx/imgproc/BilateralFilter.h>
#include <stromx/imgproc/Blur.h>
#include <stromx/imgproc/BoxFilter.h>
#include <stromx/imgproc/Dilate.h>
#include <stromx/imgproc/Erode.h>
#include <stromx/imgproc/GaussianBlur.h>
#include <stromx/imgproc/MedianBlur.h>
#include <stromx/imgproc/MorphologyEx.h>
#include <stromx/imgproc/Laplacian.h>
#include <stromx/imgproc/PyrDown.h>
#include <stromx/imgproc/PyrUp.h>
#include <stromx/imgproc/Resize.h>
#include <stromx/imgproc/Scharr.h>
#include <stromx/imgproc/Sobel.h>
#include <python/stromx/runtime/ExportOperatorKernel.h>

using namespace boost::python;
using namespace stromx::imgproc;

BOOST_PYTHON_MODULE(libimgproc)
{
    def("registerImgproc", stromxRegisterImgproc);
    
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
    stromx::python::exportOperatorKernel<Resize>("Resize");
    stromx::python::exportOperatorKernel<Scharr>("Scharr");
    stromx::python::exportOperatorKernel<Sobel>("Sobel");
}
