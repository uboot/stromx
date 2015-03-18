#ifndef STROMX_CVCALIB_3D_UTILITY_H
#define STROMX_CVCALIB_3D_UTILITY_H

#include <opencv2/core/core.hpp>

namespace stromx
{
    namespace cvcalib3d
    {
        void generatePattern(const cv::Size & size, const float squareSize, cv::Mat & output);
        
    }
}
#endif // STROMX_CVCALIB_3D_UTILITY_H
