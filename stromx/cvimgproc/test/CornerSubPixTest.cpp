#include "stromx/cvimgproc/test/CornerSubPixTest.h"

#include <boost/lexical_cast.hpp>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/ReadAccess.h>
#include "stromx/cvsupport/Image.h"
#include "stromx/cvimgproc/CornerSubPix.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::cvimgproc::CornerSubPixTest);

namespace stromx
{
    namespace cvimgproc
    {
        void CornerSubPixTest::setUp()
        {
            m_operator = new stromx::runtime::OperatorTester(new CornerSubPix);
        }
        
        void CornerSubPixTest::tearDown()
        {
            delete m_operator;
        }
        
        void CornerSubPixTest::testInPlace0()
        {
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("corners.png", cvsupport::Image::GRAYSCALE));
            runtime::DataContainer pointMatrix(new cvsupport::Matrix("corners.npy"));
            
            m_operator->setInputData(CornerSubPix::SRC, src);
            m_operator->setInputData(CornerSubPix::POINT_MATRIX, pointMatrix);
            
            runtime::DataContainer pointMatrixResult = m_operator->getOutputData(CornerSubPix::POINT_MATRIX);
            
            runtime::ReadAccess<runtime::Matrix> pointMatrixAccess(pointMatrixResult);
            cvsupport::Matrix::save("CornerSubPixTest_testInPlace0_pointMatrix.npy", pointMatrixAccess());
        }
        
    } // cvimgproc
} // stromx

