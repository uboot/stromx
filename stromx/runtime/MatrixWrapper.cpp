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
#include <fstream>
#include "stromx/runtime/Exception.h"
#include "stromx/runtime/InputProvider.h"
#include "stromx/runtime/MatrixWrapper.h"
#include "stromx/runtime/OutputProvider.h"

namespace stromx
{
    namespace runtime
    {
        MatrixWrapper::MatrixWrapper(const unsigned int bufferSize, uint8_t* const buffer)
          : m_rows(0),
            m_cols(0),
            m_stride(0),
            m_bufferSize(bufferSize),
            m_valueType(NONE),
            m_data(0),
            m_buffer(buffer),
            m_variant(Variant::IMAGE)
        {
        }
        
        MatrixWrapper::MatrixWrapper()
          : m_rows(0),
            m_cols(0),
            m_stride(0),
            m_bufferSize(0),
            m_valueType(NONE),
            m_data(0),
            m_buffer(0),
            m_variant(Variant::IMAGE)
        {
        }

        void MatrixWrapper::setBuffer(uint8_t*const buffer, const unsigned int bufferSize)
        {
            m_bufferSize = bufferSize;
            m_buffer = buffer;
        }
        
        void MatrixWrapper::initializeMatrix(const unsigned int rows, const unsigned int cols,
                                             const unsigned int stride, uint8_t* data, 
                                             const ValueType valueType) 
        {
            validate(rows, cols, stride, data, valueType);
            
            m_rows = rows;
            m_cols = cols;
            m_stride = stride;
            m_data = data;
            m_valueType = valueType;
            m_variant = dataVariantFromValueType(valueType);
        }
        
        void MatrixWrapper::validate(const unsigned int rows,
                                     const unsigned int cols,
                                     const unsigned int stride,
                                     const uint8_t*const data,
                                     const ValueType valueType) const
        {
            if(cols == 0 || rows == 0)
                return;
            
            // check row length
            if(cols * Matrix::valueSize(valueType) > stride)
                throw WrongArgument("Too small stride.");
            
            // check total data size
            unsigned int dataSize = stride * (rows - 1) + cols;
            if(data + dataSize > m_buffer + m_bufferSize)
                throw WrongArgument("Too small buffer.");
        }

        void MatrixWrapper::resize(const unsigned int rows, const unsigned int cols, const stromx::runtime::Matrix::ValueType valueType)
        {
            allocate(rows, cols, valueType);
        }

        void MatrixWrapper::resize(const unsigned int size)
        {
            allocate(size, 1, runtime::Matrix::NONE);
        }
        
        void MatrixWrapper::open(const std::string& filename)
        {
            // open the file
            std::ifstream in(filename.c_str(), std::ios_base::in | std::ios_base::binary);
            
            // deserialize the matrix
            doDeserialize(in, *this);
        }
        
        void MatrixWrapper::save(const std::string& filename) const
        {
            // open the file
            std::ofstream out(filename.c_str(), std::ios_base::out | std::ios_base::binary);
            
            // deserialize the matrix
            doSerialize(out, *this);
        }
        
        void MatrixWrapper::serialize(runtime::OutputProvider& output) const
        {
            // open the output file
            std::ostream & outStream = output.openFile("npy", runtime::OutputProvider::BINARY);
            
            // call the actual serialization function
            doSerialize(outStream, *this);
        }

        void MatrixWrapper::deserialize(runtime::InputProvider& input, const stromx::runtime::Version&)
        {
            // open the file
            input.openFile(runtime::InputProvider::BINARY);
            
            // call the actual deserialization method
            doDeserialize(input.file(), *this);
        }
        
        void MatrixWrapper::doDeserialize(std::istream& in, MatrixWrapper & matrix)
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
            std::string endianess = what[1];
            if(endianess == ">")
                throw stromx::runtime::Exception("Matrix deserialization does not support big endian files.");
            
            // only C-style arrays are currently supported
            bool isFortran = false;
            if(what[4] == "True")
            	isFortran = true;
            
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
            std::string type = what[2];
            Matrix::ValueType valueType = valueTypeFromNpyHeader(type[0], wordSize);
            matrix.allocate(numRows, numCols, valueType);
            
            // read data
            if (isFortran)
            {
                // in case the input stream is Fortran-style read each element
                // and write it to the transposed position in the matrix
                uint8_t* dataPtr = matrix.data();
                for (unsigned int j = 0; j < matrix.cols(); ++j)
                {
                    for (unsigned int i = 0; i < matrix.rows(); ++i)
                    {
                        unsigned int offsetInBytes = i * matrix.stride() +
                                                     j * matrix.valueSize();
                        char* elementPtr = (char*)(dataPtr + offsetInBytes);
                        in.read(elementPtr, matrix.valueSize());
                    }
                }
            }
            else
            {
                // in case the input stream is C-style read each row to the
                // matrix
                uint8_t* rowPtr = matrix.data();
                unsigned int rowSize = matrix.cols() * matrix.valueSize();
                for(unsigned int i = 0; i < matrix.rows(); ++i)
                {
                    in.read((char*)(rowPtr), rowSize);
                    rowPtr += matrix.stride();
                }
            }
            

            if(in.fail())
                throw stromx::runtime::Exception("Failed to load matrix.");
        }
        
        Matrix::ValueType MatrixWrapper::valueTypeFromNpyHeader(const char valueType,
                                                         const int wordSize)
        {
            /* valueType  i  i  i  i   u  u  u  u   f  f  f  f  
             * wordSize   1  2  4  8   1  2  4  8   1  2  4  8 */
            Matrix::ValueType valueTypeTable[3][4] =
                {{Matrix::INT_8, Matrix::INT_16, Matrix::INT_32, Matrix::NONE},
                 {Matrix::UINT_8, Matrix::UINT_16, Matrix::UINT_32, Matrix::NONE},
                 {Matrix::NONE, Matrix::NONE, Matrix::FLOAT_32, Matrix::FLOAT_64}};
               
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

        bool MatrixWrapper::isLittleEndian()
        {
            unsigned char x[] = {1,0};
            short y = *(short*) x;
            return y == 1;
        }

        char MatrixWrapper::npyTypeSymbol(const stromx::runtime::Matrix::ValueType valueType)
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
            else if(valueType == Matrix::FLOAT_32
               || valueType == Matrix::FLOAT_64)
            {
                return 'f';
            }
            
            throw stromx::runtime::Exception("Attempt to serialize unsupported matrix value type.");
        }
        
        void MatrixWrapper::doSerialize(std::ostream& out, const runtime::Matrix & matrix)
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
        
        void MatrixWrapper::save(const std::string& filename, const runtime::Matrix& matrix)
        {
            // open the file
            std::ofstream out(filename.c_str(), std::ios_base::out | std::ios_base::binary);
            
            // deserialize the matrix
            doSerialize(out, matrix);
        }
    }
}
