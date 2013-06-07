#include "stromx/imgproc/test/PyrDownTest.h"

#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/ReadAccess.h>
#include "stromx/imgutil/Image.h"
#include "stromx/imgproc/PyrDown.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::imgproc::PyrDownTest);

namespace stromx
{
    namespace imgproc
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
            
            runtime::DataContainer src(new imgutil::Image("lenna.jpg"));
            runtime::DataContainer dst(new imgutil::Image(1000000));
            
            m_operator->setInputData(PyrDown::SRC, src);
            m_operator->setInputData(PyrDown::DST, dst);
            
            runtime::DataContainer result = m_operator->getOutputData(PyrDown::DST);
            
            runtime::ReadAccess<runtime::Image> access(result);
            imgutil::Image::save("PyrDownTest_testManual0.png", access());
        }
        
        void PyrDownTest::testAllocate0()
        {
            m_operator->setParameter(PyrDown::DATA_FLOW, runtime::Enum(PyrDown::ALLOCATE));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new imgutil::Image("lenna.jpg", imgutil::Image::GRAYSCALE));
            
            m_operator->setInputData(PyrDown::SRC, src);
            
            runtime::DataContainer result = m_operator->getOutputData(PyrDown::DST);
            
            runtime::ReadAccess<runtime::Image> access(result);
            imgutil::Image::save("PyrDownTest_testAllocate0.png", access());
        }
        
    }
}

