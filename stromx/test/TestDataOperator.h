/* 
*  Copyright 2013 Matthias Fuchs
*
*  This file is part of stromx-studio.
*
*  Stromx-studio is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  Stromx-studio is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with stromx-studio.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef STROMX_TEST_TESTDATAOPERATOR_H
#define STROMX_TEST_TESTDATAOPERATOR_H

#include <stromx/runtime/Matrix.h>
#include <stromx/runtime/OperatorKernel.h>
#include <stromx/runtime/Primitive.h>

#include "stromx/test/Config.h"

namespace stromx 
{
    namespace test 
    {
        class STROMX_TEST_API TestDataOperator : public stromx::runtime::OperatorKernel
        {
            enum Outputs 
            { 
                OUTPUT
            };
            
            enum Parameters
            { 
                DATA_TYPE,
                OBJECT_TYPE,
                SIZE_X,
                SIZE_Y,
                NUM_BINS,
                NUM_LINES,
                POINT_OFFSET
            };
            
            enum DataType
            {
                IMAGE_MONO_8,
                IMAGE_MONO_16,
                IMAGE_RGB_24,
                IMAGE_RGB_48,
                MATRIX_UINT_16,
                MATRIX_FLOAT_32,
                MATRIX_FLOAT_64
            };
            
            enum ObjectType
            {
                IMAGE_RAMP,
                HISTOGRAM,
                LINE_SEGMENTS,
                POINTS,
                POLYGONS
            };
            
        public:
            TestDataOperator();
            
            virtual OperatorKernel* clone() const { return new TestDataOperator; }
            virtual void setParameter(const unsigned int id, const stromx::runtime::Data& value);
            const stromx::runtime::DataRef getParameter(const unsigned int id) const;
            virtual void execute(stromx::runtime::DataProvider& provider);
            virtual void activate();
            
        private:
            static const std::vector<const stromx::runtime::Description*> setupInputs();
            static const std::vector<const stromx::runtime::Description*> setupOutputs();
            static const std::vector<const stromx::runtime::Parameter*> setupParameters();
            
            stromx::runtime::Data* imageRamp();
            stromx::runtime::Data* lineSegments();
            stromx::runtime::Data* histogram();
            stromx::runtime::Data* points();
            stromx::runtime::Data* polygons();
            
            void setMatrixEntry(stromx::runtime::Matrix* matrix, unsigned int row,
                                unsigned int col, double value);
            
            static const std::string TYPE;
            static const std::string PACKAGE;
            static const stromx::runtime::Version VERSION;
            
            stromx::runtime::Enum m_dataType;
            stromx::runtime::Enum m_objectType;
            stromx::runtime::UInt32 m_sizeX;
            stromx::runtime::UInt32 m_sizeY;
            stromx::runtime::UInt32 m_numBins;
            stromx::runtime::UInt32 m_numLines;
            stromx::runtime::UInt32 m_pointOffset;
            
            unsigned int m_index;
        };
    }
}

#endif // STROMX_TEST_TESTDATAOPERATOR_H
