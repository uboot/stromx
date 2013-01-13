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
#include "../Image.h"
#include "../ImageFilter.h"

#include "MorphologicalFilterTest.h"

namespace stromx
{
    using namespace runtime;

    namespace example
    {
        void MorphologicalFilterTest::setUp ( void )
        {
            m_operator = new runtime::OperatorTester(allocateOperator());
        }
        
        void MorphologicalFilterTest::testExecuteMono()
        {
            m_operator->setParameter(ImageFilter::IN_PLACE, Bool(true));
            m_operator->initialize();
            m_operator->activate();
            
            Image* image = new Image("lenna_bw.jpg");
            DataContainer source(image);
            m_operator->setInputData(ImageFilter::SOURCE, source);
            
            runtime::DataContainer result = m_operator->getOutputData(ImageFilter::OUTPUT);
            
            ReadAccess<Image> access(result);
            const Image& resultImage = access();
            CPPUNIT_ASSERT_EQUAL(runtime::Image::MONO_8, resultImage.pixelType());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(500), resultImage.width());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(512), resultImage.height());
            
            resultImage.save(getTestName() + "_testExecuteMono.png");
        }
        
        void MorphologicalFilterTest::testExecuteColor()
        {
            m_operator->setParameter(ImageFilter::IN_PLACE, Bool(true));
            m_operator->initialize();
            m_operator->activate();
            
            Image* image = new Image("lenna.jpg");
            DataContainer source(image);
            m_operator->setInputData(ImageFilter::SOURCE, source);
            
            runtime::DataContainer result = m_operator->getOutputData(ImageFilter::OUTPUT);
            
            ReadAccess<Image> access(result);
            const Image& resultImage = access();
            CPPUNIT_ASSERT_EQUAL(runtime::Image::BGR_24, resultImage.pixelType());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(500), resultImage.width());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(512), resultImage.height());
            
            resultImage.save(getTestName() + "_testExecuteColor.png");
        }
        
        void MorphologicalFilterTest::tearDown ( void )
        {
            delete m_operator;
        }
    }
}