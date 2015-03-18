#include "stromx/cvcalib3d/test/FindChessboardCornersTest.h"

#include <boost/lexical_cast.hpp>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/ReadAccess.h>
#include "stromx/cvsupport/Image.h"
#include "stromx/cvcalib3d/FindChessboardCorners.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::cvcalib3d::FindChessboardCornersTest);

namespace stromx
{
    namespace cvcalib3d
    {
        void FindChessboardCornersTest::setUp()
        {
            m_operator = new stromx::runtime::OperatorTester(new FindChessboardCorners);
        }
        
        void FindChessboardCornersTest::tearDown()
        {
            delete m_operator;
        }
        
        void FindChessboardCornersTest::testAllocate0()
        {
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer image(new cvsupport::Image("chess.png", cvsupport::Image::GRAYSCALE));
            
            m_operator->setInputData(FindChessboardCorners::IMAGE, image);
            
            runtime::DataContainer cornersResult = m_operator->getOutputData(FindChessboardCorners::CORNERS);
            
            runtime::ReadAccess<runtime::Matrix> cornersAccess(cornersResult);
            cvsupport::Matrix::save("FindChessboardCornersTest_testAllocate0_corners.npy", cornersAccess());
        }
        
        void FindChessboardCornersTest::testAllocate1()
        {
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer image(new cvsupport::Image("chess.png"));
            
            m_operator->setInputData(FindChessboardCorners::IMAGE, image);
            
            runtime::DataContainer cornersResult = m_operator->getOutputData(FindChessboardCorners::CORNERS);
            
            runtime::ReadAccess<runtime::Matrix> cornersAccess(cornersResult);
            cvsupport::Matrix::save("FindChessboardCornersTest_testAllocate1_corners.npy", cornersAccess());
        }
        
    } // cvcalib3d
} // stromx

