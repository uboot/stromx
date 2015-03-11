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
#include <stromx/cvimgproc/DistanceTransform.h>
#include <stromx/cvimgproc/FloodFill.h>
#include <stromx/cvimgproc/Integral.h>
#include <stromx/cvimgproc/CalcHist1D.h>
#include <stromx/cvimgproc/FindContours.h>
#include <stromx/cvimgproc/DrawContours.h>
#include <stromx/cvimgproc/ApproxPolyDP.h>
#include <stromx/cvimgproc/BoundingRect.h>
#include <stromx/cvimgproc/ConvexHull.h>
#include <stromx/cvimgproc/FitEllipse.h>
#include <stromx/cvimgproc/FitLine.h>
#include <stromx/cvimgproc/MinAreaRect.h>
#include <stromx/cvimgproc/MinEnclosingCircle.h>
#include <stromx/cvimgproc/Canny.h>
#include <stromx/cvimgproc/CornerHarris.h>
#include <stromx/cvimgproc/CornerMinEigenVal.h>
#include <stromx/cvimgproc/CornerSubPix.h>
#include <stromx/cvimgproc/GoodFeaturesToTrack.h>
#include <stromx/cvimgproc/HoughLinesP.h>
#include <stromx/cvimgproc/PreCornerDetect.h>
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
    stromx::python::exportOperatorKernel<DistanceTransform>("DistanceTransform");
    stromx::python::exportOperatorKernel<FloodFill>("FloodFill");
    stromx::python::exportOperatorKernel<Integral>("Integral");
    stromx::python::exportOperatorKernel<CalcHist1D>("CalcHist1D");
    stromx::python::exportOperatorKernel<FindContours>("FindContours");
    stromx::python::exportOperatorKernel<DrawContours>("DrawContours");
    stromx::python::exportOperatorKernel<ApproxPolyDP>("ApproxPolyDP");
    stromx::python::exportOperatorKernel<BoundingRect>("BoundingRect");
    stromx::python::exportOperatorKernel<ConvexHull>("ConvexHull");
    stromx::python::exportOperatorKernel<FitEllipse>("FitEllipse");
    stromx::python::exportOperatorKernel<FitLine>("FitLine");
    stromx::python::exportOperatorKernel<MinAreaRect>("MinAreaRect");
    stromx::python::exportOperatorKernel<MinEnclosingCircle>("MinEnclosingCircle");
    stromx::python::exportOperatorKernel<Canny>("Canny");
    stromx::python::exportOperatorKernel<CornerHarris>("CornerHarris");
    stromx::python::exportOperatorKernel<CornerMinEigenVal>("CornerMinEigenVal");
    stromx::python::exportOperatorKernel<CornerSubPix>("CornerSubPix");
    stromx::python::exportOperatorKernel<GoodFeaturesToTrack>("GoodFeaturesToTrack");
    stromx::python::exportOperatorKernel<HoughLinesP>("HoughLinesP");
    stromx::python::exportOperatorKernel<PreCornerDetect>("PreCornerDetect");
}
