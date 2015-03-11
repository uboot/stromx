#include "stromx/cvcore/test/AddTest.h"

#include <boost/lexical_cast.hpp>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/ReadAccess.h>
#include "stromx/cvsupport/Image.h"
#include "stromx/cvcore/Add.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::cvcore::AddTest);

namespace stromx
{
    namespace cvcore
    {
        void AddTest::setUp()
        {
            m_operator = new stromx::runtime::OperatorTester(new Add);
        }
        
        void AddTest::tearDown()
        {
            delete m_operator;
        }
        
        void AddTest::testManual0()
        {
            m_operator->setParameter(Add::DATA_FLOW, runtime::Enum(Add::MANUAL));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src1(new cvsupport::Image("lenna.jpg"));
            runtime::DataContainer src2(new cvsupport::Image("barbara.jpg"));
            runtime::DataContainer dst(new cvsupport::Image(5000000));
            
            m_operator->setInputData(Add::SRC_1, src1);
            m_operator->setInputData(Add::SRC_2, src2);
            m_operator->setInputData(Add::DST, dst);
            
            runtime::DataContainer result = m_operator->getOutputData(Add::DST);
            
            runtime::ReadAccess<runtime::Image> access(result);
            cvsupport::Image::save("AddTest_testManual0.png", access());
        }
        
        void AddTest::testManual1()
        {
            m_operator->setParameter(Add::DATA_FLOW, runtime::Enum(Add::MANUAL));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src1(new cvsupport::Image("lenna.jpg", cvsupport::Image::GRAYSCALE));
            runtime::DataContainer src2(new cvsupport::Image("barbara.jpg", cvsupport::Image::GRAYSCALE));
            runtime::DataContainer dst(new cvsupport::Image(5000000));
            
            m_operator->setInputData(Add::SRC_1, src1);
            m_operator->setInputData(Add::SRC_2, src2);
            m_operator->setInputData(Add::DST, dst);
            
            runtime::DataContainer result = m_operator->getOutputData(Add::DST);
            
            runtime::ReadAccess<runtime::Image> access(result);
            cvsupport::Image::save("AddTest_testManual1.png", access());
        }
        
        void AddTest::testManual2()
        {
            m_operator->setParameter(Add::DATA_FLOW, runtime::Enum(Add::MANUAL));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src1(new cvsupport::Image("lenna.jpg", cvsupport::Image::DEPTH_16));
            runtime::DataContainer src2(new cvsupport::Image("barbara.jpg"));
            runtime::DataContainer dst(new cvsupport::Image(5000000));
            runtime::Enum ddepth(1);
            
            m_operator->setInputData(Add::SRC_1, src1);
            m_operator->setInputData(Add::SRC_2, src2);
            m_operator->setInputData(Add::DST, dst);
            m_operator->setParameter(Add::DDEPTH, ddepth);
            
            runtime::DataContainer result = m_operator->getOutputData(Add::DST);
            
            runtime::ReadAccess<runtime::Image> access(result);
            cvsupport::Image::save("AddTest_testManual2.png", access());
        }
        
        void AddTest::testAllocate0()
        {
            m_operator->setParameter(Add::DATA_FLOW, runtime::Enum(Add::ALLOCATE));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src1(new cvsupport::Image("lenna.jpg", cvsupport::Image::DEPTH_16));
            runtime::DataContainer src2(new cvsupport::Image("barbara.jpg", cvsupport::Image::DEPTH_16));
            
            m_operator->setInputData(Add::SRC_1, src1);
            m_operator->setInputData(Add::SRC_2, src2);
            
            runtime::DataContainer result = m_operator->getOutputData(Add::DST);
            
            runtime::ReadAccess<runtime::Image> access(result);
            cvsupport::Image::save("AddTest_testAllocate0.png", access());
        }
        
        void AddTest::testAllocate1()
        {
            m_operator->setParameter(Add::DATA_FLOW, runtime::Enum(Add::ALLOCATE));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src1(new cvsupport::Image("lenna.jpg", cvsupport::Image::DEPTH_16));
            runtime::DataContainer src2(new cvsupport::Image("barbara.jpg"));
            runtime::Enum ddepth(2);
            
            m_operator->setInputData(Add::SRC_1, src1);
            m_operator->setInputData(Add::SRC_2, src2);
            m_operator->setParameter(Add::DDEPTH, ddepth);
            
            runtime::DataContainer result = m_operator->getOutputData(Add::DST);
            
            runtime::ReadAccess<runtime::Image> access(result);
            cvsupport::Image::save("AddTest_testAllocate1.png", access());
        }
        
    } // cvcore
} // stromx

