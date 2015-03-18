#include "Utility.h"

#include <opencv2/imgproc/imgproc.hpp>
#include <cmath>

namespace stromx
{
    namespace cvimgproc
    {
        void calcHist1D(const cv::Mat & input, cv::Mat & result, const float min, const float max, int size)
        {
            int channels[] = {0};
            float range[] = {min, max};
            const float* ranges[] = {range};
            
            cv::calcHist(&input, 1, channels, cv::Mat(), result, 1, &size, ranges);
        }
        
        void minEnclosingCircle(const cv::Mat & points, cv::Mat & result)
        {
            cv::Point2f center;
            float radius;
            cv::minEnclosingCircle(points, center, radius);
            
            result = cv::Mat(1, 3, CV_32F);
            result.at<float>(0, 0) = center.x;
            result.at<float>(0, 1) = center.y;
            result.at<float>(0, 2) = radius;
        }
        
        void fitLine(const cv::Mat & points, cv::Mat & result, const int distType,
                     const double param, const double reps, const double aeps)
        {
            cv::Vec4f line;
            cv::fitLine(points, line, distType, param, reps, aeps);
            
            result = cv::Mat(1, 2, CV_32F);
            result.at<float>(0, 0) = (line[1]*line[2] - line[0]*line[3]);
            result.at<float>(0, 1) = std::atan2(line[0], line[1]) * 180 / M_PI;
        }
        
    }
}
