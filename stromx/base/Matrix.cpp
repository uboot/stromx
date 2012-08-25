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

#include "Matrix.h"
#include "Utilities.h"
#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>
#include <stromx/core/OutputProvider.h>
#include <stromx/core/InputProvider.h>
#include <opencv2/core/core.hpp>


namespace stromx
{
    namespace base
    {
        const std::string Matrix::TYPE = "Matrix";
        const std::string Matrix::PACKAGE = STROMX_BASE_PACKAGE_NAME;
        const core::Version Matrix::VERSION = core::Version(BASE_VERSION_MAJOR, BASE_VERSION_MINOR, BASE_VERSION_PATCH);
                
        Matrix::Matrix()
          : m_matrix(new cv::Mat())
        {
            getDataFromCvMatrix(NONE);
        }

        Matrix::Matrix(const unsigned int rows, const unsigned int cols, const stromx::core::Matrix::ValueType valueType)
          : m_matrix(new cv::Mat())
        {
            allocate(rows, cols, valueType);
            getDataFromCvMatrix(valueType);
        }
        
        Matrix::Matrix(cv::Mat& cvMatrix)
          : m_matrix(new cv::Mat(cvMatrix))
        {
            getDataFromCvMatrix(valueTypeFromCvType(m_matrix->type()));
        }

        Matrix::Matrix(const stromx::core::Matrix& matrix)
          : m_matrix(new cv::Mat())
        {
            copy(matrix);
        }

        Matrix::Matrix(const stromx::base::Matrix& matrix)
          : m_matrix(new cv::Mat())
        {
            copy(matrix);
        }

        Matrix::Matrix(const unsigned int size)
          : m_matrix(new cv::Mat())
        {
            allocate(size, 1, UINT_8);
        }

        Matrix::~Matrix()
        {
            delete m_matrix;
        }

        core::Data*const Matrix::clone() const
        {
            return new Matrix(*this);
        }

        void Matrix::deserialize(core::InputProvider& input, const stromx::core::Version& version)
        {
            /*
            * TODO
            * 
            * This code is based on Carl Rogers cnpy library
            * (https://github.com/rogersce/cnpy). This should be mentioned at an appropriate place.
            */
            
            // deserialization currently works only on little endian systems
            if(! isLittleEndian())
                throw stromx::core::Exception("Matrix deserialization is currently only supported on little endian systems");
            
            // open the file
            input.openFile(core::InputProvider::BINARY);
            
            // read and test the magic byte
            char magicByte = 0;
            input.file() >> magicByte;
            if(magicByte != NUMPY_MAGIC_BYTE)
                throw stromx::core::Exception("This is not a numpy file");
            
            // move to the array header size
            input.file().seekg(8);
            uint16_t headerSize = 0;
            input.file().read((char*)(&headerSize), sizeof(headerSize));
            
            // allocate and read the array header
            std::vector<char> headerData(headerSize);
            input.file().read(&headerData[0], headerData.size());
            std::string header(headerData.begin(), headerData.end());
            
            // match the array header regex
            static const boost::regex e(".*'descr': ?'([<>])([uif])(\\d+)'.*'fortran_order': ?(False|True).*'shape': ?\\((\\d+), (\\d+)\\).*");
            boost::smatch what;
            if(! boost::regex_match(header, what, e))
                throw stromx::core::Exception("Failed to parse numpy header.");
            
            // only little endian files are currently supported
            if(what[0] == ">")
                throw stromx::core::Exception("Matrix deserialization does not support big endian files.");
            
            // only C-style arrays are currently supported
            if(what[1] == "True")
                throw stromx::core::Exception("Matrix deserialization does not support fortran-ordered array.");
            
            // extract the matrix dimensions
            int numRows = 0;
            int numCols = 0;
            int wordSize = 0;
            try
            {
                wordSize = boost::lexical_cast<int>(what[3]);
                numRows = boost::lexical_cast<int>(what[5]);
                numCols = boost::lexical_cast<int>(what[6]);
            }
            catch(boost::bad_lexical_cast&)
            {
                throw stromx::core::Exception("Failed to interpret numpy header.");
            }
            
            // get the value type and allocate the matrix
            Matrix::ValueType valueType = valueTypeFromNpyHeader(what[2].str()[0], wordSize);
            allocate(numRows, numCols, valueType);
            
            // read data
            const uint8_t* rowPtr = data();
            unsigned int rowSize = cols() * valueSize();
            for(unsigned int i = 0; i < rows(); ++i)
            {
                input.file().read((char*)(rowPtr), rowSize);
                rowPtr += stride();
            }
            
            if(input.file().fail())
                throw stromx::core::Exception("Failed to load matrix.");
        }
        
        Matrix::ValueType Matrix::valueTypeFromNpyHeader(const char valueType,
                                                         const int wordSize)
        {
            /* valueType  i  i  i  i   u  u  u  u   f  f  f  f  
             * wordSize   1  2  4  8   1  2  4  8   1  2  4  8 */
            Matrix::ValueType valueTypeTable[3][4] =
                {{Matrix::INT_8, Matrix::INT_16, Matrix::INT_32, Matrix::NONE},
                 {Matrix::UINT_8, Matrix::UINT_16, Matrix::UINT_32, Matrix::NONE},
                 {Matrix::NONE, Matrix::NONE, Matrix::FLOAT, Matrix::DOUBLE}};
               
            int i = 0;
            switch(valueType)
            {
            case 'i':
                i = 0;
                break;
            case 'u':
                i = 1;
                break;
            case 'f':
                i = 2;
                break;
            default:
                throw stromx::core::Exception("Unknown numpy value identifier.");
            }
            
            int j = 0;
            switch(wordSize)
            {
            case 1:
                j = 0;
                break;
            case 2:
                j = 1;
                break;
            case 4:
                j = 2;
                break;
            case 8:
                j = 3;
                break;
            default:
                throw stromx::core::Exception("Unsupported numpy word size.");
            }
            
            return valueTypeTable[i][j];
        }

