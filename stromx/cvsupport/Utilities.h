/* 
*  Copyright 2011 Matthias Fuchs
*
*  Licensed under the Apache License, Version 2.0 (the "License");
*  you may not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS,
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
*/

#ifndef STROMX_CVSUPPORT_UTILITIES_H
#define STROMX_CVSUPPORT_UTILITIES_H

#include <opencv2/core/core.hpp>

#include <stromx/runtime/Image.h>
#include <stromx/runtime/OperatorException.h>

#include "stromx/cvsupport/Config.h"

namespace stromx
{
    namespace runtime
    {
        class DataContainer;
        class EnumParameter;
        class List;
        class MatrixDescription;
        class MatrixParameter;
        class OperatorInfo;
    }

    namespace cvsupport
    {
        class Image;
        
        /** Returns an OpenCV matrix header for \c image. */
        STROMX_CVSUPPORT_API cv::Mat getOpenCvMat(const runtime::Image& image);
        
        /** Returns an OpenCV matrix header for \c matrix. */
        STROMX_CVSUPPORT_API cv::Mat getOpenCvMat(const runtime::Matrix& matrix,
                                                  const unsigned int numChannels = 1);
        
        /** 
         * Converts a stromx list of matrices to a vector of OpenCV matrices.
         * \throws BadCast If one of the values in \c list can not be casted to a matrix.
         */
        STROMX_CVSUPPORT_API std::vector<cv::Mat> getOpenCvMatVector(const runtime::List& list);
        
        /** 
         * Computes the stromx pixel type of the input type \c inPixelType 
         * after conversion to the OpenCV depth \c outDdepth.
         */
        STROMX_CVSUPPORT_API runtime::Image::PixelType computeOutPixelType(const int outDdepth,
                                    const runtime::Image::PixelType inPixelType);
         
        /** 
         * Checks if \c value is within the limits (minimum, maximum) defined 
         * by \c param.
         * \throws WrongParameterValue If this is not the case.
         */                           
        template<class T>
        void checkNumericValue(const T & value, const runtime::NumericParameter<T>* param, const stromx::runtime::OperatorInfo& op)
        {
            if(value < runtime::data_cast<T>(param->min()))
                throw runtime::WrongParameterValue(*param, op);
            if(value > runtime::data_cast<T>(param->max()))
                throw runtime::WrongParameterValue(*param, op);
        }
        
        /** 
         * Checks if \c value is one of the valid values defined by \c param.
         * by \c param.
         * \throws WrongParameterValue If this is not the case.
         */
        STROMX_CVSUPPORT_API void checkEnumValue(const stromx::runtime::Enum & value, const stromx::runtime::EnumParameter* param, const stromx::runtime::OperatorInfo& op);
        
        /** 
         * Checks if the size of \c value conforms with the dimensions defined by \c param.
         * \throws WrongParameterValue If this is not the case.
         */
        STROMX_CVSUPPORT_API void checkMatrixValue(const stromx::runtime::Matrix & value,
                              const stromx::runtime::MatrixParameter* param,
                              const stromx::runtime::OperatorInfo& op);
              
        /** 
         * Checks if the size of \c value conforms with the dimensions defined by \c desc.
         * \throws InputError If this is not the case.
         */                
        STROMX_CVSUPPORT_API void checkMatrixValue(const stromx::runtime::Matrix & value,
                             const stromx::runtime::MatrixDescription* desc,
                             const stromx::runtime::OperatorInfo& op);
        
    }
}

#endif // STROMX_CVSUPPORT_UTILITIES_H
