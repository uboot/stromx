#include "stromx/cvimgproc/test/CornerEigenValsAndVecsTest.h"

#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/ReadAccess.h>
#include "stromx/cvsupport/Image.h"
#include "stromx/cvimgproc/CornerEigenValsAndVecs.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::cvimgproc::CornerEigenValsAndVecsTest);

namespace stromx
{
    namespace cvimgproc
    {
        void CornerEigenValsAndVecsTest::setUp()
        {
            m_operator = new stromx::runtime::OperatorTester(new CornerEigenValsAndVecs);
        }
        
        void CornerEigenValsAndVecsTest::tearDown()
        {
            delete m_operator;
        }
        
        void CornerEigenValsAndVecsTest::testManual0()
        {
            m_operator->setParameter(CornerEigenValsAndVecs::DATA_FLOW, runtime::Enum(CornerEigenValsAndVecs::MANUAL));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg", cvsupport::Image::GRAYSCALE));
            runtime::DataContainer dst(new cvsupport::Image(1000000));
            
            m_operator->setInputData(CornerEigenValsAndVecs::SRC, src);
            m_operator->setInputData(CornerEigenValsAndVecs::DST, dst);
            
            runtime::DataContainer result = m_operator->getOutputData(CornerEigenValsAndVecs::DST);
            
            runtime::ReadAccess<runtime::Image> access(result);
            cvsupport::Image::save("CornerEigenValsAndVecsTest_testManual0.png", access());
        }
        
        void CornerEigenValsAndVecsTest::testManual1()
        {
            m_operator->setParameter(CornerEigenValsAndVecs::DATA_FLOW, runtime::Enum(CornerEigenValsAndVecs::MANUAL));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg", cvsupport::Image::GRAYSCALE));
            
            m_operator->setInputData(CornerEigenValsAndVecs::SRC, src);
            m_operator->setInputData(CornerEigenValsAndVecs::DST, src);
            
            runtime::DataContainer result = m_operator->getOutputData(CornerEigenValsAndVecs::DST);
            
            runtime::ReadAccess<runtime::Image> access(result);
            cvsupport::Image::save("CornerEigenValsAndVecsTest_testManual1.png", access());
        }
        
        void CornerEigenValsAndVecsTest::testAllocate0()
        {
            m_operator->setParameter(CornerEigenValsAndVecs::DATA_FLOW, runtime::Enum(CornerEigenValsAndVecs::ALLOCATE));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg", cvsupport::Image::GRAYSCALE));
            
            m_operator->setInputData(CornerEigenValsAndVecs::SRC, src);
            
            runtime::DataContainer result = m_operator->getOutputData(CornerEigenValsAndVecs::DST);
            
            runtime::ReadAccess<runtime::Image> access(result);
            cvsupport::Image::save("CornerEigenValsAndVecsTest_testAllocate0.png", access());
        }
        
        void CornerEigenValsAndVecsTest::testInPlace0()
        {
            m_operator->setParameter(CornerEigenValsAndVecs::DATA_FLOW, runtime::Enum(CornerEigenValsAndVecs::IN_PLACE));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg", cvsupport::Image::GRAYSCALE));
            
            m_operator->setInputData(CornerEigenValsAndVecs::SRC, src);
            
            runtime::DataContainer result = m_operator->getOutputData(CornerEigenValsAndVecs::SRC);
            
            runtime::ReadAccess<runtime::Image> access(result);
            cvsupport::Image::save("CornerEigenValsAndVecsTest_testInPlace0.png", access());
        }
        
    }
}

