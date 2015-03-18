#include "stromx/cvimgproc/test/GoodFeaturesToTrackTest.h"

#include <boost/lexical_cast.hpp>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/ReadAccess.h>
#include "stromx/cvsupport/Image.h"
#include "stromx/cvimgproc/GoodFeaturesToTrack.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::cvimgproc::GoodFeaturesToTrackTest);

namespace stromx
{
    namespace cvimgproc
    {
        void GoodFeaturesToTrackTest::setUp()
        {
            m_operator = new stromx::runtime::OperatorTester(new GoodFeaturesToTrack);
        }
        
        void GoodFeaturesToTrackTest::tearDown()
        {
            delete m_operator;
        }
        
        void GoodFeaturesToTrackTest::testAllocate0()
        {
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("corners.png", cvsupport::Image::GRAYSCALE));
            
            m_operator->setInputData(GoodFeaturesToTrack::SRC, src);
            
            runtime::DataContainer pointMatrixResult = m_operator->getOutputData(GoodFeaturesToTrack::POINT_MATRIX);
            
            runtime::ReadAccess<runtime::Matrix> pointMatrixAccess(pointMatrixResult);
            cvsupport::Matrix::save("GoodFeaturesToTrackTest_testAllocate0_pointMatrix.npy", pointMatrixAccess());
        }
        
    } // cvimgproc
} // stromx

