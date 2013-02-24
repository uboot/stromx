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

#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>
#include <opencv2/core/core.hpp>
#include <fstream>
#include "stromx/example/Matrix.h"
#include "stromx/example/Utilities.h"
#include <stromx/runtime/InputProvider.h>
#include <stromx/runtime/OutputProvider.h>

namespace stromx
{
    namespace example
    {
        const std::string Matrix::TYPE = "Matrix";
        const std::string Matrix::PACKAGE = STROMX_EXAMPLE_PACKAGE_NAME;
        const runtime::Version Matrix::VERSION = runtime::Version(STROMX_EXAMPLE_VERSION_MAJOR, STROMX_EXAMPLE_VERSION_MINOR, STROMX_EXAMPLE_VERSION_PATCH);
                
        Matrix::Matrix()
          : m_matrix(new cv::Mat())
        {
            getDataFromCvMatrix(NONE);
        }

        Matrix::Matrix(const unsigned int rows, const unsigned int cols, const stromx::runtime::Matrix::ValueType valueType)
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

        Matrix::Matrix(const stromx::runtime::Matrix& matrix)
          : m_matrix(new cv::Mat())
        {
            copy(matrix);
        }

        Matrix::Matrix(const stromx::example::Matrix& matrix)
          : runtime::MatrixWrapper(),
            m_matrix(new cv::Mat())
        {
            copy(matrix);
        }

        Matrix::Matrix(const unsigned int size)
          : m_matrix(new cv::Mat())
        {
            allocate(size, 1, UINT_8);
        }
        
        Matrix::Matrix(const std::string& filename)
          : m_matrix(new cv::Mat())
        {
            open(filename);
        }

        Matrix::~Matrix()
        {
            delete m_matrix;
        }

        runtime::Data* Matrix::clone() const
        {
            return new Matrix(*this);
        }
                
        void Matrix::doDeserialize(std::istream& in, example::Matrix & matrix)
        {
            /*
            * TODO
            * 
            * This code is based on Carl Rogers cnpy library
            * (https://github.com/rogersce/cnpy). This should be mentioned at an appropriate place.
            */
            
            // deserialization currently works only on little endian systems
            if(! isLittleEndian())
                throw stromx::runtime::Exception("Matrix deserialization is currently only supported on little endian systems");  
            
            // read and test the magic byte
            char magicByte = 0;
            in >> magicByte;
            if(magicByte != NUMPY_MAGIC_BYTE)
                throw stromx::runtime::Exception("This is not a numpy file");
            
            // move to the array header size
            in.seekg(8);
            uint16_t headerSize = 0;
            in.read((char*)(&headerSize), sizeof(headerSize));
            
            // allocate and read the array header
            std::vector<char> headerData(headerSize);
            in.read(&headerData[0], headerData.size());
            std::string header(headerData.begin(), headerData.end());
            
            // match the array header regex
            static const boost::regex e(".*'descr': ?'([<>])([uif])(\\d+)'.*'fortran_order': ?(False|True).*'shape': ?\\((\\d+), (\\d+)\\).*");
            boost::smatch what;
            if(! boost::regex_match(header, what, e))
                throw stromx::runtime::Exception("Failed to parse numpy header.");
            
            // only little endian files are currently supported
            if(what[0] == ">")
                throw stromx::runtime::Exception("Matrix deserialization does not support big endian files.");
            
            // only C-style arrays are currently supported
            if(what[1] == "True")
                throw stromx::runtime::Exception("Matrix deserialization does not support fortran-ordered array.");
            
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
                throw stromx::runtime::Exception("Failed to interpret numpy header.");
            }
            
            // get the value type and allocate the matrix
            Matrix::ValueType valueType = valueTypeFromNpyHeader(what[2].str()[0], wordSize);
            matrix.allocate(numRows, numCols, valueType);
            
            // read data
            const uint8_t* rowPtr = matrix.data();
            unsigned int rowSize = matrix.cols() * matrix.valueSize();
            for(unsigned int i = 0; i < matrix.rows(); ++i)
            {
                in.read((char*)(rowPtr), rowSize);
                rowPtr += matrix.stride();
            }
            
            if(in.fail())
                throw stromx::runtime::Exception("Failed to load matrix.");
        }

        void Matrix::deserialize(runtime::InputProvider& input, const stromx::runtime::Version&)
        {
            // open the file
            input.openFile(runtime::InputProvider::BINARY);
            
            // call the actual deserialization method
            doDeserialize(input.file(), *this);
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
                throw stromx::runtime::Exception("Unknown numpy value identifier.");
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
                throw stromx::runtime::Exception("Unsupported numpy word size.");
            }
            
