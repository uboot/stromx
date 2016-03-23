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
            
            /** 
             * Returns the path to the directory where temporary files are stored.
             */
            static const std::string & tempDir();
            
            /** 
             * Sets the path to the directory where temporary files are stored. Pass
             * an empty string to reset the temporary directory.
             */
            static void setTempDir(const std::string dir);
            
            /** 
             * Returns a path to an unique temporary file ending with
             * \c extension.
             */
            static const std::string tempPath(const std::string & extension = "");
            
            /** Creates an empty file, i.e. a file object with no path. */
            File() : m_mode(TEXT) {}
            
            /** 
             * Create a file object which represents the file at \c path.
             * 
             * \throw FileAccessFailed If \c path does not exist or if \c path is not 
             *                         a file.
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
            
            /**
             * Sets the extension of a file with no path. Use this e.g. to
             * indicate the file type of a parameter.
             * 
             * \throw WrongState If the path of this file object is set, i.e.
             *                   the extension is determined by the file path.
             */
            void setExtension(const std::string & extension);
            
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
            
            static std::string staticTempDir;
            
            OpenMode m_mode;
            std::string m_path;
            std::string m_extension;
        };     
        
        /** \cond */
        template <>
        class STROMX_RUNTIME_API data_traits<File>
        {
        public:
            static const VariantHandle & variant();
        };
        /** \endcond */
    }
}

#endif // STROMX_RUNTIME_FILE_H
