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
#include "stromx/cvimgutil/Clip.h"
#include "stromx/cvimgutil/Image.h"
#include "stromx/cvimgutil/test/ClipTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::cvimgutil::ClipTest);

namespace stromx
{
    using namespace runtime;

    namespace cvimgutil
    {
        void ClipTest::setUp ( void )
        {
            m_operator = new runtime::OperatorTester(new Clip());
            m_operator->initialize();
            m_operator->activate();
            DataContainer image(new Image("lenna.jpg"));
            m_operator->setInputData(Clip::INPUT, image);
        }
        
        void ClipTest::testExecute()
        {
            m_operator->setParameter(Clip::LEFT, UInt32(200));
            m_operator->setParameter(Clip::TOP, UInt32(210));
            m_operator->setParameter(Clip::WIDTH, UInt32(100));
            m_operator->setParameter(Clip::HEIGHT, UInt32(90));

            runtime::DataContainer result = m_operator->getOutputData(Clip::OUTPUT);
                
            ReadAccess<runtime::Image> access(result);
            const runtime::Image& image = access();
            CPPUNIT_ASSERT_EQUAL((unsigned int)(100), image.width());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(90), image.height());
            
            cvimgutil::Image::save("ClipTest_testExecute.png", image);
        }
        
        void ClipTest::testAdjustClipRegion1()
        {
            m_operator->setParameter(Clip::LEFT, UInt32(450));
            m_operator->setParameter(Clip::TOP, UInt32(500));
            m_operator->setParameter(Clip::WIDTH, UInt32(100));
            m_operator->setParameter(Clip::HEIGHT, UInt32(90));
            
            runtime::DataContainer result = m_operator->getOutputData(Clip::OUTPUT);
            ReadAccess<runtime::Image> access(result);
            
            const runtime::Image& image = access();
            CPPUNIT_ASSERT_EQUAL((unsigned int)(50), image.width());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(12), image.height());
            
            cvimgutil::Image::save("ClipTest_testAdjustClipRegion1.png", image);
        }
        
        void ClipTest::testAdjustClipRegion2()
        {
            m_operator->setParameter(Clip::LEFT, UInt32(650));
            m_operator->setParameter(Clip::TOP, UInt32(700));
            m_operator->setParameter(Clip::WIDTH, UInt32(100));
            m_operator->setParameter(Clip::HEIGHT, UInt32(90));
            
            runtime::DataContainer result = m_operator->getOutputData(Clip::OUTPUT);
            ReadAccess<runtime::Image> access(result);
            
            const runtime::Image& image = access();
            CPPUNIT_ASSERT_EQUAL((unsigned int)(0), image.width());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(0), image.height());
        }

        void ClipTest::tearDown ( void )
        {
            delete m_operator;
        }
    }
}
