#include "Utility.h"

#include <opencv2/imgproc/imgproc.hpp>
#include <cmath>

namespace stromx
{
    namespace cvimgproc
    {
        void calcHist(const cv::Mat & input, cv::Mat & result, const float min, const float max, int size)
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
        
        void extractRectangle(const cv::Mat & image, const cv::RotatedRect& rectangle, cv::Mat & result)
        {
            cv::Rect bbox = rectangle.boundingRect();
            bbox.x = std::min(std::max(bbox.x, 0), image.cols - 1);
            bbox.y = std::min(std::max(bbox.y, 0), image.rows - 1);
            bbox.width = std::min(std::max(bbox.width, 1), image.cols - bbox.x);
            bbox.height = std::min(std::max(bbox.height, 1), image.rows - bbox.y);
            
            cv::Mat cropped = image(bbox);
            
            float angle = rectangle.angle;
            cv::Size size = rectangle.size;
            
            if (rectangle.angle < -45.)
            {
                angle += 90.0;
                std::swap(size.width, size.height);
            }
            
            cv::Point2f shiftedCenter = rectangle.center - cv::Point2f(bbox.x, bbox.y);
            cv::Mat transform = cv::getRotationMatrix2D(shiftedCenter, angle, 1.0);
            
            cv::Mat rotated;
            cv::warpAffine(cropped, rotated, transform, cropped.size(), cv::INTER_CUBIC);
            cv::getRectSubPix(rotated, rectangle.size, shiftedCenter, result);
        }
        
    }
}