        const bool Matrix::isLittleEndian()
        {
            unsigned char x[] = {1,0};
            short y = *(short*) x;
            return y == 1;
        }

        const char Matrix::npyTypeSymbol(const stromx::core::Matrix::ValueType valueType)
        {
            using namespace stromx::core;
            
            if(valueType == Matrix::INT_8
               || valueType == Matrix::INT_16
               || valueType == Matrix::INT_32)
            {
                return 'i';
            }
            else if(valueType == Matrix::UINT_8
               || valueType == Matrix::UINT_16
               || valueType == Matrix::UINT_32)
            {
                return 'u';
            }
            else if(valueType == Matrix::FLOAT
               || valueType == Matrix::DOUBLE)
            {
                return 'f';
            }
            
            throw stromx::core::Exception("Attempt to serialize unsupported matrix value type.");
        }
             
        void Matrix::serialize(core::OutputProvider& output) const
        {
            /*
            * TODO
            * 
            * This code is based on Carl Rogers cnpy library
            * (https://github.com/rogersce/cnpy). This should be mentioned at an appropriate place.
            */ 
            
            // serialization currently works only on little endian systems
            if(! isLittleEndian())
                throw stromx::core::Exception("Matrix serialization is currently only supported on little endian systems");
            
            // setup the array header
            std::ostringstream header;
            header << "{'descr': '";
            header << '<';
            header << npyTypeSymbol(valueType());
            header << valueSize();
            header << "', 'fortran_order': False, 'shape': (";
            header << rows() << ", " << cols();
            header << "), }";
            
            // extract the header string and pad it
            std::string headerString = header.str();
            int remainder = 16 - (10 + headerString.length()) % 16;
            headerString.append(remainder - 1, ' ');
            
            // open the output file
            std::ostream & outStream = output.openFile("npy", core::OutputProvider::BINARY);
            
            // write the numpy header
            outStream << NUMPY_MAGIC_BYTE;
            outStream << "NUMPY";
            outStream << char(0x01);
            outStream << char(0x00);
            uint16_t headerSize = headerString.size() + 1;
            outStream.write((char*)(&headerSize), sizeof(uint16_t));
            
            // write the array header
            outStream << headerString;
            outStream << '\n';
            
            // write data
            const uint8_t* rowPtr = data();
            unsigned int rowSize = cols() * valueSize();
            for(unsigned int i = 0; i < rows(); ++i)
            {
                outStream.write((const char*)(rowPtr), rowSize);
                rowPtr += rowSize;
            }
            
            if(outStream.fail())
                throw stromx::core::Exception("Failed to save matrix.");
        }

        void Matrix::copy(const stromx::core::Matrix& matrix)
        {
            allocate(matrix.rows(), matrix.cols(), matrix.valueType());
            
            try
            {
                cv::Mat cvInImage = getOpenCvMat(matrix);
                cvInImage.copyTo(*m_matrix);
            }
            catch(cv::Exception&)
            {
                throw stromx::core::Exception("Failed to copy construct matrix.");
            }
        }

        void Matrix::resize(const unsigned int rows, const unsigned int cols, const stromx::core::Matrix::ValueType valueType)
        {
            allocate(rows, cols, valueType);
        }

        void Matrix::resize(const unsigned int size)
        {
            allocate(size, 1, core::Matrix::NONE);
        }

        void Matrix::allocate(const unsigned int rows, const unsigned int cols, const core::Matrix::ValueType valueType)
        {
            try
            {
                *m_matrix = cv::Mat(rows, cols, cvTypeFromValueType(valueType));
                getDataFromCvMatrix(valueType);
            }
            catch(cv::Exception&)
            {
                throw core::OutOfMemory("Failed to allocate matrix.");
            }
        }
        
        void Matrix::getDataFromCvMatrix(const ValueType valueType)
        {
            setBuffer((uint8_t*)(m_matrix->data), m_matrix->step * m_matrix->rows);
            initializeMatrix(m_matrix->rows, m_matrix->cols, m_matrix->step,
                             (uint8_t*)(m_matrix->data), valueType);
        }
        
        const int Matrix::cvTypeFromValueType(const core::Matrix::ValueType valueType)
        {
            switch(valueType)
            {
            case core::Matrix::NONE:
            case core::Matrix::UINT_8:
                return CV_8UC1;
            case core::Matrix::INT_8:
                return CV_8SC1;
            case core::Matrix::UINT_16:
                return CV_16UC1;
            case core::Matrix::INT_16:
                return CV_16SC1;
            case core::Matrix::INT_32:
                return CV_32SC1;
            case core::Matrix::FLOAT:
                return CV_32FC1;
            case core::Matrix::DOUBLE:
                return CV_64FC1;
            default:
                throw core::WrongArgument("Unsupported value type.");  
            }
        }
        
        const core::Matrix::ValueType Matrix::valueTypeFromCvType(const int cvType)
        {
            switch(cvType)
            {
            case CV_8UC1:
                return core::Matrix::UINT_8;
            case CV_8SC1:
                return core::Matrix::INT_8;
            case CV_16UC1:
                return core::Matrix::UINT_16;
            case CV_16SC1:
                return core::Matrix::INT_16;
            case CV_32SC1:
                return core::Matrix::INT_32;
            case CV_32FC1:
                return core::Matrix::FLOAT;
            case CV_64FC1:
                return core::Matrix::DOUBLE;
            default:
                throw core::WrongArgument("Unsupported OpenCV element type.");  
            }
        }
    }
}
