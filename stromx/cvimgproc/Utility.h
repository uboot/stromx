#ifndef STROMX_CVIMGPROC_UTILITY_H
#define STROMX_CVIMGPROC_UTILITY_H

#include <opencv2/core/core.hpp>

namespace stromx
{
    namespace cvimgproc
    {
        void calcHist(const cv::Mat & input, cv::Mat & result, const float min, const float max, int size);
        
        void minEnclosingCircle(const cv::Mat & points, cv::Mat & result);
        
        void fitLine(const cv::Mat & points, cv::Mat & result, const int distType,
                     const double param, const double reps, const double aeps);
        
        void extractRectangle(const cv::Mat & image, const cv::RotatedRect& rectangle, cv::Mat & result);
        
    }
}
#endif // STROMX_CVIMGPROC_UTILITY_H
