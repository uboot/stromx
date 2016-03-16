/* 
*  Copyright 2015 Matthias Fuchs
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

#include "stromx/runtime/test/VisualizationTest.h"

#include <cppunit/TestAssert.h>
#include "stromx/runtime/Variant.h"
#include "stromx/runtime/Visualization.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::runtime::VisualizationTest);

namespace stromx
{
    namespace runtime
    {
        void VisualizationTest::testPointIsPoint()
        {
            CPPUNIT_ASSERT(Visualization::POINT.isVariant(Visualization::POINT));
        }
        
        void VisualizationTest::testPointIsData()
        {
            CPPUNIT_ASSERT(! Visualization::POINT.isVariant(Variant::DATA));
        }
        
        void VisualizationTest::testPointIsMatrix()
        {
            CPPUNIT_ASSERT(! Visualization::POINT.isVariant(Variant::MATRIX));
        }
        
        void VisualizationTest::testPointIsLine()
        {
            CPPUNIT_ASSERT(! Visualization::POINT.isVariant(Visualization::LINE));
        }
        
        void VisualizationTest::testDataIsHistogram()
        {
            CPPUNIT_ASSERT(! Variant::DATA.isVariant(Visualization::HISTOGRAM));
        }
        
        void VisualizationTest::testMatrixIsLine()
        {
            CPPUNIT_ASSERT(! Variant::MATRIX.isVariant(Visualization::LINE));
        }
    }
}
