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

#include "stromx/cvml/test/SvmTest.h"

#include <cppunit/TestAssert.h>

#include <stromx/cvsupport/Matrix.h>
#include <stromx/runtime/File.h>
#include <stromx/runtime/Id2DataMapper.h>
#include <stromx/runtime/OperatorTester.h>
#include <stromx/runtime/ReadAccess.h>

#include "stromx/cvml/Svm.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::cvml::SvmTest);

namespace stromx
{
    using namespace runtime;

    namespace cvml
    {
        void SvmTest::setUp ( void )
        {
            m_operator = new runtime::OperatorTester(new Svm());
            m_operator->initialize();
            m_operator->activate();
        }
        
        void SvmTest::testExecute()
        {
            File modelFile("svm_model.xml");
            m_operator->setParameter(Svm::STATISTICAL_MODEL, modelFile);
            
            cvsupport::Matrix* data = new cvsupport::Matrix(1, 3, runtime::Matrix::FLOAT_32);
            data->at<float>(0, 0) = 0.0;
            data->at<float>(0, 1) = 0.0;
            data->at<float>(0, 2) = -1.0;
            
            DataContainer input(data);
            m_operator->setInputData(Svm::DATA, input);
            
            DataContainer output = m_operator->getOutputData(Svm::PREDICTED_RESPONSE);
            ReadAccess access(output);
            const Float32 & response = access.get<Float32>();
            CPPUNIT_ASSERT_DOUBLES_EQUAL(float(-1.0), static_cast<float>(response), 0.001);
        }
        
        void SvmTest::tearDown ( void )
        {
            delete m_operator;
        }
    }
}
