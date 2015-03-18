#include "stromx/cvimgproc/test/WarpAffineTest.h"

#include <boost/lexical_cast.hpp>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/ReadAccess.h>
#include "stromx/cvsupport/Image.h"
#include "stromx/cvimgproc/WarpAffine.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::cvimgproc::WarpAffineTest);

namespace stromx
{
    namespace cvimgproc
    {
        void WarpAffineTest::setUp()
        {
            m_operator = new stromx::runtime::OperatorTester(new WarpAffine);
        }
        
        void WarpAffineTest::tearDown()
        {
            delete m_operator;
        }
        
        void WarpAffineTest::testManual0()
        {
            m_operator->setParameter(WarpAffine::DATA_FLOW, runtime::Enum(WarpAffine::MANUAL));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg", cvsupport::Image::GRAYSCALE));
            runtime::DataContainer dst(new cvsupport::Image(1000000));
            cvsupport::Matrix affineM("affine.npy");
            runtime::UInt32 dsizex(400);
            runtime::UInt32 dsizey(500);
            
            m_operator->setInputData(WarpAffine::SRC, src);
            m_operator->setInputData(WarpAffine::DST, dst);
            m_operator->setParameter(WarpAffine::AFFINE_M, affineM);
            m_operator->setParameter(WarpAffine::DSIZEX, dsizex);
            m_operator->setParameter(WarpAffine::DSIZEY, dsizey);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(WarpAffine::DST);
            
            runtime::ReadAccess<runtime::Image> dstAccess(dstResult);
            cvsupport::Image::save("WarpAffineTest_testManual0_dst.png", dstAccess());
        }
        
        void WarpAffineTest::testManual1()
        {
            m_operator->setParameter(WarpAffine::DATA_FLOW, runtime::Enum(WarpAffine::MANUAL));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg"));
            runtime::DataContainer dst(new cvsupport::Image(1000000));
            runtime::UInt32 dsizex(400);
            runtime::UInt32 dsizey(500);
            
            m_operator->setInputData(WarpAffine::SRC, src);
            m_operator->setInputData(WarpAffine::DST, dst);
            m_operator->setParameter(WarpAffine::DSIZEX, dsizex);
            m_operator->setParameter(WarpAffine::DSIZEY, dsizey);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(WarpAffine::DST);
            
            runtime::ReadAccess<runtime::Image> dstAccess(dstResult);
            cvsupport::Image::save("WarpAffineTest_testManual1_dst.png", dstAccess());
        }
        
        void WarpAffineTest::testAllocate0()
        {
            m_operator->setParameter(WarpAffine::DATA_FLOW, runtime::Enum(WarpAffine::ALLOCATE));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg"));
            cvsupport::Matrix affineM("affine.npy");
            runtime::UInt32 dsizex(400);
            runtime::UInt32 dsizey(500);
            
            m_operator->setInputData(WarpAffine::SRC, src);
            m_operator->setParameter(WarpAffine::AFFINE_M, affineM);
            m_operator->setParameter(WarpAffine::DSIZEX, dsizex);
            m_operator->setParameter(WarpAffine::DSIZEY, dsizey);
            
            runtime::DataContainer dstResult = m_operator->getOutputData(WarpAffine::DST);
            
            runtime::ReadAccess<runtime::Image> dstAccess(dstResult);
            cvsupport::Image::save("WarpAffineTest_testAllocate0_dst.png", dstAccess());
        }
        
    } // cvimgproc
} // stromx

