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

#ifndef STROMX_CORE_ZIPFILEOUTPUT_H
#define STROMX_CORE_ZIPFILEOUTPUT_H

#include <sstream>
#include <vector>
#include "Config.h"
#include "FileOutput.h"

struct zip;

namespace stromx
{
    namespace core
    {
        /** \brief File output which stores the output data in a zip file. */
        class STROMX_CORE_API ZipFileOutput : public FileOutput
        {
        public:
            /**
             * Constructs a file output which stores data in a zip file.
             * 
             * \param archive The path to the zip file.
             * \throws FileAccessFailed If the zip file could not be opened.
             */
            ZipFileOutput(const std::string & archive);
            virtual ~ZipFileOutput();
            
            /**
             * Closes the zip file. If this functions is not called the file is closed 
             * by the destructor.
             * 
             * \throws FileAccessFailed If the zip file could not be saved (e.g. due to
             *                          missing write permission).
             */
            virtual void close();
            
            virtual void initialize(const std::string & filename);
            virtual const std::string & getFilename() const;
            virtual const std::string getText() const;
            
            virtual std::ostream & text();
            virtual std::ostream & openFile(const std::string & ext, const OpenMode mode);
            virtual std::ostream & file();
            
        private:
            void dumpFile();
            
            zip* m_archiveHandle;
            bool m_initialized;
            std::string m_archive;
            std::string m_currentBasename;
            std::string m_currentFilename;
            std::ostringstream *m_currentFile;
            std::ostringstream m_currentText;
            std::vector<std::string> m_bufferedFiles;
        };
    }
}

#endif // STROMX_CORE_ZIPFILEOUTPUT_H
