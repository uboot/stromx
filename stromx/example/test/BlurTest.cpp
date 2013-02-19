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
#include <stromx/runtime/DataContainer.h>
#include <stromx/runtime/OperatorTester.h>
#include <stromx/runtime/Primitive.h>
#include <stromx/runtime/ReadAccess.h>
#include "stromx/example/Blur.h"
#include "stromx/example/Image.h"
#include "stromx/example/test/BlurTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::example::BlurTest);

namespace stromx
{
    using namespace runtime;

    namespace example
    {
        void BlurTest::setUp ( void )
        {
            m_operator = new runtime::OperatorTester(new Blur());
        }
        
        void BlurTest::testExecuteCopy()
        {
            m_operator->setParameter(Blur::IN_PLACE, Bool(false));
            m_operator->initialize();
            m_operator->activate();
            
            Image* image = new Image("lenna_bw.jpg");
            DataContainer source(image);
            m_operator->setInputData(Blur::SOURCE, source);
            
            DataContainer destination(new Image(image->width(), image->height(), runtime::Image::BAYERBG_8));
            m_operator->setInputData(Blur::DESTINATION, destination);
            
            runtime::DataContainer result = m_operator->getOutputData(Blur::OUTPUT);
            
            ReadAccess<Image> access(result);
            const Image& resultImage = access();
            CPPUNIT_ASSERT_EQUAL(runtime::Image::MONO_8, resultImage.pixelType());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(500), resultImage.width());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(512), resultImage.height());
            
            resultImage.save("BlurTest_testExecuteCopy.png");
        }
        
        void BlurTest::testExecuteGaussianColor()
        {
            m_operator->initialize();
            m_operator->activate();
            
            Image* image = new Image("lenna.jpg");
            DataContainer source(image);
            m_operator->setInputData(Blur::SOURCE, source);
            
            m_operator->setParameter(Blur::FILTER_TYPE, Enum(Blur::GAUSSIAN));
            
            runtime::DataContainer result = m_operator->getOutputData(Blur::OUTPUT);
            
            ReadAccess<Image> access(result);
            const Image& resultImage = access();
            CPPUNIT_ASSERT_EQUAL(runtime::Image::BGR_24, resultImage.pixelType());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(500), resultImage.width());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(512), resultImage.height());
            
            resultImage.save("BlurTest_testExecuteGaussianColor.png");
        }
        
        void BlurTest::testExecuteGaussianMono()
        {
            m_operator->initialize();
            m_operator->activate();
            
            Image* image = new Image("lenna_bw.jpg");
            DataContainer source(image);
            m_operator->setInputData(Blur::SOURCE, source);
            
            m_operator->setParameter(Blur::FILTER_TYPE, Enum(Blur::GAUSSIAN));
            
            runtime::DataContainer result = m_operator->getOutputData(Blur::OUTPUT);
            
            ReadAccess<Image> access(result);
            const Image& resultImage = access();
            CPPUNIT_ASSERT_EQUAL(runtime::Image::MONO_8, resultImage.pixelType());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(500), resultImage.width());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(512), resultImage.height());
            
            resultImage.save("BlurTest_testExecuteGaussianMono.png");
        }

        void BlurTest::testExecuteMedianColor()
        {
            m_operator->initialize();
            m_operator->activate();
            
            Image* image = new Image("lenna.jpg");
            DataContainer source(image);
            m_operator->setInputData(Blur::SOURCE, source);
            
            m_operator->setParameter(Blur::FILTER_TYPE, Enum(Blur::GAUSSIAN));
            
            runtime::DataContainer result = m_operator->getOutputData(Blur::OUTPUT);
            
            ReadAccess<Image> access(result);
            const Image& resultImage = access();
            CPPUNIT_ASSERT_EQUAL(runtime::Image::BGR_24, resultImage.pixelType());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(500), resultImage.width());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(512), resultImage.height());
            
            resultImage.save("BlurTest_testExecuteMedianColor.png");
        }

        void BlurTest::testExecuteMedianMono()
        {
            m_operator->initialize();
            m_operator->activate();
            
            Image* image = new Image("lenna_bw.jpg");
            DataContainer source(image);
            m_operator->setInputData(Blur::SOURCE, source);
            
            m_operator->setParameter(Blur::FILTER_TYPE, Enum(Blur::GAUSSIAN));
            
            runtime::DataContainer result = m_operator->getOutputData(Blur::OUTPUT);
            
            ReadAccess<Image> access(result);
            const Image& resultImage = access();
            CPPUNIT_ASSERT_EQUAL(runtime::Image::MONO_8, resultImage.pixelType());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(500), resultImage.width());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(512), resultImage.height());
            
            resultImage.save("BlurTest_testExecuteMedianMono.png");
        }
        
        void BlurTest::tearDown ( void )
        {
            delete m_operator;
        }
    }
}