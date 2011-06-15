#include "AdjustRgbChannelsTest.h"

#include <base/AdjustRgbChannels.h>
#include <base/Image.h>
#include <stream/DataContainer.h>
#include <stream/OperatorWrapper.h>
#include <stream/Primitive.h>

#include <cppunit/TestAssert.h>

CPPUNIT_TEST_SUITE_REGISTRATION (base::AdjustRgbChannelsTest);

using namespace stream;

namespace base
{
    void AdjustRgbChannelsTest::setUp ( void )
    {
        m_operator = new OperatorWrapper(new AdjustRgbChannels());
        m_operator->activate();
        m_image = new DataContainer(new Image("lenna.jpg"));
        m_operator->setInputData(AdjustRgbChannels::INPUT, m_image);
    }
    
    void AdjustRgbChannelsTest::testExecute()
    {
    }
    
    void AdjustRgbChannelsTest::tearDown ( void )
    {
        delete m_operator;
    }
}