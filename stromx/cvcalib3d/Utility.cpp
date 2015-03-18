#include "Utility.h"

namespace stromx
{
    namespace cvcalib3d
    {
        void generatePattern(const cv::Size & size, const float squareSize, cv::Mat & output)
        {
            output = cv::Mat(size.width * size.height, 1, CV_32FC3);
            
            for (int j = 0; j < size.height; ++j)
            {
                for (int i = 0; i < size.width; ++i)
                {
                    output.at<cv::Point3f>(i + j * size.width) = 
                        cv::Point3f((size.width - i) * squareSize,
                                    j * squareSize, 0);
                }
            }
        }
        
    }
}
