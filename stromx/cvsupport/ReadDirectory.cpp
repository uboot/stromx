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

#include "stromx/cvsupport/ReadDirectory.h"

#include "stromx/cvsupport/Image.h"
#include "stromx/cvsupport/Locale.h"
#include "stromx/cvsupport/Utilities.h"
#include <stromx/runtime/DataContainer.h>
#include <stromx/runtime/DataProvider.h>
#include <stromx/runtime/EnumParameter.h>
#include <stromx/runtime/Id2DataPair.h>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/Primitive.h>
#include <stromx/runtime/WriteAccess.h>

#include <boost/filesystem.hpp>

namespace stromx
{
    using namespace runtime;

    namespace cvsupport
    {
        const std::string ReadDirectory::BASE_DIRECTORY = "/usr/share/stromx-web/data";
        
        const std::string ReadDirectory::TYPE("ReadDirectory");
        const std::string ReadDirectory::PACKAGE(STROMX_CVSUPPORT_PACKAGE_NAME);
        const Version ReadDirectory::VERSION(0, 1, 0);
        
        ReadDirectory::ReadDirectory()
          : OperatorKernel(TYPE, PACKAGE, VERSION),
            m_currentIndex(0)
        {
        }
        
        void ReadDirectory::initialize()
        {
            OperatorKernel::initialize(setupInputs(), setupOutputs(), setupParameters());
        }
        
        void ReadDirectory::activate()
        {
            if (m_directory == NO_DIRECTORY)
                throw OperatorError(*this, "No valid directory selected.");
                
            std::string directory = m_directoryMap[std::size_t(m_directory)];
            
            boost::filesystem::path path(BASE_DIRECTORY);
            path /= boost::filesystem::path(directory);
            
            m_currentIndex = 0;
            m_files.clear();
            if (boost::filesystem::exists(path))
            {
                if (boost::filesystem::is_directory(path))
                {
                    for(boost::filesystem::directory_iterator iter(path);
                        iter != boost::filesystem::directory_iterator();
                        ++iter)
                    {
                        if (! boost::filesystem::is_regular_file(iter->path()))
                            continue;
                            
                        std::string file = iter->path().string();
                        m_files.push_back(file);
                    }
                    
                    std::sort(m_files.begin(), m_files.end());
                }
            }
        }

        void ReadDirectory::setParameter(unsigned int id, const Data& value)
        {
            Enum enumValue;
            try
            {
                switch(id)
                {
                case DIRECTORY:
                    enumValue = data_cast<Enum>(value);
                    
                    if (m_directory != NO_DIRECTORY)
                    {
                        if(m_directoryMap.count(std::size_t(enumValue)) == 0)
                            throw WrongParameterValue(parameter(id), *this);
                    }
                    m_directory = enumValue;
                    break;
                default:
                    throw WrongParameterId(id, *this);
                }
            }
            catch(std::bad_cast&)
            {
                throw WrongParameterType(parameter(id), *this);
            }
        }

        const DataRef ReadDirectory::getParameter(const unsigned int id) const
        {
            switch(id)
            {
            case DIRECTORY:
                return m_directory;
            default:
                throw WrongParameterId(id, *this);
            }
        }  
        
        void ReadDirectory::execute(DataProvider& provider)
        {
            if (m_files.size() == 0)
                throw OperatorError(*this, "Directory is empty.");
                
            Data* data = 0;
            std::size_t index = m_currentIndex;
            do
            {
                boost::filesystem::path file(m_files[index]);
                index = (index + 1) % m_files.size();
                std::string ext = file.extension().string();
                std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
                if (ext == ".png" || ext == ".jpg" || ext == ".jpeg")
                {
                    data = new cvsupport::Image(file.string());
                }
            }
            while (data == 0 && index != m_currentIndex);
            
            m_currentIndex = index;
            
            if (data == 0)
                throw OperatorError(*this, "Found no usable file in selected directory.");
            
            DataContainer container(data);
            Id2DataPair outputDataMapper(OUTPUT, container);
            provider.sendOutputData(outputDataMapper);
        }
        
        const std::vector<const runtime::Description*> ReadDirectory::setupInputs()
        {
            return std::vector<const Description*>();
        }
        
        const std::vector<const Description*> ReadDirectory::setupOutputs()
        {
            std::vector<const Description*> outputs;
            
            Description* output = new Description(OUTPUT, Variant::DATA);
            output->setTitle(L_("Output"));
            outputs.push_back(output);
            
            return outputs;
        }
        
        const std::vector<const Parameter*> ReadDirectory::setupParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            EnumParameter* directory = new EnumParameter(DIRECTORY);
            directory->setTitle(L_("Directory"));
            directory->setAccessMode(runtime::Parameter::INITIALIZED_WRITE);
            parameters.push_back(directory);
            
            directory->add(EnumDescription(Enum(NO_DIRECTORY), L_("None")));
            boost::filesystem::path path (BASE_DIRECTORY);
            m_directoryMap.clear();
            std::vector<std::string> dirNames;
            if (boost::filesystem::exists(path))
            {
                if (boost::filesystem::is_directory(path))
                {
                    for(boost::filesystem::directory_iterator iter(path);
                        iter != boost::filesystem::directory_iterator();
                        ++iter)
                    {
                        if (! boost::filesystem::is_directory(iter->path()))
                            continue;
                        
                        dirNames.push_back(iter->path().filename().string());
                    }
                }
            }
            
            std::sort(dirNames.begin(), dirNames.end());
            std::size_t i = NO_DIRECTORY + 1;
            for (std::vector<std::string>::const_iterator iter = dirNames.begin();
                 iter != dirNames.end(); ++iter)
            {
                directory->add(EnumDescription(Enum(i), *iter));
                m_directoryMap[i] = *iter;
                i++;
            }
                                        
            return parameters;
        }
    } 
}
