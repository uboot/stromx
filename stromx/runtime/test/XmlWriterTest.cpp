/* 
 *  Copyright 2011 Thomas Fidler
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
#include "stromx/runtime/Exception.h"
#include "stromx/runtime/Stream.h"
#include "stromx/runtime/XmlWriter.h"
#include "stromx/runtime/test/TestUtilities.h"
#include "stromx/runtime/test/TestOperator.h"
#include "stromx/runtime/test/XmlWriterTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::runtime::XmlWriterTest);

namespace stromx
{
    namespace runtime
    {
        void XmlWriterTest::setUp()
        {
            m_stream = TestUtilities::buildTestStream();         
        }
        
        void XmlWriterTest::testWriteStream()
        {
            //Attempt to write to filesystem where access is guaranteed (hopefully: it is written to the subdirectory where
            //the test was initiated
            XmlWriter().writeStream("XmlWriterTest_testWriteStream.xml", *m_stream);
        }
        
        void XmlWriterTest::testWriteStreamZip()
        {
            CPPUNIT_ASSERT_NO_THROW(XmlWriter().writeStream("XmlWriterTest_testWriteStreamZip.zip", *m_stream));
        }
        
        void XmlWriterTest::testWriteStreamStromx()
        {
            CPPUNIT_ASSERT_NO_THROW(XmlWriter().writeStream("XmlWriterTest_testWriteStreamZip.stromx", *m_stream));
        }
        
        void XmlWriterTest::testWriteParameters()
        {
            std::vector<const Operator*> operators(m_stream->operators().begin(), m_stream->operators().end());
                 
            XmlWriter().writeParameters("XmlWriterTest_testWriteParameters.xml", operators);
        }
        
        void XmlWriterTest::testWriteParametersZip()
        {
            std::vector<const Operator*> operators(m_stream->operators().begin(), m_stream->operators().end());
                 
            CPPUNIT_ASSERT_NO_THROW(XmlWriter().writeParameters("XmlWriterTest_testWriteParametersZip.zip", operators));
        }
        
        void XmlWriterTest::testWriteParametersStromx()
        {
            std::vector<const Operator*> operators(m_stream->operators().begin(), m_stream->operators().end());
                 
            CPPUNIT_ASSERT_NO_THROW(XmlWriter().writeParameters("XmlWriterTest_testWriteParametersZip.stromx", operators));
        }
        
        void XmlWriterTest::testWriteNoAccess()
        {
            //Attempt to write to filesystem without neccessary permission
            CPPUNIT_ASSERT_THROW(XmlWriter().writeStream("/root/test/XmlWriterTest_testWriteNoAccess.stromx", *m_stream), FileAccessFailed);
        }
        
        void XmlWriterTest::testWriteStreamPullParameter()
        {
            Operator* op0 = m_stream->operators()[0];
            m_stream->setConnectorType(op0, TestOperator::OUTPUT_1, Description::PARAMETER,
                                       Description::PULL);
            XmlWriter().writeStream("XmlWriterTest_testWriteStreamPullParameter.xml", *m_stream);
        }
        
        void XmlWriterTest::testWriteStreamPushParameter()
        {
            Operator* op1 = m_stream->operators()[1];
            m_stream->setConnectorType(op1, TestOperator::INPUT_1, Description::PARAMETER,
                                       Description::PUSH);
            XmlWriter().writeStream("XmlWriterTest_testWriteStreamPushParameter.xml", *m_stream);
        }

        void XmlWriterTest::tearDown()
        {
            delete m_stream;
        }
    }
}
