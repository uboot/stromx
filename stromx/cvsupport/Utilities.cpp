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

#include "stromx/cvsupport/Utilities.h"

#include <stromx/runtime/DataContainer.h>
#include <stromx/runtime/EnumParameter.h>
#include <stromx/runtime/Exception.h>
#include <stromx/runtime/List.h>
#include <stromx/runtime/MatrixDescription.h>
#include <stromx/runtime/MatrixParameter.h>

#include "stromx/cvsupport/Matrix.h"
#include "stromx/cvsupport/Image.h"

namespace stromx
{
    using namespace runtime;

    namespace cvsupport
    {
        cv::Mat getOpenCvMat(const runtime::Image& image)
        {
            int cvType = Image::cvTypeFromPixelType(image.pixelType());
            uint8_t* data = const_cast<uint8_t*>(image.data());
            
            return cv::Mat(image.height(), image.width(), cvType, data, image.stride());
        }
        
        cv::Mat getOpenCvMat(const runtime::Matrix& matrix, const unsigned int numChannels)
        {
            if(matrix.cols() % numChannels != 0)
                throw WrongArgument("Number of input matrix rows must be multiple of the number of output channels.");
            
            int cvType = Matrix::cvTypeFromValueType(matrix.valueType(), numChannels);
            uint8_t* data = const_cast<uint8_t*>(matrix.data());
            
            return cv::Mat(matrix.rows(), matrix.cols() / numChannels, cvType, data, matrix.stride());
        }
        
        std::vector<cv::Mat> getOpenCvMatVector(const runtime::List& list)
        {
            std::vector<cv::Mat> vector;
            std::vector<const Data*> content = list.content();
            for(std::vector<const Data*>::const_iterator iter = content.begin();
                iter != content.end(); ++iter)
            {
                const runtime::Matrix* matrix = runtime::data_cast<runtime::Matrix>(*iter);
                vector.push_back(getOpenCvMat(*matrix));
            }
            return vector;
        }
        
        runtime::Image::PixelType computeOutPixelType(const int outDdepth,
                                    const runtime::Image::PixelType inPixelType)
        {
            int inDdepth = 0;
            switch(inPixelType)
            {
            case runtime::Image::MONO_8:
            case runtime::Image::RGB_24:
            case runtime::Image::BGR_24:
                inDdepth = 0;
                break;
            case runtime::Image::MONO_16:
            case runtime::Image::RGB_48:
            case runtime::Image::BGR_48:
                inDdepth = 1;
                break;
            default:
                throw runtime::WrongArgument("Invalid input pixel type.");
            }
            
            int ddepth = 0;
            switch(outDdepth)
            {
            case -1:
                ddepth = inDdepth;
                break;
            case CV_8U:
                ddepth = 0;
                break;
            case CV_16U:
                ddepth = 1;
                break;
            default:
                throw runtime::WrongArgument("Invalid output pixel depth.");
            }
            
            int pixelType = 0;
            switch(inPixelType)
            {
            case runtime::Image::MONO_8:
            case runtime::Image::MONO_16:
                pixelType = 0;
                break;
            case runtime::Image::RGB_24:
            case runtime::Image::RGB_48:
                pixelType = 1;
                break;
            case runtime::Image::BGR_24:
            case runtime::Image::BGR_48:
                pixelType = 2;
                break;
            default:
                throw runtime::WrongArgument("Invalid input pixel type.");
            }
                
            
            // the decision table
            int table[2][3] =
            /* ddepth      0  0  0    1  1  1 
             * pixelType   0  1  2    0  1  2 */
            /* result */ {{0, 1, 2}, {3, 4, 5}};
            
            int result = table[ddepth][pixelType];
            
            switch(result)
            {
            case 0:
                return runtime::Image::MONO_8;
            case 1:
                return runtime::Image::RGB_24;
            case 2:
                return runtime::Image::BGR_24;
            case 3:
                return runtime::Image::MONO_16;
            case 4:
                return runtime::Image::RGB_48;
            case 5:
                return runtime::Image::BGR_48;
            default:
                throw runtime::InternalError("Invalid output pixel type.");
            }
        }
        
        void checkEnumValue(const stromx::runtime::Enum & value, const stromx::runtime::EnumParameter* param, const stromx::runtime::OperatorInfo& op)
        {
            using namespace runtime;
            
            for(std::vector<EnumDescription>::const_iterator iter = param->descriptions().begin(); iter != param->descriptions().end(); ++iter)
            {
                 if(value == iter->value())
                return;
            }
            throw stromx::runtime::WrongParameterValue(*param, op);
        }
        
        void checkMatrixValue(const stromx::runtime::Matrix & value,
                             const stromx::runtime::MatrixDescription* desc,
                             const stromx::runtime::OperatorInfo& op)
        {
            if(desc->rows() && value.rows() != desc->rows())
            {
                std::ostringstream str;
                str << desc->rows();
                throw runtime::InputError(desc->id(), op, "Number of matrix rows must be " + str.str() + " .");
            }
            
            if(desc->cols() && value.cols() != desc->cols())
            {
                std::ostringstream str;
                str << desc->cols();
                throw runtime::InputError(desc->id(), op, "Number of matrix columns must be " + str.str() + " .");
            }
        }
        
        void checkMatrixValue(const stromx::runtime::Matrix & value,
                              const stromx::runtime::MatrixParameter* param,
                              const stromx::runtime::OperatorInfo& op)
        {
            if(param->rows() && value.rows() != param->rows())
            {
                std::ostringstream str;
                str << param->rows();
                throw runtime::WrongParameterValue(*param, op, "Number of matrix rows must be " + str.str() + " .");
            }
            
            if(param->cols() && value.cols() != param->cols())
            {
                std::ostringstream str;
                str << param->cols();
                throw runtime::WrongParameterValue(*param, op, "Number of matrix columns must be " + str.str() + " .");
            }
        }
    }
}
