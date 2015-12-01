#include "stromx/cvimgproc/test/ContourAreaTest.h"

#include <boost/lexical_cast.hpp>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/ReadAccess.h>
#include "stromx/cvsupport/Image.h"
#include "stromx/cvimgproc/ContourArea.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::cvimgproc::ContourAreaTest);

namespace stromx
{
    namespace cvimgproc
    {
        void ContourAreaTest::setUp()
        {
            m_operator = new stromx::runtime::OperatorTester(new ContourArea);
        }
        
        void ContourAreaTest::tearDown()
        {
            delete m_operator;
        }
        
        void ContourAreaTest::testAllocate0()
        {
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer contour(new cvsupport::Matrix("non_convex_f32.npy"));
            
            m_operator->setInputData(ContourArea::CONTOUR, contour);
            
            runtime::DataContainer areaResult = m_operator->getOutputData(ContourArea::AREA);
            
            runtime::ReadAccess areaAccess(areaResult);
            std::ofstream areaFile;
            areaFile.open("ContourAreaTest_testAllocate0_area.txt");
            areaFile << areaAccess.get<runtime::Float64>();
            areaFile.close();
        }
        
        void ContourAreaTest::testAllocate1()
        {
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer contour(new cvsupport::Matrix("points_i32.npy"));
            
            m_operator->setInputData(ContourArea::CONTOUR, contour);
            
            runtime::DataContainer areaResult = m_operator->getOutputData(ContourArea::AREA);
            
            runtime::ReadAccess areaAccess(areaResult);
            std::ofstream areaFile;
            areaFile.open("ContourAreaTest_testAllocate1_area.txt");
            areaFile << areaAccess.get<runtime::Float64>();
            areaFile.close();
        }
        
    } // cvimgproc
} // stromx

