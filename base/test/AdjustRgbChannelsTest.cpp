#include "AdjustRgbChannelsTest.h"

#include <base/AdjustRgbChannels.h>
#include <base/Image.h>

#include <stream/DataContainer.h>
#include <stream/OperatorWrapper.h>
#include <stream/Primitive.h>
#include <stream/ReadAccess.h>

#include <cppunit/TestAssert.h>

CPPUNIT_TEST_SUITE_REGISTRATION (base::AdjustRgbChannelsTest);

using namespace stream;

namespace base
{
    void AdjustRgbChannelsTest::setUp ( void )
    {
        m_operator = new OperatorWrapper(new AdjustRgbChannels());
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
        
        stream::DataContainer result = m_operator->getOutputData(AdjustRgbChannels::OUTPUT);
        ReadAccess access(result);
        const Image* image = dynamic_cast<const Image*>(access());
        
        image->save("AdjustRgbChannelsTest_testExecute.png");
    }
    
    void AdjustRgbChannelsTest::tearDown ( void )
    {
        delete m_operator;
    }
}