#include "AdjustRgbChannelsTest.h"


#include <stromx/base/AdjustRgbChannels.h>

#include <stromx/base/Image.h>

#include <stromx/core/DataContainer.h>
#include <stromx/core/Primitive.h>
#include <stromx/core/ReadAccess.h>

#include <stromx/core/OperatorTester.h>

#include <cppunit/TestAssert.h>

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::base::AdjustRgbChannelsTest);

namespace stromx
{
    using namespace core;

    namespace base
    {
        void AdjustRgbChannelsTest::setUp ( void )
        {
            m_operator = new core::OperatorTester(new AdjustRgbChannels());
            m_operator->initialize();
            m_operator->activate();
            DataContainer image(new Image("lenna.jpg"));
            m_operator->setInputData(AdjustRgbChannels::INPUT, image);
        }
        
        void AdjustRgbChannelsTest::testExecute()
        {
            m_operator->setParameter(AdjustRgbChannels::RED, Double(0.1));
            m_operator->setParameter(AdjustRgbChannels::GREEN, Double(1.0));
            m_operator->setParameter(AdjustRgbChannels::BLUE, Double(1.5));
            
            core::DataContainer result = m_operator->getOutputData(AdjustRgbChannels::OUTPUT);
            ReadAccess<Image> access(result);
            const Image& image = access();
            
            image.save("AdjustRgbChannelsTest_testExecute.png");
        }
        
        void AdjustRgbChannelsTest::tearDown ( void )
        {
            delete m_operator;
        }
    }
}
