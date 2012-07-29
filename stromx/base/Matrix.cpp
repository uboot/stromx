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

namespace stromx
{
    namespace base
    {
        const std::string Matrix::TYPE = "Matrix";
        const std::string Matrix::PACKAGE = STROMX_BASE_PACKAGE_NAME;
        const core::Version Matrix::VERSION = core::Version(BASE_VERSION_MAJOR, BASE_VERSION_MINOR, BASE_VERSION_PATCH);
                
        Matrix::Matrix()
        {

        }

        Matrix::Matrix(const unsigned int rows, const unsigned int cols, const stromx::core::Matrix::ValueType valueType)
        {

        }

        Matrix::Matrix(const stromx::core::Matrix& image)
        {

        }

        Matrix::Matrix(const stromx::base::Matrix& image)
        {

        }

        Matrix::Matrix(const unsigned int size)
        {

        }

        Matrix::~Matrix()
        {

        }

        core::Data*const Matrix::clone() const
        {

        }

        void Matrix::deserialize(core::InputProvider& input, const stromx::core::Version& version)
        {
            stromx::core::Data::deserialize(input, version);
        }

        void Matrix::serialize(core::OutputProvider& output) const
        {
            stromx::core::Data::serialize(output);
        }

        void Matrix::copy(const stromx::core::Matrix& matrix)
        {

        }

        void Matrix::resize(const unsigned int rows, const unsigned int cols, const stromx::core::Matrix::ValueType valueType)
        {

        }

        void Matrix::resize(const unsigned int size)
        {

        }

        void Matrix::allocate(const unsigned int width, const unsigned int height, const stromx::core::Matrix::ValueType valueType)
        {

        }
    }
}
