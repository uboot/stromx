#include "Cvimgproc.h"

#include "BilateralFilter.h"
#include "Blur.h"
#include "BoxFilter.h"
#include "Dilate.h"
#include "Erode.h"
#include "GaussianBlur.h"
#include "MedianBlur.h"
#include "MorphologyEx.h"
#include "Laplacian.h"
#include "PyrDown.h"
#include "PyrUp.h"
#include "Scharr.h"
#include "Sobel.h"
#include "Resize.h"
#include "AdaptiveThreshold.h"
#include "Threshold.h"
#include "WarpAffine.h"
#include "WarpPerspective.h"
#include "Undistort.h"
#include "UndistortPoints.h"
#include "DistanceTransform.h"
#include "FloodFill.h"
#include "Integral.h"
#include "CalcHist1D.h"
#include "Canny.h"
#include "CornerHarris.h"
#include "CornerMinEigenVal.h"
#include "HoughLinesP.h"
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
