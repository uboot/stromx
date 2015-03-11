#include "stromx/cvimgproc/test/FitLineTest.h"

#include <boost/lexical_cast.hpp>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/ReadAccess.h>
#include "stromx/cvsupport/Image.h"
#include "stromx/cvimgproc/FitLine.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::cvimgproc::FitLineTest);

namespace stromx
{
    namespace cvimgproc
    {
        void FitLineTest::setUp()
        {
            m_operator = new stromx::runtime::OperatorTester(new FitLine);
        }
        
        void FitLineTest::tearDown()
        {
            delete m_operator;
        }
        
        void FitLineTest::testAllocate0()
        {
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer points(new cvsupport::Matrix("points_i32.npy"));
            
            m_operator->setInputData(FitLine::POINTS, points);
            
            runtime::DataContainer result = m_operator->getOutputData(FitLine::LINE);
            
            runtime::ReadAccess<runtime::Matrix> access(result);
            cvsupport::Matrix::save("FitLineTest_testAllocate0.npy", access());
        }
        
        void FitLineTest::testAllocate1()
        {
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer points(new cvsupport::Matrix("points_f32.npy"));
            
            m_operator->setInputData(FitLine::POINTS, points);
            
            runtime::DataContainer result = m_operator->getOutputData(FitLine::LINE);
            
            runtime::ReadAccess<runtime::Matrix> access(result);
            cvsupport::Matrix::save("FitLineTest_testAllocate1.npy", access());
        }
        
    } // cvimgproc
} // stromx

