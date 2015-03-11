#ifndef STROMX_CVIMGPROC_UTILITY_H
#define STROMX_CVIMGPROC_UTILITY_H

#include <opencv2/core/core.hpp>

namespace stromx
{
    namespace cvimgproc
    {
        void calcHist1D(const cv::Mat & input, cv::Mat & result, const float min, const float max, int size);
        
        void minEnclosingCircle(const cv::Mat & points, cv::Mat & result);
        
    }
}
#endif // STROMX_CVIMGPROC_UTILITY_H
