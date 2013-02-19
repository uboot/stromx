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
#include "stromx/example/HoughLinesP.h"
#include "stromx/example/Matrix.h"
#include "stromx/example/test/HoughLinesPTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::example::HoughLinesPTest);

namespace stromx
{
    namespace example
    {
        void HoughLinesPTest::setUp ( void )
        {
            m_operator = new runtime::OperatorTester(new HoughLinesP());
            m_operator->initialize();
            m_operator->activate();
        }
        
        void HoughLinesPTest::testExecute()
        {            
            Image* image = new Image("edges.png");
            runtime::DataContainer source(image);
            m_operator->setInputData(HoughLinesP::IMAGE, source);
            
            runtime::DataContainer result = m_operator->getOutputData(HoughLinesP::LINES);
            runtime::ReadAccess<Matrix> access(result);
            const Matrix& resultLines = access();
            
            resultLines.save("HoughLinesPTest_testExecute.npy");
        }
        
        void HoughLinesPTest::tearDown ( void )
        {
            delete m_operator;
        }
    }
}
