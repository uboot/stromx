#include "stromx/cvimgproc/test/DrawContoursTest.h"

#include <boost/lexical_cast.hpp>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/ReadAccess.h>
#include "stromx/cvsupport/Image.h"
#include "stromx/cvimgproc/DrawContours.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::cvimgproc::DrawContoursTest);

namespace stromx
{
    namespace cvimgproc
    {
        void DrawContoursTest::setUp()
        {
            m_operator = new stromx::runtime::OperatorTester(new DrawContours);
        }
        
        void DrawContoursTest::tearDown()
        {
            delete m_operator;
        }
        
        void DrawContoursTest::testInPlace0()
        {
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer img(new cvsupport::Image("lenna.jpg", cvsupport::Image::GRAYSCALE));
            runtime::Data* dataObject = 0;
            std::vector<runtime::Data*> contoursVector;
            dataObject = new cvsupport::Matrix("contour_1.npy");
            contoursVector.push_back(dataObject);
            dataObject = new cvsupport::Matrix("contour_2.npy");
            contoursVector.push_back(dataObject);
            runtime::DataContainer contours(new runtime::List(contoursVector));
            runtime::UInt8 ch1(255);
            runtime::UInt8 ch2(0);
            runtime::UInt8 ch3(0);
            
            m_operator->setInputData(DrawContours::IMG, img);
            m_operator->setInputData(DrawContours::CONTOURS, contours);
            m_operator->setParameter(DrawContours::CH_1, ch1);
            m_operator->setParameter(DrawContours::CH_2, ch2);
            m_operator->setParameter(DrawContours::CH_3, ch3);
            
            runtime::DataContainer result = m_operator->getOutputData(DrawContours::IMG);
            
            runtime::ReadAccess<runtime::Image> access(result);
            cvsupport::Image::save("DrawContoursTest_testInPlace0.png", access());
        }
        
        void DrawContoursTest::testInPlace1()
        {
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer img(new cvsupport::Image("lenna.jpg"));
            runtime::Data* dataObject = 0;
            std::vector<runtime::Data*> contoursVector;
            dataObject = new cvsupport::Matrix("contour_1.npy");
            contoursVector.push_back(dataObject);
            dataObject = new cvsupport::Matrix("contour_2.npy");
            contoursVector.push_back(dataObject);
            runtime::DataContainer contours(new runtime::List(contoursVector));
            runtime::UInt8 ch1(255);
            runtime::UInt8 ch2(0);
            runtime::UInt8 ch3(0);
            
            m_operator->setInputData(DrawContours::IMG, img);
            m_operator->setInputData(DrawContours::CONTOURS, contours);
            m_operator->setParameter(DrawContours::CH_1, ch1);
            m_operator->setParameter(DrawContours::CH_2, ch2);
            m_operator->setParameter(DrawContours::CH_3, ch3);
            
            runtime::DataContainer result = m_operator->getOutputData(DrawContours::IMG);
            
            runtime::ReadAccess<runtime::Image> access(result);
            cvsupport::Image::save("DrawContoursTest_testInPlace1.png", access());
        }
        
    }
}

