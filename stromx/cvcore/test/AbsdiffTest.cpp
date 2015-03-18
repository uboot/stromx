#include "stromx/cvcore/test/AbsdiffTest.h"

#include <boost/lexical_cast.hpp>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/ReadAccess.h>
#include "stromx/cvsupport/Image.h"
#include "stromx/cvcore/Absdiff.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::cvcore::AbsdiffTest);

namespace stromx
{
    namespace cvcore
    {
        void AbsdiffTest::setUp()
        {
            m_operator = new stromx::runtime::OperatorTester(new Absdiff);
        }
        
        void AbsdiffTest::tearDown()
        {
            delete m_operator;
        }
        
        void AbsdiffTest::testManual0()
        {
            m_operator->setParameter(Absdiff::DATA_FLOW, runtime::Enum(Absdiff::MANUAL));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src1(new cvsupport::Image("lenna.jpg"));
            runtime::DataContainer src2(new cvsupport::Image("barbara.jpg"));
            runtime::DataContainer dst(new cvsupport::Image(5000000));
            
            m_operator->setInputData(Absdiff::SRC_1, src1);
            m_operator->setInputData(Absdiff::SRC_2, src2);
            m_operator->setInputData(Absdiff::DST, dst);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(Absdiff::DST);
            
            runtime::ReadAccess<runtime::Image> dstAccess(dstResult);
            cvsupport::Image::save("AbsdiffTest_testManual0_dst.png", dstAccess());
        }
        
        void AbsdiffTest::testManual1()
        {
            m_operator->setParameter(Absdiff::DATA_FLOW, runtime::Enum(Absdiff::MANUAL));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src1(new cvsupport::Image("lenna.jpg", cvsupport::Image::GRAYSCALE));
            runtime::DataContainer src2(new cvsupport::Image("barbara.jpg", cvsupport::Image::GRAYSCALE));
            runtime::DataContainer dst(new cvsupport::Image(5000000));
            
            m_operator->setInputData(Absdiff::SRC_1, src1);
            m_operator->setInputData(Absdiff::SRC_2, src2);
            m_operator->setInputData(Absdiff::DST, dst);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(Absdiff::DST);
            
            runtime::ReadAccess<runtime::Image> dstAccess(dstResult);
            cvsupport::Image::save("AbsdiffTest_testManual1_dst.png", dstAccess());
        }
        
        void AbsdiffTest::testAllocate0()
        {
            m_operator->setParameter(Absdiff::DATA_FLOW, runtime::Enum(Absdiff::ALLOCATE));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src1(new cvsupport::Image("lenna.jpg", cvsupport::Image::DEPTH_16));
            runtime::DataContainer src2(new cvsupport::Image("barbara.jpg", cvsupport::Image::DEPTH_16));
            
            m_operator->setInputData(Absdiff::SRC_1, src1);
            m_operator->setInputData(Absdiff::SRC_2, src2);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(Absdiff::DST);
            
            runtime::ReadAccess<runtime::Image> dstAccess(dstResult);
            cvsupport::Image::save("AbsdiffTest_testAllocate0_dst.png", dstAccess());
        }
        
    } // cvcore
} // stromx

