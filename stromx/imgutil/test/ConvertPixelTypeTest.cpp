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
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/OperatorTester.h>
#include <stromx/runtime/Primitive.h>
#include <stromx/runtime/ReadAccess.h>
#include "stromx/imgutil/ConvertPixelType.h"
#include "stromx/imgutil/Image.h"
#include "stromx/imgutil/test/ConvertPixelTypeTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::imgutil::ConvertPixelTypeTest);

namespace stromx
{
    using namespace runtime;

    namespace imgutil
    {
        void ConvertPixelTypeTest::setUp ( void )
        {
            m_operator = new runtime::OperatorTester(new ConvertPixelType());
            m_operator->initialize();
            m_operator->activate();
            Image* image = new Image("lenna.jpg");
            image->initializeImage(499, 511, image->stride(), image->data(), image->pixelType());
            m_source = DataContainer(image);
            m_operator->setInputData(ConvertPixelType::SOURCE, m_source);
        }
        
        void ConvertPixelTypeTest::testExecuteMono8()
        {
            m_operator->setParameter(ConvertPixelType::PIXEL_TYPE, Enum(runtime::Image::MONO_8));
            DataContainer destination(new Image(512, 500, runtime::Image::BAYERBG_8));
            m_operator->setInputData(ConvertPixelType::DESTINATION, destination);
            
            runtime::DataContainer result = m_operator->getOutputData(ConvertPixelType::OUTPUT);
            
            ReadAccess<runtime::Image> access(result);
            const runtime::Image& image = access();
            CPPUNIT_ASSERT_EQUAL(runtime::Image::MONO_8, image.pixelType());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(499), image.width());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(511), image.height());
            
            imgutil::Image::save("ConvertPixelTypeTest_testExecuteMono8.png", image);
        }
        
        void ConvertPixelTypeTest::testExecuteBayerBg8()
        {
            m_operator->setParameter(ConvertPixelType::PIXEL_TYPE, Enum(runtime::Image::BAYERBG_8));
            DataContainer destination(new Image(512, 500, runtime::Image::MONO_8));
            m_operator->setInputData(ConvertPixelType::DESTINATION, destination);
            
            runtime::DataContainer result = m_operator->getOutputData(ConvertPixelType::OUTPUT);
            
            ReadAccess<runtime::Image> access(result);
            const runtime::Image& image = access();
            CPPUNIT_ASSERT_EQUAL(runtime::Image::BAYERBG_8, image.pixelType());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(499), image.width());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(511), image.height());
            
            imgutil::Image::save("ConvertPixelTypeTest_testExecuteBayerBg8.png", image);
        }
        
        void ConvertPixelTypeTest::testExecuteBayerRgb24()
        {
            m_operator->setParameter(ConvertPixelType::PIXEL_TYPE, Enum(runtime::Image::RGB_24));
            DataContainer destination(new Image(512, 500, runtime::Image::BGR_24));
            m_operator->setInputData(ConvertPixelType::DESTINATION, destination);
            
            runtime::DataContainer result = m_operator->getOutputData(ConvertPixelType::OUTPUT);
            
            ReadAccess<runtime::Image> access(result);
            const runtime::Image& image = access();
            CPPUNIT_ASSERT_EQUAL(runtime::Image::RGB_24, image.pixelType());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(499), image.width());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(511), image.height());
            
            imgutil::Image::save("ConvertPixelTypeTest_testExecuteBayerRgb24.png", image);
        }
        
        void ConvertPixelTypeTest::testExecuteIdenticalInputs()
        {
            m_operator->setInputData(ConvertPixelType::DESTINATION, m_source);
            runtime::DataContainer result;
            CPPUNIT_ASSERT_THROW(result = m_operator->getOutputData(ConvertPixelType::OUTPUT), InputError);
        }
        
        void ConvertPixelTypeTest::tearDown ( void )
        {
            delete m_operator;
            m_source = DataContainer();
        }
    }
}