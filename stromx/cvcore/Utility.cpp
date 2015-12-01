#include "Utility.h"

#include <opencv2/core/core.hpp>

namespace stromx
{
    namespace cvcore
    {
        double mean(const cv::Mat & input)
        {
            cv::Scalar value = cv::mean(input);
            return value[0];
        }
        
        double sum(const cv::Mat & input)
        {
            cv::Scalar value = cv::sum(input);
            return value[0];
        }
        
    }
}
