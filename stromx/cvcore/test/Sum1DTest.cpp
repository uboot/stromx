#include "stromx/cvcore/test/Sum1DTest.h"

#include <boost/lexical_cast.hpp>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/ReadAccess.h>
#include "stromx/cvsupport/Image.h"
#include "stromx/cvcore/Sum1D.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::cvcore::Sum1DTest);

namespace stromx
{
    namespace cvcore
    {
        void Sum1DTest::setUp()
        {
            m_operator = new stromx::runtime::OperatorTester(new Sum1D);
        }
        
        void Sum1DTest::tearDown()
        {
            delete m_operator;
        }
        
        void Sum1DTest::testAllocate0()
        {
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg", cvsupport::Image::GRAYSCALE));
            
            m_operator->setInputData(Sum1D::SRC, src);
            
            runtime::DataContainer valueResult = m_operator->getOutputData(Sum1D::VALUE);
            
            runtime::ReadAccess valueAccess(valueResult);
            std::ofstream valueFile;
            valueFile.open("Sum1DTest_testAllocate0_value.txt");
            valueFile << valueAccess.get<runtime::Float64>();
            valueFile.close();
        }
        
        void Sum1DTest::testAllocate1()
        {
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Matrix("lenna_32f.npy"));
            
            m_operator->setInputData(Sum1D::SRC, src);
            
            runtime::DataContainer valueResult = m_operator->getOutputData(Sum1D::VALUE);
            
            runtime::ReadAccess valueAccess(valueResult);
            std::ofstream valueFile;
            valueFile.open("Sum1DTest_testAllocate1_value.txt");
            valueFile << valueAccess.get<runtime::Float64>();
            valueFile.close();
        }
        
    } // cvcore
} // stromx

