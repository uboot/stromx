#include "stromx/cvimgproc/test/PyrUpTest.h"

#include <boost/lexical_cast.hpp>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/ReadAccess.h>
#include "stromx/cvsupport/Image.h"
#include "stromx/cvimgproc/PyrUp.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::cvimgproc::PyrUpTest);

namespace stromx
{
    namespace cvimgproc
    {
        void PyrUpTest::setUp()
        {
            m_operator = new stromx::runtime::OperatorTester(new PyrUp);
        }
        
        void PyrUpTest::tearDown()
        {
            delete m_operator;
        }
        
        void PyrUpTest::testManual0()
        {
            m_operator->setParameter(PyrUp::DATA_FLOW, runtime::Enum(PyrUp::MANUAL));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg"));
            runtime::DataContainer dst(new cvsupport::Image(10000000));
            
            m_operator->setInputData(PyrUp::SRC, src);
            m_operator->setInputData(PyrUp::DST, dst);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(PyrUp::DST);
            
            runtime::ReadAccess<runtime::Image> dstAccess(dstResult);
            cvsupport::Image::save("PyrUpTest_testManual0_dst.png", dstAccess());
        }
        
        void PyrUpTest::testAllocate0()
        {
            m_operator->setParameter(PyrUp::DATA_FLOW, runtime::Enum(PyrUp::ALLOCATE));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg", cvsupport::Image::GRAYSCALE));
            
            m_operator->setInputData(PyrUp::SRC, src);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(PyrUp::DST);
            
            runtime::ReadAccess<runtime::Image> dstAccess(dstResult);
            cvsupport::Image::save("PyrUpTest_testAllocate0_dst.png", dstAccess());
        }
        
    } // cvimgproc
} // stromx

