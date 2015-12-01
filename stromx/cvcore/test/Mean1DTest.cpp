#include "stromx/cvcore/test/Mean1DTest.h"

#include <boost/lexical_cast.hpp>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/ReadAccess.h>
#include "stromx/cvsupport/Image.h"
#include "stromx/cvcore/Mean1D.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::cvcore::Mean1DTest);

namespace stromx
{
    namespace cvcore
    {
        void Mean1DTest::setUp()
        {
            m_operator = new stromx::runtime::OperatorTester(new Mean1D);
        }
        
        void Mean1DTest::tearDown()
        {
            delete m_operator;
        }
        
        void Mean1DTest::testAllocate0()
        {
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg", cvsupport::Image::GRAYSCALE));
            
            m_operator->setInputData(Mean1D::SRC, src);
            
            runtime::DataContainer valueResult = m_operator->getOutputData(Mean1D::VALUE);
            
            runtime::ReadAccess valueAccess(valueResult);
            std::ofstream valueFile;
            valueFile.open("Mean1DTest_testAllocate0_value.txt");
            valueFile << valueAccess.get<runtime::Float64>();
            valueFile.close();
        }
        
        void Mean1DTest::testAllocate1()
        {
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Matrix("lenna_32f.npy"));
            
            m_operator->setInputData(Mean1D::SRC, src);
            
            runtime::DataContainer valueResult = m_operator->getOutputData(Mean1D::VALUE);
            
            runtime::ReadAccess valueAccess(valueResult);
            std::ofstream valueFile;
            valueFile.open("Mean1DTest_testAllocate1_value.txt");
            valueFile << valueAccess.get<runtime::Float64>();
            valueFile.close();
        }
        
    } // cvcore
} // stromx

