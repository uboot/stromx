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
#include "ConvertPixelTypeTest.h"
#include "../ConvertPixelType.h"
#include "../Image.h"
#include <stromx/core/DataContainer.h>
#include <stromx/core/OperatorException.h>
#include <stromx/core/OperatorTester.h>
#include <stromx/core/Primitive.h>
#include <stromx/core/ReadAccess.h>

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::base::ConvertPixelTypeTest);

namespace stromx
{
    using namespace core;

    namespace base
    {
        void ConvertPixelTypeTest::setUp ( void )
        {
            m_operator = new core::OperatorTester(new ConvertPixelType());
            m_operator->initialize();
            m_operator->activate();
            Image* image = new Image("lenna.jpg");
            image->initialize(499, 511, image->stride(), image->data(), image->pixelType());
            m_source = DataContainer(image);
            m_operator->setInputData(ConvertPixelType::SOURCE, m_source);
        }
        
        void ConvertPixelTypeTest::testExecuteMono8()
        {
            m_operator->setParameter(ConvertPixelType::PIXEL_TYPE, Enum(core::Image::MONO_8));
            DataContainer destination(new Image(512, 500, core::Image::BAYERBG_8));
            m_operator->setInputData(ConvertPixelType::DESTINATION, destination);
            
            core::DataContainer result = m_operator->getOutputData(ConvertPixelType::OUTPUT);
            
            ReadAccess<Image> access(result);
            const Image& image = access();
            CPPUNIT_ASSERT_EQUAL(core::Image::MONO_8, image.pixelType());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(499), image.width());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(511), image.height());
            
            image.save("ConvertPixelTypeTest_testExecuteMono8.png");
        }
        
        void ConvertPixelTypeTest::testExecuteBayerBg8()
        {
            m_operator->setParameter(ConvertPixelType::PIXEL_TYPE, Enum(core::Image::BAYERBG_8));
            DataContainer destination(new Image(512, 500, core::Image::MONO_8));
            m_operator->setInputData(ConvertPixelType::DESTINATION, destination);
            
            core::DataContainer result = m_operator->getOutputData(ConvertPixelType::OUTPUT);
            
            ReadAccess<Image> access(result);
            const Image& image = access();
            CPPUNIT_ASSERT_EQUAL(core::Image::BAYERBG_8, image.pixelType());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(499), image.width());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(511), image.height());
            
            image.save("ConvertPixelTypeTest_testExecuteBayerBg8.png");
        }
        
        void ConvertPixelTypeTest::testExecuteBayerRgb24()
        {
            m_operator->setParameter(ConvertPixelType::PIXEL_TYPE, Enum(core::Image::RGB_24));
            DataContainer destination(new Image(512, 500, core::Image::BGR_24));
            m_operator->setInputData(ConvertPixelType::DESTINATION, destination);
            
            core::DataContainer result = m_operator->getOutputData(ConvertPixelType::OUTPUT);
            
            ReadAccess<Image> access(result);
            const Image& image = access();
            CPPUNIT_ASSERT_EQUAL(core::Image::RGB_24, image.pixelType());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(499), image.width());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(511), image.height());
            
            image.save("ConvertPixelTypeTest_testExecuteBayerRgb24.png");
        }
        
        void ConvertPixelTypeTest::testExecuteIdenticalInputs()
        {
            m_operator->setInputData(ConvertPixelType::DESTINATION, m_source);
            core::DataContainer result;
            CPPUNIT_ASSERT_THROW(result = m_operator->getOutputData(ConvertPixelType::OUTPUT), InputError);
        }
        
        void ConvertPixelTypeTest::tearDown ( void )
        {
            delete m_operator;
            m_source = DataContainer();
        }
    }
}