            return valueTypeTable[i][j];
        }

        bool Matrix::isLittleEndian()
        {
            unsigned char x[] = {1,0};
            short y = *(short*) x;
            return y == 1;
        }

        char Matrix::npyTypeSymbol(const stromx::runtime::Matrix::ValueType valueType)
        {
            using namespace stromx::runtime;
            
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
            
            throw stromx::runtime::Exception("Attempt to serialize unsupported matrix value type.");
        }
        
        void Matrix::doSerialize(std::ostream& out, const runtime::Matrix & matrix)
        {
            /*
            * TODO
            * 
            * This code is based on Carl Rogers cnpy library
            * (https://github.com/rogersce/cnpy). This should be mentioned at an appropriate place.
            */ 
            
            // serialization currently works only on little endian systems
            if(! isLittleEndian())
                throw stromx::runtime::Exception("Matrix serialization is currently only supported on little endian systems");
            
            // setup the array header
            std::ostringstream header;
            header << "{'descr': '";
            header << '<';
            header << npyTypeSymbol(matrix.valueType());
            header << matrix.valueSize();
            header << "', 'fortran_order': False, 'shape': (";
            header << matrix.rows() << ", " << matrix.cols();
            header << "), }";
            
            // extract the header string and pad it
            std::string headerString = header.str();
            int remainder = 16 - (10 + headerString.length()) % 16;
            headerString.append(remainder - 1, ' ');
            
            // write the numpy header
            out << NUMPY_MAGIC_BYTE;
            out << "NUMPY";
            out << char(0x01);
            out << char(0x00);
            uint16_t headerSize = headerString.size() + 1;
            out.write((char*)(&headerSize), sizeof(uint16_t));
            
            // write the array header
            out << headerString;
            out << '\n';
            
            // write data
            const uint8_t* rowPtr = matrix.data();
            unsigned int rowSize = matrix.cols() * matrix.valueSize();
            for(unsigned int i = 0; i < matrix.rows(); ++i)
            {
                out.write((const char*)(rowPtr), rowSize);
                rowPtr += rowSize;
            }
            
            if(out.fail())
                throw stromx::runtime::Exception("Failed to save matrix.");
        }
             
        void Matrix::serialize(runtime::OutputProvider& output) const
        {
            // open the output file
            std::ostream & outStream = output.openFile("npy", runtime::OutputProvider::BINARY);
            
            // call the actual serialization function
            doSerialize(outStream, *this);
        }

        void Matrix::copy(const stromx::runtime::Matrix& matrix)
        {
            allocate(matrix.rows(), matrix.cols(), matrix.valueType());
            
            try
            {
                cv::Mat cvInImage = getOpenCvMat(matrix);
                cvInImage.copyTo(*m_matrix);
            }
            catch(cv::Exception&)
            {
                throw stromx::runtime::Exception("Failed to copy construct matrix.");
            }
        }

        void Matrix::resize(const unsigned int rows, const unsigned int cols, const stromx::runtime::Matrix::ValueType valueType)
        {
            allocate(rows, cols, valueType);
        }

        void Matrix::resize(const unsigned int size)
        {
            allocate(size, 1, runtime::Matrix::NONE);
        }
        
        void Matrix::open(const std::string& filename)
        {
            // open the file
            std::ifstream in(filename.c_str(), std::ios_base::in | std::ios_base::binary);
            
            // deserialize the matrix
            doDeserialize(in, *this);
        }

        void Matrix::save(const std::string& filename) const
        {
            save(filename, *this);
        }

        void Matrix::allocate(const unsigned int rows, const unsigned int cols, const runtime::Matrix::ValueType valueType)
        {
            try
            {
                *m_matrix = cv::Mat(rows, cols, cvTypeFromValueType(valueType));
                getDataFromCvMatrix(valueType);
            }
            catch(cv::Exception&)
            {
                throw runtime::OutOfMemory("Failed to allocate matrix.");
            }
        }
        
        void Matrix::getDataFromCvMatrix(const ValueType valueType)
        {
            setBuffer((uint8_t*)(m_matrix->data), m_matrix->step * m_matrix->rows);
            initializeMatrix(m_matrix->rows, m_matrix->cols * m_matrix->channels(), m_matrix->step,
                             (uint8_t*)(m_matrix->data), valueType);
        }
        
        int Matrix::cvTypeFromValueType(const runtime::Matrix::ValueType valueType)
        {
            switch(valueType)
            {
            case runtime::Matrix::NONE:
            case runtime::Matrix::UINT_8:
                return CV_8UC1;
            case runtime::Matrix::INT_8:
                return CV_8SC1;
            case runtime::Matrix::UINT_16:
                return CV_16UC1;
            case runtime::Matrix::INT_16:
                return CV_16SC1;
            case runtime::Matrix::INT_32:
                return CV_32SC1;
            case runtime::Matrix::FLOAT:
                return CV_32FC1;
            case runtime::Matrix::DOUBLE:
                return CV_64FC1;
            default:
                throw runtime::WrongArgument("Unsupported value type.");  
            }
        }
        
        runtime::Matrix::ValueType Matrix::valueTypeFromCvType(const int cvType)
        {
            switch(cvType)
            {
            case CV_8UC1:
            case CV_8UC2:
            case CV_8UC3:
            case CV_8UC4:
                return runtime::Matrix::UINT_8;
            case CV_8SC1:
            case CV_8SC2:
            case CV_8SC3:
            case CV_8SC4:
                return runtime::Matrix::INT_8;
            case CV_16UC1:
            case CV_16UC2:
            case CV_16UC3:
            case CV_16UC4:
                return runtime::Matrix::UINT_16;
            case CV_16SC1:
            case CV_16SC2:
            case CV_16SC3:
            case CV_16SC4:
                return runtime::Matrix::INT_16;
            case CV_32SC1:
            case CV_32SC2:
            case CV_32SC3:
            case CV_32SC4:
                return runtime::Matrix::INT_32;
            case CV_32FC1:
            case CV_32FC2:
            case CV_32FC3:
            case CV_32FC4:
                return runtime::Matrix::FLOAT;
            case CV_64FC1:
            case CV_64FC2:
            case CV_64FC3:
            case CV_64FC4:
                return runtime::Matrix::DOUBLE;
            default:
                throw runtime::WrongArgument("Unsupported OpenCV element type.");  
            }
        }
        
        void Matrix::save(const std::string& filename, const runtime::Matrix& matrix)
        {
            // open the file
            std::ofstream out(filename.c_str(), std::ios_base::out | std::ios_base::binary);
            
            // deserialize the matrix
            doSerialize(out, matrix);
        }
    }
}
