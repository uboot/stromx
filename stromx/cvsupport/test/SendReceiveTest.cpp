/* 
*  Copyright 2013 Matthias Fuchs
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

#include "stromx/cvsupport/Image.h"
#include "stromx/cvsupport/Matrix.h"
#include "stromx/runtime/Factory.h"
#include "stromx/runtime/OperatorTester.h"
#include "stromx/runtime/ReadAccess.h"
#include "stromx/runtime/Receive.h"
#include "stromx/runtime/Send.h"

#include "stromx/cvsupport/test/SendReceiveTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::cvsupport::SendReceiveTest);

namespace stromx
{
    using namespace runtime;

    namespace cvsupport
    {
        void SendReceiveTest::setUp ( void )
        {
            m_factory = new Factory();
            m_send = new runtime::OperatorTester(new Send());
            m_receive = new runtime::OperatorTester(new Receive());
            m_receive->setFactory(m_factory);
            
            m_send->initialize();
            m_receive->initialize();
            
            m_send->setParameter(Send::PORT, UInt16(49152));
            m_receive->setParameter(Receive::URL, String("localhost"));
            m_receive->setParameter(Receive::PORT, UInt16(49152));
            
            m_send->activate();
            m_receive->activate();
        }
        
        void cvsupport::SendReceiveTest::testSendImage()
        {
            DataContainer in(new Image("lenna.jpg"));
            m_send->setInputData(Send::INPUT, in);
            
            m_factory->registerData(new Image);
            DataContainer out = m_receive->getOutputData(Receive::OUTPUT);
            ReadAccess<stromx::runtime::Image> access(out);
            
            CPPUNIT_ASSERT_EQUAL((unsigned int)(500), access().width());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(512), access().height());
            CPPUNIT_ASSERT_EQUAL(Image::BGR_24, access().pixelType());
        }   
        
        void cvsupport::SendReceiveTest::testSendGrayImage()
        {
            DataContainer in(new Image("lenna_bw.jpg"));
            m_send->setInputData(Send::INPUT, in);
            
            m_factory->registerData(new Image);
            DataContainer out = m_receive->getOutputData(Receive::OUTPUT);
            ReadAccess<stromx::runtime::Image> access(out);
            
            CPPUNIT_ASSERT_EQUAL((unsigned int)(500), access().width());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(512), access().height());
            CPPUNIT_ASSERT_EQUAL(Image::MONO_8, access().pixelType());
        }
        
        void cvsupport::SendReceiveTest::testSendDoubleMatrix()
        {
            DataContainer in(new Matrix("double_matrix.npy"));
            m_send->setInputData(Send::INPUT, in);
            
            m_factory->registerData(new Matrix);
            DataContainer out = m_receive->getOutputData(Receive::OUTPUT);
            ReadAccess<stromx::runtime::Matrix> access(out);
            
            CPPUNIT_ASSERT_EQUAL((unsigned int)(50), access().rows());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(100), access().cols());
            CPPUNIT_ASSERT_EQUAL(Matrix::FLOAT_64, access().valueType());
        }  
        
        void SendReceiveTest::tearDown ( void )
        {
            delete m_factory;
            delete m_send;
            delete m_receive;
        }
    }
}
