/* 
*  Copyright 2012 Matthias Fuchs
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

#include <cppunit/TestAssert.h>
#include <stromx/core/DataContainer.h>
#include <stromx/core/OperatorException.h>
#include <stromx/core/OperatorTester.h>
#include <stromx/core/Primitive.h>
#include <stromx/core/ReadAccess.h>
#include "../Image.h"

#include "ImageFilterTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::base::ImageFilterTest);

namespace stromx
{
    namespace base
    {
        using namespace core;
        
        class ImageFilterTester : public ImageFilter
        {
        public:
            ImageFilterTester()
              : ImageFilter("Test")
            {
            }
            
            OperatorKernel* clone() const
            {
                return new ImageFilterTester;
            } 
            
            const std::vector< const stromx::core::Parameter* > setupParameters()
            {
                return std::vector<const core::Parameter*>();
            }
    
            void applyFilter (const cv::Mat&, cv::Mat&)
            {
            }
            
            void validateSourceImage(const core::Image & source)
            {
                if(source.pixelType() != core::Image::MONO_8)
                    throw core::InputError(SOURCE, *this, "Source image is not a grayscale image.");
            }
            
            unsigned int computeDestinationSize(const core::Image & source)
            {
                return source.width() * source.pixelSize() * source.height();
            }
        };
        
        void ImageFilterTest::setUp()
        {
            m_operator = new OperatorTester(new ImageFilterTester);
        }

        void ImageFilterTest::testTwoInputs()
        {
            m_operator->setParameter(ImageFilter::IN_PLACE, Bool(false));
            m_operator->initialize();
            m_operator->activate();
            
            core::Image* sourceImage = new base::Image("lenna_bw.jpg");
            DataContainer source(sourceImage);
            m_operator->setInputData(ImageFilter::SOURCE, source);
            
            core::Image* destinationImage = new Image(sourceImage->width(), sourceImage->height(),
                                                      core::Image::BAYERBG_8);
            DataContainer destination(destinationImage);
            m_operator->setInputData(ImageFilter::DESTINATION, destination);
            
            DataContainer result = m_operator->getOutputData(ImageFilter::OUTPUT);
            
            ReadAccess<Image> access(result);
            const Image& resultImage = access();
            CPPUNIT_ASSERT_EQUAL((const Image*)(destinationImage), &resultImage);
        }

        void ImageFilterTest::testOneInput()
        {
            m_operator->setParameter(ImageFilter::IN_PLACE, Bool(true));
            m_operator->initialize();
            m_operator->activate();
            
            core::Image* sourceImage = new base::Image("lenna_bw.jpg");
            DataContainer source(sourceImage);
            m_operator->setInputData(ImageFilter::SOURCE, source);
            
            DataContainer result = m_operator->getOutputData(ImageFilter::OUTPUT);
            
            ReadAccess<Image> access(result);
            const Image& resultImage = access();
            CPPUNIT_ASSERT_EQUAL((const Image*)(sourceImage), &resultImage);
        }


        void ImageFilterTest::testIdenticalInputs()
        {
            m_operator->setParameter(ImageFilter::IN_PLACE, Bool(false));
            m_operator->initialize();
            m_operator->activate();
            
            core::Image* sourceImage = new base::Image("lenna_bw.jpg");
            DataContainer source(sourceImage);
            m_operator->setInputData(ImageFilter::SOURCE, source);
            m_operator->setInputData(ImageFilter::DESTINATION, source);
            
            DataContainer result = m_operator->getOutputData(ImageFilter::OUTPUT);
            
            ReadAccess<Image> access(result);
            const Image& resultImage = access();
            CPPUNIT_ASSERT_EQUAL((const Image*)(sourceImage), &resultImage);
        }
        
        void ImageFilterTest::testWrongSource()
        {
            m_operator->setParameter(ImageFilter::IN_PLACE, Bool(true));
            m_operator->initialize();
            m_operator->activate();
            
            core::Image* sourceImage = new Image(100, 100, core::Image::BAYERBG_8);
            DataContainer source(sourceImage);
            m_operator->setInputData(ImageFilter::SOURCE, source);
            
            DataContainer result;
            CPPUNIT_ASSERT_THROW(result = m_operator->getOutputData(ImageFilter::OUTPUT), InputError);
        }

        void ImageFilterTest::testWrongDestination()
        {
            m_operator->setParameter(ImageFilter::IN_PLACE, Bool(false));
            m_operator->initialize();
            m_operator->activate();
            
            core::Image* sourceImage = new base::Image("lenna_bw.jpg");
            DataContainer source(sourceImage);
            m_operator->setInputData(ImageFilter::SOURCE, source);
            
            core::Image* destinationImage = new Image(100, 100, core::Image::BAYERBG_8);
            DataContainer destination(destinationImage);
            m_operator->setInputData(ImageFilter::DESTINATION, destination);
            
            DataContainer result;
            CPPUNIT_ASSERT_THROW(result = m_operator->getOutputData(ImageFilter::OUTPUT), InputError);
        }

        void ImageFilterTest::tearDown()
        {
            delete m_operator;
        }
    }
}
