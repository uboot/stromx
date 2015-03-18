#include "stromx/cvcalib3d/test/GeneratePatternTest.h"

#include <boost/lexical_cast.hpp>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/ReadAccess.h>
#include "stromx/cvsupport/Image.h"
#include "stromx/cvcalib3d/GeneratePattern.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::cvcalib3d::GeneratePatternTest);

namespace stromx
{
    namespace cvcalib3d
    {
        void GeneratePatternTest::setUp()
        {
            m_operator = new stromx::runtime::OperatorTester(new GeneratePattern);
        }
        
        void GeneratePatternTest::tearDown()
        {
            delete m_operator;
        }
        
        void GeneratePatternTest::testAllocate0()
        {
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer cornersResult = m_operator->getOutputData(GeneratePattern::CORNERS);
            
            runtime::ReadAccess<runtime::Matrix> cornersAccess(cornersResult);
            cvsupport::Matrix::save("GeneratePatternTest_testAllocate0_corners.npy", cornersAccess());
        }
        
    } // cvcalib3d
} // stromx

