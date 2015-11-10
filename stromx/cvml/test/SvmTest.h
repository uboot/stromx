/* 
*  Copyright 2015 Matthias Fuchs
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

#ifndef STROMX_CVML_SVMTEST_H
#define STROMX_CVML_SVMTEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <stromx/runtime/DataContainer.h>

namespace stromx
{
    namespace runtime
    {
        class OperatorTester;
    }

    namespace cvml
    {
        class SvmTest : public CPPUNIT_NS :: TestFixture
        {
            CPPUNIT_TEST_SUITE (SvmTest);
            CPPUNIT_TEST (testExecutePredict);
            CPPUNIT_TEST (testExecutePredictInt32Data);
            CPPUNIT_TEST (testExecutePredictWrongDimension);
            CPPUNIT_TEST (testExecuteTrain);
            CPPUNIT_TEST (testExecuteTrainWrongDimension);
            CPPUNIT_TEST (testGetStatisticalModel);
            CPPUNIT_TEST_SUITE_END ();

            public:
                SvmTest() : m_operator(0) {}
                
                void setUp();
                void tearDown();

            protected:
                void testExecutePredict();
                void testExecutePredictInt32Data();
                void testExecutePredictWrongDimension();
                void testExecuteTrain();
                void testExecuteTrainWrongDimension();
                void testGetStatisticalModel();
                
            private:
                runtime::OperatorTester* m_operator;
        };
    }
}

#endif // STROMX_CVML_SVMTEST_H