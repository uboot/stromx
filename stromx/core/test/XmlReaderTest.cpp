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
#include "TestData.h"
#include "TestOperator.h"
#include "TestUtilities.h"
#include "XmlReaderTest.h"
#include "../Factory.h"
#include "../Operator.h"
#include "../Stream.h"
#include "../XmlReader.h"
#include "../XmlWriter.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::core::XmlReaderTest);

namespace stromx
{
    namespace core
    {
        void XmlReaderTest::setUp()
        {
            m_factory = new Factory;
            m_factory->registerOperator(new TestOperator());
            m_factory->registerData(new UInt32());
            m_factory->registerData(new Bool());
            m_factory->registerData(new TestData);
            
            m_stream = TestUtilities::buildTestStream();
        }
        
        void XmlReaderTest::testReadStream()
        {
            Stream* stream = 0;
            CPPUNIT_ASSERT_NO_THROW(stream = XmlReader().readStream("stream.xml", *m_factory));
            
            XmlWriter writer;
            writer.writeStream("XmlReaderTest_testReadStream.xml", *stream);
            
            delete stream;
        }
        
        void XmlReaderTest::testReadStreamZip()
        {
            Stream* stream = 0;
            CPPUNIT_ASSERT_NO_THROW(stream = XmlReader().readStream("stream.zip", *m_factory));
            
            XmlWriter writer;
            writer.writeStream("XmlReaderTest_testReadStreamZip.xml", *stream);
            
            delete stream;
        }
        
        void XmlReaderTest::testReadStreamWrongFile()
        {
            CPPUNIT_ASSERT_THROW(XmlReader().readStream("wrongfile.xml", *m_factory), FileAccessFailed);
        }
        
        void XmlReaderTest::testReadStreamInvalidFile()
        {
            CPPUNIT_ASSERT_THROW(XmlReader().readStream("invalid.xml", *m_factory), InvalidFileFormat);
        }
        
        void XmlReaderTest::testReadStreamEmpty()
        {
            CPPUNIT_ASSERT_THROW(XmlReader().readStream("empty.xml", *m_factory), FileAccessFailed);
        }
        
        void XmlReaderTest::testReadParameters()
        {
            CPPUNIT_ASSERT_NO_THROW(XmlReader().readParameters("parameters.xml", *m_factory, m_stream->operators()));
            
            CPPUNIT_ASSERT_EQUAL(UInt32(7000), dynamic_cast<const UInt32&>(m_stream->operators()[2]->getParameter(0)));
            CPPUNIT_ASSERT_EQUAL(UInt32(200), dynamic_cast<const UInt32&>(m_stream->operators()[2]->getParameter(1)));
            
            XmlWriter().writeStream("XmlReaderTest_testReadParameters.xml", *m_stream);
        }
        
        void XmlReaderTest::testReadParametersZip()
        {
            CPPUNIT_ASSERT_NO_THROW(XmlReader().readParameters("parameters.zip", *m_factory, m_stream->operators()));
            
            CPPUNIT_ASSERT_EQUAL(UInt32(7000), dynamic_cast<const UInt32&>(m_stream->operators()[2]->getParameter(0)));
            CPPUNIT_ASSERT_EQUAL(UInt32(200), dynamic_cast<const UInt32&>(m_stream->operators()[2]->getParameter(1)));
            
            XmlWriter().writeStream("XmlReaderTest_testReadParametersZip.xml", *m_stream);
        }
        
        void XmlReaderTest::testReadParametersEmpty()
        {
            CPPUNIT_ASSERT_THROW(XmlReader().readParameters("empty.xml", *m_factory, m_stream->operators()),
                                 FileAccessFailed);
        }

        void XmlReaderTest::testReadParametersInvalidFile()
        {
            CPPUNIT_ASSERT_THROW(XmlReader().readParameters("invalid.xml", *m_factory, m_stream->operators()),
                                 InvalidFileFormat);

        }

        void XmlReaderTest::testReadParametersWrongFile()
        {
            CPPUNIT_ASSERT_THROW(XmlReader().readParameters("wrongfile.xml", *m_factory, m_stream->operators()),
                                 FileAccessFailed);
        }

        void XmlReaderTest::tearDown()
        {
            delete m_factory;
            delete m_stream;
        }
    }
}
