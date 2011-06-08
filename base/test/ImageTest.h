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
        CPPUNIT_TEST (testSave);
        CPPUNIT_TEST_SUITE_END ();

        public:
            ImageTest() : m_image(0) {}
            
            void setUp();
            void tearDown();

        protected:
            void testSave();
            
        private:
            Image* m_image;
    };
}

#endif // BASE_IMAGETEST_H