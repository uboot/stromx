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

#ifndef STROMX_CVSUPPORT_READDIRECTORY_H
#define STROMX_CVSUPPORT_READDIRECTORY_H

#include "stromx/cvsupport/Config.h"
#include <stromx/runtime/OperatorKernel.h>
#include <stromx/runtime/Enum.h>

namespace stromx
{
    namespace runtime
    {
        class DataContainer;
    }

    namespace cvsupport
    {
        /** \brief Applies a random coefficent to the brightness of the image. */
        class STROMX_CVSUPPORT_API ReadDirectory : public runtime::OperatorKernel
        {
            friend class ReadDirectoryTest;
            
        public:
            enum OutputId
            {
                OUTPUT
            };
            
            enum ParameterId
            {
                DIRECTORY
            };
            
            enum Directory
            {
                NO_DIRECTORY
            };
            
            ReadDirectory();
            
            virtual OperatorKernel* clone() const { return new ReadDirectory; }
            virtual void setParameter(const unsigned int id, const runtime::Data& value);
            virtual const runtime::DataRef getParameter(const unsigned int id) const;
            virtual void execute(runtime::DataProvider& provider);
            virtual void initialize();
            virtual void activate();
            
        private:
            static const std::string BASE_DIRECTORY;
            
            static const std::vector<const runtime::Description*> setupInputs();
            static const std::vector<const runtime::Description*> setupOutputs();
            const std::vector<const runtime::Parameter*> setupParameters();
            
            static const std::string TYPE;
            static const std::string PACKAGE;
            static const runtime::Version VERSION;                         
            
            runtime::Enum m_directory;
            std::map<std::size_t, std::string> m_directoryMap;
            std::vector<std::string> m_files;
            std::size_t m_currentIndex;
        };
    }
}

#endif // STROMX_CVSUPPORT_READDIRECTORY_H
