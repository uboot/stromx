/* 
*  Copyright 2011 Matthias Fuchs
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

#ifndef BASE_IMAGETEST_H
#define BASE_IMAGETEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace base
{
    class Image;

    class ImageTest : public CPPUNIT_NS :: TestFixture
    {
        CPPUNIT_TEST_SUITE (ImageTest);
        CPPUNIT_TEST (testOpenUnknownFile);
        CPPUNIT_TEST (testOpenJpeg);
        CPPUNIT_TEST (testImageFile);
        CPPUNIT_TEST (testImageRgb24);
        CPPUNIT_TEST (testImageMono8);
        CPPUNIT_TEST (testSaveJpeg);
        CPPUNIT_TEST (testImageDefault);
        CPPUNIT_TEST (testSerialize);
        CPPUNIT_TEST (testDeserialize);
        CPPUNIT_TEST_SUITE_END ();

        public:
            ImageTest() : m_image(0) {}
            
            void setUp();
            void tearDown();

        protected:
            void testOpenJpeg();
            void testOpenUnknownFile();
            void testImageFile();
            void testImageRgb24();
            void testImageMono8();
            void testImageDefault();
            void testSaveJpeg();
            void testSerialize();
            void testDeserialize();
            
        private:
            Image* m_image;
    };
}

#endif // BASE_IMAGETEST_H