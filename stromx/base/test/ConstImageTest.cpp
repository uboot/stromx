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
#include "ConstImageTest.h"
#include "../ConstImage.h"
#include "../Image.h"
#include <stromx/core/DataContainer.h>
#include <stromx/core/OperatorTester.h>
#include <stromx/core/ReadAccess.h>

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::base::ConstImageTest);

namespace stromx
{
    using namespace core;

    namespace base
    {
        void ConstImageTest::setUp ( void )
        {
            m_operator = new core::OperatorTester(new ConstImage());
            m_operator->initialize();
            m_operator->activate();
            
            Image image("lenna.jpg");
            m_operator->setParameter(ConstImage::IMAGE, image);
        }
        
        void ConstImageTest::testExecute()
        {
            core::DataContainer result = m_operator->getOutputData(ConstImage::OUTPUT);
            
            ReadAccess<Image> access(result);
            const Image& image = access();
            
            image.save("ConstImageTest_testExecute.png");
        }
        
        void ConstImageTest::tearDown ( void )
        {
            delete m_operator;
        }
    }
}
