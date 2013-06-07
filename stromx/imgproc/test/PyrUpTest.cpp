#include "stromx/imgproc/test/PyrUpTest.h"

#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/ReadAccess.h>
#include "stromx/imgutil/Image.h"
#include "stromx/imgproc/PyrUp.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::imgproc::PyrUpTest);

namespace stromx
{
    namespace imgproc
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
            
            runtime::DataContainer src(new imgutil::Image("lenna.jpg"));
            runtime::DataContainer dst(new imgutil::Image(10000000));
            
            m_operator->setInputData(PyrUp::SRC, src);
            m_operator->setInputData(PyrUp::DST, dst);
            
            runtime::DataContainer result = m_operator->getOutputData(PyrUp::DST);
            
            runtime::ReadAccess<runtime::Image> access(result);
            imgutil::Image::save("PyrUpTest_testManual0.png", access());
        }
        
        void PyrUpTest::testAllocate0()
        {
            m_operator->setParameter(PyrUp::DATA_FLOW, runtime::Enum(PyrUp::ALLOCATE));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new imgutil::Image("lenna.jpg", imgutil::Image::GRAYSCALE));
            
            m_operator->setInputData(PyrUp::SRC, src);
            
            runtime::DataContainer result = m_operator->getOutputData(PyrUp::DST);
            
            runtime::ReadAccess<runtime::Image> access(result);
            imgutil::Image::save("PyrUpTest_testAllocate0.png", access());
        }
        
    }
}

