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

#ifndef STROMX_CORE_XMLREADER_H
#define STROMX_CORE_XMLREADER_H

#include <string>
#include <vector>
#include "Config.h"

namespace stromx
{
    namespace core
    {
        class Factory;
        class FileInput;
        class Operator;
        class Stream;
        
        /** \brief Reader for XML encoded streams. */
        class STROMX_CORE_API XmlReader
        {
        public:
            /** 
             * Reads a stream from an XML file.
             * 
             * \param filepath The path of the file to be read.
             * \param factory The factory is used to instantiate the operators and data
             *                objects in the stream. I.e. all required operator and data types
             *                must have been registered with the factory.
             * \throws FileException Failed to access the XML file or a data file.
             * \throws FactoryException Failed to allocate an operator or a data object.
             * \throws DeserializationError Failed to deserialize data referenced in the XML file.
             */
            Stream* const readStream(const std::string & filepath, const Factory& factory) const;
            
            /** 
             * Reads a stream from an file input.
             * 
             * \param input The file input which provides the stream files.
             * \param filename The name of the file in the input to be read.
             * \param factory The factory is used to instantiate the operators and data
             *                objects in the stream. I.e. all required operator and data types
             *                must have been registered with the factory.
             * \throws FileException Failed to access the XML file or a data file.
             * \throws FactoryException Failed to allocate an operator or a data object.
             * \throws DeserializationError Failed to deserialize data referenced in the XML file.
             */
            Stream* const readStream(FileInput & input, const std::string filename, const Factory& factory) const;
            
            /** 
             * Sets the parameters of \c operators to the values in the XML file \c filepath.
             * If a parameter can not be set the error is silently ignored. In particular, only
             * those operator parameters are set which can be written to in the current operator state.
             * 
             * \param filepath The path of the file to be read.
             * \param factory The factory is used to instantiate data objects in the stream.
             *                I.e. all required data types must have been registered with the factory.
             * \throws FileException Failed to access the XML file or a data file.
             * \throws FactoryException Failed to allocate an operator or a data object.
             * \throws DeserializationError Failed to deserialize data referenced in the XML file.
             */
            void readParameters(const std::string & filepath, const Factory& factory,
                                const std::vector<stromx::core::Operator*> & operators) const;
            
            
            /** 
             * Sets the parameters of \c operators to the values in the XML file \c filename.
             * If a parameter can not be set the error is silently ignored. In particular, only
             * those operator parameters are set which can be written to in the current operator state.
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
                                const std::vector<stromx::core::Operator*> & operators) const;
        };
    }
}

#endif // STROMX_CORE_XMLREADER_H
