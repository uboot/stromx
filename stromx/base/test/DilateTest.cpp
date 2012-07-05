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
#include "DilateTest.h"
#include "../Dilate.h"
#include "../Image.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::base::DilateTest);

namespace stromx
{
    using namespace core;

    namespace base
    {
        void DilateTest::setUp ( void )
        {
            m_operator = new core::OperatorTester(new Dilate());
        }
        
        void DilateTest::testExecuteMono()
        {
            m_operator->initialize();
            m_operator->activate();
            
            Image* image = new Image("lenna_bw.jpg");
            DataContainer source(image);
            m_operator->setInputData(Dilate::SOURCE, source);
            
            core::DataContainer result = m_operator->getOutputData(Dilate::OUTPUT);
            
            ReadAccess<Image> access(result);
            const Image& resultImage = access();
            CPPUNIT_ASSERT_EQUAL(core::Image::MONO_8, resultImage.pixelType());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(500), resultImage.width());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(512), resultImage.height());
            
            resultImage.save("DilateTest_testExecuteMono.png");
        }
        
        void DilateTest::tearDown ( void )
        {
            delete m_operator;
        }
    }
}