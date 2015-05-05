/* 
*  Copyright 2013 Matthias Fuchs
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

#include <cppunit/TestAssert.h>
#include <stromx/runtime/Data.h>
#include <stromx/runtime/Factory.h>
#include <stromx/runtime/Operator.h>
#include "stromx/cvsupport/Matrix.h"
#include "stromx/cvsupport/Utilities.h"
#include "stromx/cvsupport/test/UtilitiesTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::cvsupport::UtilitiesTest);

namespace stromx
{
    namespace cvsupport
    {
        void UtilitiesTest::testComputeOutPixelTypeSame()
        {
            int outDdepth = -1;
            runtime::Image::PixelType inPixelType = runtime::Image::MONO_16;
            CPPUNIT_ASSERT_EQUAL(computeOutPixelType(outDdepth, inPixelType), 
                                 runtime::Image::MONO_16);
        }
        
        void UtilitiesTest::testComputeOutPixelType8Bit()
        {
            int outDdepth = CV_8U;
            runtime::Image::PixelType inPixelType = runtime::Image::RGB_48;
            CPPUNIT_ASSERT_EQUAL(computeOutPixelType(outDdepth, inPixelType), 
                                 runtime::Image::RGB_24);
        }
        
        void UtilitiesTest::testComputeOutPixelType16Bit()
        {
            int outDdepth = CV_16U;
            runtime::Image::PixelType inPixelType = runtime::Image::BGR_24;
            CPPUNIT_ASSERT_EQUAL(computeOutPixelType(outDdepth, inPixelType), 
                                 runtime::Image::BGR_48);
        }
        
        void UtilitiesTest::testGetOpenCvMat3Channels()
        {
            cvsupport::Matrix matrix(5, 3, runtime::Matrix::INT_16);
            
            cv::Mat cvMat = getOpenCvMat(matrix, 3);
            CPPUNIT_ASSERT_EQUAL(cvMat.cols, 1);
            CPPUNIT_ASSERT_EQUAL(cvMat.type(), CV_16SC3);
        }
        
        void UtilitiesTest::testGetOpenCvRotatedRect()
        {
            cvsupport::Matrix matrix(1, 5, runtime::Matrix::FLOAT_32);
            matrix.at<float>(0, 0) = 150;
            matrix.at<float>(0, 1) = 50;
            matrix.at<float>(0, 2) = 200;
            matrix.at<float>(0, 3) = 100;
            matrix.at<float>(0, 4) = -20;
            
            cv::RotatedRect cvRect = getOpenCvRotatedRect(matrix);
            
            CPPUNIT_ASSERT_DOUBLES_EQUAL(150, cvRect.center.x, 1e-7);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(50, cvRect.center.y, 1e-7);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(200, cvRect.size.width, 1e-7);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(100, cvRect.size.height, 1e-7);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(-20, cvRect.angle, 1e-7);
        }
    }
}
