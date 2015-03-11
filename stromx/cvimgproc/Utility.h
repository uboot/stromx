#ifndef STROMX_CVIMGPROC_UTILITY_H
#define STROMX_CVIMGPROC_UTILITY_H

#include <opencv2/core/core.hpp>

namespace stromx
{
    namespace cvimgproc
    {
        void calcHist1D(const cv::Mat & input, cv::Mat & result, const float min, const float max, int size);
        
        void minEnclosingCircle(const cv::Mat & points, cv::Mat & result);
        
        void fitLine(const cv::Mat & points, cv::Mat & result, const int distType,
                     const double param, const double reps, const double aeps);
        
    }
}
#endif // STROMX_CVIMGPROC_UTILITY_H
