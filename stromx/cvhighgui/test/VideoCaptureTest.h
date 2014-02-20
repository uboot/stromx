/* 
*  Copyright 2013 Thomas Fidler
*
*  Licensed under the Apache License, Version 2.0 (the "License");
*  you may not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS,
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
*/

#ifndef STROMX_CVHIGHGUI_VIDEOCAPTURETEST_H
#define STROMX_CVHIGHGUI_VIDEOCAPTURETEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

namespace stromx
{
    namespace runtime
    {
        class OperatorTester;
    }
    
    namespace cvhighgui
    {
        class VideoCaptureTest : public CPPUNIT_NS :: TestFixture
        {
            
            
            CPPUNIT_TEST_SUITE (VideoCaptureTest);
            CPPUNIT_TEST (testParameterFrameRate);
            CPPUNIT_TEST (testParameterBrightness);
            CPPUNIT_TEST (testParameterContrast);
            CPPUNIT_TEST (testParameterSaturation);
            CPPUNIT_TEST (testParameterHue);
            CPPUNIT_TEST (testParameterGain);
            CPPUNIT_TEST (testParameterExposure);
            CPPUNIT_TEST (testExecute);
            CPPUNIT_TEST_SUITE_END ();
            
            public:
                VideoCaptureTest():m_operator(0),m_hasCamera(true),m_deltaAcceptance(0.01){}
                
                void setUp();
                void tearDown();
                
            protected:
                void testParameterFrameRate();
                void testParameterBrightness();
                void testParameterContrast();
                void testParameterSaturation();
                void testParameterHue();
                void testParameterGain();
                void testParameterExposure();
                void testExecute();
                
            private:
                
                bool checkParameter(const unsigned int id);
                
                runtime::OperatorTester* m_operator;
                bool m_hasCamera;
                double m_deltaAcceptance;
        };
    }
}

#endif // STROMX_CVHIGHGUI_VIDEOCAPTURETEST_H
