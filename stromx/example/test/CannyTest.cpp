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

#include <cppunit/TestAssert.h>
#include <stromx/runtime/DataContainer.h>
#include <stromx/runtime/OperatorTester.h>
#include <stromx/runtime/Primitive.h>
#include <stromx/runtime/ReadAccess.h>
#include "stromx/example/Canny.h"
#include "stromx/example/Image.h"
#include "stromx/example/test/CannyTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::example::CannyTest);

namespace stromx
{
    using namespace runtime;

    namespace example
    {
        void CannyTest::setUp ( void )
        {
            m_operator = new runtime::OperatorTester(new Canny());
        }
        
        void CannyTest::testExecute()
        {
            m_operator->setParameter(Canny::IN_PLACE, Bool(false));
            m_operator->initialize();
            m_operator->activate();
            
            Image* image = new Image("lenna_bw.jpg");
            DataContainer source(image);
            m_operator->setInputData(Canny::SOURCE, source);
            
            DataContainer destination(new Image(image->width(), image->height(), runtime::Image::BAYERBG_8));
            m_operator->setInputData(Canny::DESTINATION, destination);
            
            m_operator->setParameter(Canny::THRESHOLD_1, Double(50));
            m_operator->setParameter(Canny::THRESHOLD_2, Double(250));
            
            runtime::DataContainer result = m_operator->getOutputData(Canny::OUTPUT);
            
            ReadAccess<Image> access(result);
            const Image& resultImage = access();
            CPPUNIT_ASSERT_EQUAL(runtime::Image::MONO_8, resultImage.pixelType());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(500), resultImage.width());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(512), resultImage.height());
            
            resultImage.save("CannyTest_testExecute.png");
        }
        
        void CannyTest::testExecuteInPlace()
        {
            m_operator->initialize();
            m_operator->activate();
            
            Image* image = new Image("lenna_bw.jpg");
            DataContainer source(image);
            m_operator->setInputData(Canny::SOURCE, source);
            
            m_operator->setParameter(Canny::THRESHOLD_1, Double(50));
            m_operator->setParameter(Canny::THRESHOLD_2, Double(250));
            
            runtime::DataContainer result = m_operator->getOutputData(Canny::OUTPUT);
            
            ReadAccess<Image> access(result);
            const Image& resultImage = access();
            CPPUNIT_ASSERT_EQUAL(runtime::Image::MONO_8, resultImage.pixelType());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(500), resultImage.width());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(512), resultImage.height());
            
            resultImage.save("CannyTest_testExecuteInPlace.png");
        }
        
        void CannyTest::tearDown ( void )
        {
            delete m_operator;
        }
    }
}