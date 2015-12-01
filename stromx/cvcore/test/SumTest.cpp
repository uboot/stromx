#include "stromx/cvcore/test/SumTest.h"

#include <boost/lexical_cast.hpp>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/ReadAccess.h>
#include "stromx/cvsupport/Image.h"
#include "stromx/cvcore/Sum.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::cvcore::SumTest);

namespace stromx
{
    namespace cvcore
    {
        void SumTest::setUp()
        {
            m_operator = new stromx::runtime::OperatorTester(new Sum);
        }
        
        void SumTest::tearDown()
        {
            delete m_operator;
        }
        
        void SumTest::testAllocate0()
        {
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg", cvsupport::Image::GRAYSCALE));
            
            m_operator->setInputData(Sum::SRC, src);
            
            runtime::DataContainer valueResult = m_operator->getOutputData(Sum::VALUE);
            
            runtime::ReadAccess valueAccess(valueResult);
            std::ofstream valueFile;
            valueFile.open("SumTest_testAllocate0_value.txt");
            valueFile << valueAccess.get<runtime::Float64>();
            valueFile.close();
        }
        
        void SumTest::testAllocate1()
        {
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Matrix("lenna_32f.npy"));
            
            m_operator->setInputData(Sum::SRC, src);
            
            runtime::DataContainer valueResult = m_operator->getOutputData(Sum::VALUE);
            
            runtime::ReadAccess valueAccess(valueResult);
            std::ofstream valueFile;
            valueFile.open("SumTest_testAllocate1_value.txt");
            valueFile << valueAccess.get<runtime::Float64>();
            valueFile.close();
        }
        
    } // cvcore
} // stromx

