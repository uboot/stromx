/* 
 *  Copyright 2011 Matthias Fuchs
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

#ifndef STROMX_RUNTIME_XMLREADER_H
#define STROMX_RUNTIME_XMLREADER_H

#include <string>
#include <vector>
#include "Config.h"

namespace stromx
{
    namespace runtime
    {
        class Factory;
        class FileInput;
        class Operator;
        class Stream;
        
        /** \brief Reader for \e stromx files.
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
         * %Stream and parameter files can also be read from \em file \em inputs. File inputs 
         * are an abstraction of the file bundles mentioned above and are represented by a
         * FileInput object. This allows the user to implement custom ways to store file bundles,
         * e.g. in a database or using a different archive format.
         */
        class STROMX_RUNTIME_API XmlReader
        {
        public:
            /** 
             * Reads a stream file. The file can be either an XML or a zip file.
             * 
             * \param filepath The path of the file to be read.
             * \param factory The factory is used to instantiate the operators and data
             *                objects in the stream. I.e. all required operator and data types
             *                must have been registered with the factory.
             * \throws DeserializationError Failed to deserialize data referenced in the XML file.
             * \throws FileException Failed to access the XML file or a data file.
             * \throws FactoryException Failed to allocate an operator or a data object.
             * \throws InconsistentFileContent The content of the file is inconsistent. This is probably due
             *                                 to a change of the file format.
             */
            Stream* readStream(const std::string & filepath, const Factory& factory) const;
            
            /** 
             * Reads a stream file from an file input.
             * 
             * \param input The file input which provides the stream files.
             * \param filename The name of the file in the input to be read.
             * \param factory The factory is used to instantiate the operators and data
             *                objects in the stream. I.e. all required operator and data types
             *                must have been registered with the factory.
             * \throws DeserializationError Failed to deserialize data referenced in the XML file.
             * \throws FileException Failed to access the XML file or a data file.
             * \throws FactoryException Failed to allocate an operator or a data object.
             * \throws InconsistentFileContent The content of the file is inconsistent. This is probably due
             *                                 to a change of the file format.
             */
            Stream* readStream(FileInput & input, const std::string filename, const Factory& factory) const;
            
            /** 
             * Reads a parameter file. The file can be either an XML or a zip file.
             * The functions sets the parameters of \c operators to the values in the file.
             * If a parameter can not be set the error is silently ignored.
             * In particular, only those operator parameters are set which can be written to 
             * in the current operator state.
             * 
             * \param filepath The path of the file to be read.
             * \param factory The factory is used to instantiate data objects in the stream.
             *                I.e. all required data types must have been registered with the factory.
             * \throws DeserializationError Failed to deserialize data referenced in the XML file.
             * \throws FileException Failed to access the XML file or a data file.
             * \throws FactoryException Failed to allocate an operator or a data object.
             * \throws InconsistentFileContent The content of the file is inconsistent. This is probably due
             *                                 to a change of the file format.
             */
            void readParameters(const std::string & filepath, const Factory& factory,
                                const std::vector<stromx::runtime::Operator*> & operators) const;
            
            
            /** 
             * Reads a parameter file from an file input.
             * The function sets the parameters of \c operators to the values in the file.
             * If a parameter can not be set the error is silently ignored. In particular, only
             * those operator parameters are set which can be written to in the current
             * operator state.
             * 
             * \param input The file input which provides the parameter files.
             * \param filename The name of the file in the input to be read.
             * \param factory The factory is used to instantiate data objects in the stream.
             *                I.e. all required data types must have been registered with the factory.
             * \throws FileException Failed to access the XML file or a data file.
             * \throws FactoryException Failed to allocate an operator or a data object.
             * \throws DeserializationError Failed to deserialize data referenced in the XML file.
             */
            void readParameters(FileInput & input, const std::string filename, const Factory& factory,
                                const std::vector<stromx::runtime::Operator*> & operators) const;
        };
    }
}

#endif // STROMX_RUNTIME_XMLREADER_H
