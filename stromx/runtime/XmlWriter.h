/* 
*  Copyright 2011 Thomas Fidler
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

#ifndef STROMX_RUNTIME_XMLWRITER_H
#define STROMX_RUNTIME_XMLWRITER_H

#include <string>
#include <vector>
#include "stromx/runtime/Config.h"

namespace stromx
{
    namespace runtime
    {
        class FileOutput;
        class Operator;
        class Stream;
        
        /** \brief Writer for \em stromx files.
         * 
         * \em Stromx can read and write \em stream and \em parameter files. A stream 
         * file contains all the operators, connections and threads of a stream. In addition,
         * the settings of the parameters of each operator are stored.
         * A parameter file contains the parameter settings of a list of operator. It can
         * be used to set the parameters of an existing stream without changing its connections
         * or threads.
         * 
         * Both stream and parameter files are XML files. Additionally, they can refer to
         * \em dependend files which contain parameter information which should not be encoded
         * as XML (such as image data). Thus the stream and parameter XML files are actually 
         * part of file bundles which are either stored in a common directory or in a zip archive.
         * In the first case the path of the file is the path of XML file contained in the same
         * directory as the dependend files. In the second case it is the path of zip file which
         * contains the XML file and the dependend files. Zip files can also have the custom
         * extension <em>*.stromx</em>.
         * 
         * %Stream and parameter files can also be written to \em file \em outputs. File outputs 
         * are an abstraction of the file bundles mentioned above and are represented by a
         * FileOutput object. This allows the user to implement custom ways to store file bundles,
         * e.g. in a database or using a different archive format.
         */
        class STROMX_RUNTIME_API XmlWriter
        {
        public:
            /** 
             * Writes a stream file. The file can be either an XML or a zip file.
             * 
             * \param filepath The path of the file. Any existing file is overwritten.
             * \param stream The stream to write.
             * \throws FileException The base name of the file name is empty or the file extension
             *                       is unknown.
             * \throws SerializationError Failed to serialize data referenced in the XML file.
             */
            void writeStream(const std::string& filepath, const Stream& stream) const;
            
            /** 
             * Writes a stream file to a file output.
             * 
             * \param output The file output which receives the stream file.
             * \param basename The common first part of the name of all files which are written to the
             *                 file output.
             * \param stream The stream to write.
             * \throws FileException The base name of the file name is empty or the file extension
             *                       is unknown.
             * \throws SerializationError Failed to serialize data referenced in the XML file.
             */
            void writeStream(FileOutput & output, const std::string & basename, const Stream& stream) const;
                        
            /** 
             * Writes a parameter file.
             * 
             * \param filepath The path of the file. Any existing file is overwritten.
             * \param operators The operators whose parameter settings are written to the file.
             * \throws FileException The base name of the file name is empty or the file extension
             *                       is unknown.
             * \throws SerializationError Failed to serialize data referenced in the XML file.
             */
            void writeParameters(const std::string& filepath,
                                 const std::vector<const stromx::runtime::Operator*>& operators) const;
            /** 
             * Writes a parameter file.
             * 
             * \param filepath The path of the file. Any existing file is overwritten.
             * \param operators The operators whose parameter settings are written to the file.
             * \throws FileException The base name of the file name is empty or the file extension
             *                       is unknown.
             * \throws SerializationError Failed to serialize data referenced in the XML file.
             */
            void writeParameters(const std::string& filepath,
                                 const std::vector<stromx::runtime::Operator*>& operators) const;
                      
            /** 
             * Writes a parameter file to a file output
             * 
             * \param basename The common first part of the name of all files which are written to the
             *                 file output.
             * \param operators The operators whose parameter settings are written to the file.
             * \throws WrongArgument The base name of the file name is empty.
             * \throws SerializationError Failed to serialize data referenced in the XML file.
             */                  
            void writeParameters(FileOutput & output, const std::string & basename,
                                 const std::vector<const stromx::runtime::Operator*>& operators) const; 
            /** 
             * Writes a parameter file to a file output
             * 
             * \param basename The common first part of the name of all files which are written to the
             *                 file output.
             * \param operators The operators whose parameter settings are written to the file.
             * \throws WrongArgument The base name of the file name is empty.
             * \throws SerializationError Failed to serialize data referenced in the XML file.
             */
            void writeParameters(FileOutput & output, const std::string & basename,
                                 const std::vector<stromx::runtime::Operator*>& operators) const; 
        };
    }
}

#endif // STROMX_RUNTIME_XMLWRITER_H
