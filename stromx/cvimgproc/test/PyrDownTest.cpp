#include "stromx/cvimgproc/test/PyrDownTest.h"

#include <boost/lexical_cast.hpp>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/ReadAccess.h>
#include "stromx/cvsupport/Image.h"
#include "stromx/cvimgproc/PyrDown.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::cvimgproc::PyrDownTest);

namespace stromx
{
    namespace cvimgproc
    {
        void PyrDownTest::setUp()
        {
            m_operator = new stromx::runtime::OperatorTester(new PyrDown);
        }
        
        void PyrDownTest::tearDown()
        {
            delete m_operator;
        }
        
        void PyrDownTest::testManual0()
        {
            m_operator->setParameter(PyrDown::DATA_FLOW, runtime::Enum(PyrDown::MANUAL));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg"));
            runtime::DataContainer dst(new cvsupport::Image(1000000));
            
            m_operator->setInputData(PyrDown::SRC, src);
            m_operator->setInputData(PyrDown::DST, dst);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(PyrDown::DST);
            
            runtime::ReadAccess<runtime::Image> dstAccess(dstResult);
            cvsupport::Image::save("PyrDownTest_testManual0_dst.png", dstAccess());
        }
        
        void PyrDownTest::testAllocate0()
        {
            m_operator->setParameter(PyrDown::DATA_FLOW, runtime::Enum(PyrDown::ALLOCATE));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg", cvsupport::Image::GRAYSCALE));
            
            m_operator->setInputData(PyrDown::SRC, src);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(PyrDown::DST);
            
            runtime::ReadAccess<runtime::Image> dstAccess(dstResult);
            cvsupport::Image::save("PyrDownTest_testAllocate0_dst.png", dstAccess());
        }
        
    } // cvimgproc
} // stromx

