/* 
 *  Copyright 2015 Matthias Fuchs
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

#ifndef STROMX_RUNTIME_FILE_H
#define STROMX_RUNTIME_FILE_H

#ifdef __GNUG__
    #include <tr1/cstdint>
#else
    #include <cstdint>
#endif

#include "stromx/runtime/Data.h"
#include <vector>

namespace stromx
{
    namespace runtime
    {
        /** \brief %Data object which represents a path to a file. */
        class STROMX_RUNTIME_API File : public Data
        {
        public:
            /** The type the file. */
            enum OpenMode
            { 
                /** Binary file. */
                BINARY,
                /** Text file. */
                TEXT 
            };
            
            /** Creates an empty file, i.e. a file object with no path. */
            File() : m_mode(TEXT) {}
            
            /**
             * Creates an empty file but sets the file extension. Use this e.g. to
             * indicate the file type of a parameter.
             */
            File(const std::string & extension) : m_mode(TEXT), m_extension(extension) {}
            
            /** 
             * Create a file object which represents the file at \c path.
             * 
             * \throw FileAccessFailed If \c path does not exist or if \c path is not 
             *                         is not a directory.
             */
            File(const std::string & path, const OpenMode mode = TEXT);
            
            /** 
             * Copy constructs the file, i.e. it creates a file object which
             * references the same file as \c file.
             */
            File(const File & file);
            
            /** Returns the path to this file. */
            const std::string & path() const { return m_path; }
            
            /** Returns the mode of this file. */
            OpenMode mode() const { return m_mode; }
            
            /** Returns the extension of this file. */
            const std::string & extension() const { return m_extension; }
            
            virtual const Version & version() const { return VERSION; }
            virtual const std::string & type() const { return TYPE; }
            virtual const std::string & package() const { return PACKAGE; }
            
            virtual const VariantHandle & variant() const;
            
            virtual Data* clone() const { return new File(*this); }
            virtual void serialize(OutputProvider & output) const;
            virtual void deserialize(InputProvider & input, const Version & version);
            
        private:
            static const std::string TYPE;
            static const std::string PACKAGE;
            static const Version VERSION;
            
            OpenMode m_mode;
            std::string m_path;
            std::string m_extension;
        };     
        
        /** \cond */
        template <>
        class data_traits<File>
        {
        public:
            static const VariantHandle & variant() { return Variant::FILE; }
        };
        /** \endcond */
    }
}

#endif // STROMX_RUNTIME_FILE_H
