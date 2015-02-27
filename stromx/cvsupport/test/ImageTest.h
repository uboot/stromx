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

#ifndef STROMX_CVSUPPORT_IMAGETEST_H
#define STROMX_CVSUPPORT_IMAGETEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <stromx/runtime/Version.h>

namespace stromx
{
    namespace cvsupport
    {
        class Image;

        class ImageTest : public CPPUNIT_NS :: TestFixture
        {
            CPPUNIT_TEST_SUITE (ImageTest);
            CPPUNIT_TEST (testOpenUnknownFile);
            CPPUNIT_TEST (testOpenJpeg);
            CPPUNIT_TEST (testOpenAs16BitColor);
            CPPUNIT_TEST (testOpenAs16BitGrayscale);
            CPPUNIT_TEST (testOpenAsColor);
            CPPUNIT_TEST (testOpenAsGrayscale);
            CPPUNIT_TEST (testOpenUnchanged);
            CPPUNIT_TEST (testImageFile);
            CPPUNIT_TEST (testImageEmpty);
            CPPUNIT_TEST (testImageCopyConstructor);
            CPPUNIT_TEST (testImageEmptyCopyConstructor);
            CPPUNIT_TEST (testImageDefaultCopyConstructor);
            CPPUNIT_TEST (testImageCvImageConstructor);
            CPPUNIT_TEST (testImageRgb24);
            CPPUNIT_TEST (testImageMono8);
            CPPUNIT_TEST (testSaveAfterInitializeImage);
            CPPUNIT_TEST (testSaveJpeg);
            CPPUNIT_TEST (testSave16Bit);
            CPPUNIT_TEST (testSaveUnknownDirectory);
            CPPUNIT_TEST (testImageDefault);
            CPPUNIT_TEST (testSerialize);
            CPPUNIT_TEST (testSerializeAfterInitializeImage);
            CPPUNIT_TEST (testSerializeAfterInitializeMatrix);
            CPPUNIT_TEST (testSerializeEmpty);
            CPPUNIT_TEST (testDeserialize);
            CPPUNIT_TEST (testDeserializeWithType);
            CPPUNIT_TEST (testDeserializeEmpty);
            CPPUNIT_TEST (testIsVariant);
            CPPUNIT_TEST (testPtrCast);
            CPPUNIT_TEST (testRefCast);
            CPPUNIT_TEST_SUITE_END ();

            public:
                ImageTest() : m_image(0) {}
                
                void setUp();
                void tearDown();

            protected:
                void testOpenJpeg();
                void testOpenAs16BitColor();
                void testOpenAs16BitGrayscale();
                void testOpenAsColor();
                void testOpenAsGrayscale();
                void testOpenUnchanged();
                void testOpenUnknownFile();
                void testImageFile();
                void testImageEmpty();
                void testImageCopyConstructor();
                void testImageEmptyCopyConstructor();
                void testImageDefaultCopyConstructor();
                void testImageCvImageConstructor();
                void testImageRgb24();
                void testImageMono8();
                void testImageDefault();
                void testSaveAfterInitializeImage();
                void testSaveJpeg();
                void testSave16Bit();
                void testSaveUnknownDirectory();
                void testSerialize();
                void testSerializeAfterInitializeImage();
                void testSerializeAfterInitializeMatrix();
                void testSerializeEmpty();
                void testDeserialize();
                void testDeserializeWithType();
                void testDeserializeEmpty();
                void testIsVariant();
                void testPtrCast();
                void testRefCast();
                
            private:
                const static stromx::runtime::Version VERSION;
                
                Image* m_image;
        };
    }
}

#endif // STROMX_CVSUPPORT_IMAGETEST_H