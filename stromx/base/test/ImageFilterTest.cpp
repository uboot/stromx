/* 
*  Copyright 2012 Matthias Fuchs
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
#include <stromx/core/DataContainer.h>
#include <stromx/core/OperatorTester.h>
#include <stromx/core/Primitive.h>
#include <stromx/core/ReadAccess.h>

#include "ImageFilterTest.h"

namespace stromx
{
    namespace base
    {
        class ImageFilterTester : public ImageFilter
        {
        public:
            ImageFilterTester()
              : ImageFilter("Test")
            {
            }
            
            OperatorKernel*const clone() const
            {
                return new ImageFilterTester;
            } 
            
            const std::vector< const stromx::core::Parameter* > setupInitParameters()
            {
                return std::vector<const core::Parameter*>();
            }
    
            void applyFilter ( const cv::Mat& in, cv::Mat& out )
            {
            }
            
        };
        
        void ImageFilterTest::setUp()
        {
            m_operator = new core::OperatorTester(new ImageFilterTester);
        }

        void ImageFilterTest::testOneInput()
        {

        }

        void ImageFilterTest::testTwoInputs()
        {

        }


        void ImageFilterTest::testIdenticalInputs()
        {

        }

        void ImageFilterTest::tearDown()
        {
            delete m_operator;
        }
    }
}
