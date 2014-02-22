#include "stromx/cvimgproc/Cvimgproc.h"

#include "stromx/cvimgproc/BilateralFilter.h"
#include "stromx/cvimgproc/Blur.h"
#include "stromx/cvimgproc/BoxFilter.h"
#include "stromx/cvimgproc/Dilate.h"
#include "stromx/cvimgproc/Erode.h"
#include "stromx/cvimgproc/GaussianBlur.h"
#include "stromx/cvimgproc/MedianBlur.h"
#include "stromx/cvimgproc/MorphologyEx.h"
#include "stromx/cvimgproc/Laplacian.h"
#include "stromx/cvimgproc/PyrDown.h"
#include "stromx/cvimgproc/PyrUp.h"
#include "stromx/cvimgproc/Scharr.h"
#include "stromx/cvimgproc/Sobel.h"
#include "stromx/cvimgproc/Resize.h"
#include "stromx/cvimgproc/AdaptiveThreshold.h"
#include "stromx/cvimgproc/Threshold.h"
#include "stromx/cvimgproc/WarpAffine.h"
#include "stromx/cvimgproc/WarpPerspective.h"
#include "stromx/cvimgproc/Undistort.h"
#include "stromx/cvimgproc/UndistortPoints.h"
#include "stromx/cvimgproc/DistanceTransform.h"
#include "stromx/cvimgproc/FloodFill.h"
#include "stromx/cvimgproc/Integral.h"
#include "stromx/cvimgproc/CalcHist1D.h"
#include "stromx/cvimgproc/Canny.h"
#include "stromx/cvimgproc/CornerHarris.h"
#include "stromx/cvimgproc/CornerMinEigenVal.h"
#include "stromx/cvimgproc/HoughLinesP.h"
#include <stromx/runtime/Registry.h>

void stromxCvimgprocRegister(stromx::runtime::Registry& registry)
{
    using namespace stromx::cvimgproc;
    
    registry.registerOperator(new BilateralFilter);
    registry.registerOperator(new Blur);
    registry.registerOperator(new BoxFilter);
    registry.registerOperator(new Dilate);
    registry.registerOperator(new Erode);
    registry.registerOperator(new GaussianBlur);
    registry.registerOperator(new MedianBlur);
    registry.registerOperator(new MorphologyEx);
    registry.registerOperator(new Laplacian);
    registry.registerOperator(new PyrDown);
    registry.registerOperator(new PyrUp);
    registry.registerOperator(new Scharr);
    registry.registerOperator(new Sobel);
    registry.registerOperator(new Resize);
    registry.registerOperator(new AdaptiveThreshold);
    registry.registerOperator(new Threshold);
    registry.registerOperator(new WarpAffine);
    registry.registerOperator(new WarpPerspective);
    registry.registerOperator(new Undistort);
    registry.registerOperator(new UndistortPoints);
    registry.registerOperator(new DistanceTransform);
    registry.registerOperator(new FloodFill);
    registry.registerOperator(new Integral);
    registry.registerOperator(new CalcHist1D);
    registry.registerOperator(new Canny);
    registry.registerOperator(new CornerHarris);
    registry.registerOperator(new CornerMinEigenVal);
    registry.registerOperator(new HoughLinesP);
}
