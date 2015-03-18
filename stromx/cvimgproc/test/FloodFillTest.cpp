#include "stromx/cvimgproc/test/FloodFillTest.h"

#include <boost/lexical_cast.hpp>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/ReadAccess.h>
#include "stromx/cvsupport/Image.h"
#include "stromx/cvimgproc/FloodFill.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::cvimgproc::FloodFillTest);

namespace stromx
{
    namespace cvimgproc
    {
        void FloodFillTest::setUp()
        {
            m_operator = new stromx::runtime::OperatorTester(new FloodFill);
        }
        
        void FloodFillTest::tearDown()
        {
            delete m_operator;
        }
        
        void FloodFillTest::testInPlace0()
        {
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("circle.png", cvsupport::Image::GRAYSCALE));
            runtime::UInt32 seedPointX(20);
            runtime::UInt32 seedPointY(10);
            runtime::Float64 newVal(125.0);
            
            m_operator->setInputData(FloodFill::SRC, src);
            m_operator->setParameter(FloodFill::SEED_POINT_X, seedPointX);
            m_operator->setParameter(FloodFill::SEED_POINT_Y, seedPointY);
            m_operator->setParameter(FloodFill::NEW_VAL, newVal);
            
            runtime::DataContainer srcResult = m_operator->getOutputData(FloodFill::SRC);
            
            runtime::ReadAccess<runtime::Image> srcAccess(srcResult);
            cvsupport::Image::save("FloodFillTest_testInPlace0_src.png", srcAccess());
        }
        
    } // cvimgproc
} // stromx

