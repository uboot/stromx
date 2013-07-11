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
#include "stromx/cvsupport/AdjustRgbChannels.h"
#include "stromx/cvsupport/Image.h"
#include "stromx/cvsupport/test/AdjustRgbChannelsTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::cvsupport::AdjustRgbChannelsTest);

namespace stromx
{
    using namespace runtime;

    namespace cvsupport
    {
        void AdjustRgbChannelsTest::setUp ( void )
        {
            m_operator = new runtime::OperatorTester(new AdjustRgbChannels());
            m_operator->initialize();
            m_operator->activate();
            DataContainer image(new Image("lenna.jpg"));
            m_operator->setInputData(AdjustRgbChannels::INPUT, image);
        }
        
        void AdjustRgbChannelsTest::testExecute()
        {
            m_operator->setParameter(AdjustRgbChannels::RED, Float64(0.1));
            m_operator->setParameter(AdjustRgbChannels::GREEN, Float64(1.0));
            m_operator->setParameter(AdjustRgbChannels::BLUE, Float64(1.5));
            
            runtime::DataContainer result = m_operator->getOutputData(AdjustRgbChannels::OUTPUT);
            ReadAccess<runtime::Image> access(result);
            const runtime::Image& image = access();
            
            cvsupport::Image::save("AdjustRgbChannelsTest_testExecute.png", image);
        }
        
        void AdjustRgbChannelsTest::tearDown ( void )
        {
            delete m_operator;
        }
    }
}
