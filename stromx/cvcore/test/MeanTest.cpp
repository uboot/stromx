#include "stromx/cvcore/test/MeanTest.h"

#include <boost/lexical_cast.hpp>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/ReadAccess.h>
#include "stromx/cvsupport/Image.h"
#include "stromx/cvcore/Mean.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::cvcore::MeanTest);

namespace stromx
{
    namespace cvcore
    {
        void MeanTest::setUp()
        {
            m_operator = new stromx::runtime::OperatorTester(new Mean);
        }
        
        void MeanTest::tearDown()
        {
            delete m_operator;
        }
        
        void MeanTest::testAllocate0()
        {
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg", cvsupport::Image::GRAYSCALE));
            
            m_operator->setInputData(Mean::SRC, src);
            
            runtime::DataContainer valueResult = m_operator->getOutputData(Mean::VALUE);
            
            runtime::ReadAccess valueAccess(valueResult);
            std::ofstream valueFile;
            valueFile.open("MeanTest_testAllocate0_value.txt");
            valueFile << valueAccess.get<runtime::Float64>();
            valueFile.close();
        }
        
        void MeanTest::testAllocate1()
        {
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Matrix("lenna_32f.npy"));
            
            m_operator->setInputData(Mean::SRC, src);
            
            runtime::DataContainer valueResult = m_operator->getOutputData(Mean::VALUE);
            
            runtime::ReadAccess valueAccess(valueResult);
            std::ofstream valueFile;
            valueFile.open("MeanTest_testAllocate1_value.txt");
            valueFile << valueAccess.get<runtime::Float64>();
            valueFile.close();
        }
        
    } // cvcore
} // stromx

