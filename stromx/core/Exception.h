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

#ifndef STROMX_CORE_EXCEPTION_H
#define STROMX_CORE_EXCEPTION_H

#include <exception>
#include <string>
#include "Config.h"

namespace stromx
{
    namespace core
    {
        class Data;
        
        /** \brief Abstract %core exception */
        class STROMX_CORE_API Exception : public std::exception
        {
        public:
            virtual ~Exception() throw() {}
            
            virtual const char* what() const throw() { return m_message.c_str(); }
            
        protected:
            Exception(const std::string & message) : m_message(message) {}
                        
        private:
            std::string m_message; 
        };  
        
        /** \brief A wrong argument was passed to a function */
        class STROMX_CORE_API WrongArgument : public Exception
        {
        public:
            WrongArgument(const std::string & message = "WrongArgument")
            : Exception(message)
            {}
        };  
        
        /** \brief An object was identified by a wrong ID */
        class STROMX_CORE_API WrongId : public WrongArgument
        {
        public:
            WrongId(const std::string & message = "WrongId")
            : WrongArgument(message)
            {}
        };  
        
        /** \brief The current state does not allow a specific operation */
        class STROMX_CORE_API WrongState : public Exception
        {
        public:
            WrongState(const std::string & message = "WrongState")
            : Exception(message)
            {}
        };
        
        /** \brief An internal, unexpected error occurred */
        class STROMX_CORE_API InternalError : public Exception
        {
        public:
            InternalError(const std::string & message = "InternalError")
            : Exception(message)
            {}
        };
        
        /** \brief The current thread was stopped */
        class STROMX_CORE_API Interrupt : public Exception
        {
        public:
            Interrupt(const std::string & message = "Interrupt")
            : Exception(message)
            {}
        };
        
        /** \brief An error occurred during file access */
        class STROMX_CORE_API FileAccessFailed : public Exception
        {
        public:
            FileAccessFailed(const std::string & filename, const std::string & message = "FileAccessFailed")
            : Exception(message),
                m_filename(filename)
            {}
            
        private:
            const std::string & m_filename;
        };
        
        /** \brief Not enought memory */
        class STROMX_CORE_API OutOfMemory : public Exception
        {
        public:
            OutOfMemory(const std::string & message = "OutOfMemory")
            : Exception(message)
            {}
        };
        
        /** \brief An operation timed out */
        class STROMX_CORE_API Timeout : public Exception
        {
        public:
            Timeout(const std::string & message = "Timeout")
            : Exception(message)
            {}
        };
        
        /** \brief An impossible cast was attempted */
        class STROMX_CORE_API BadCast : public Exception
        {
        public:
            BadCast(const std::string & message = "BadCast")
            : Exception(message)
            {}
        };
        
        /** \brief A function which is not implemented was called */
        class STROMX_CORE_API NotImplemented : public Exception
        {
        public:
            NotImplemented(const std::string & message = "NotImplemented")
              : Exception(message)
            {}
        };
        
        /** \brief %Data could not be serialized */
        class STROMX_CORE_API SerializationError : public Exception
        {
        public:
            SerializationError(const Data& data, const std::string & name, const std::string & path, const std::string & message = "SerializationError")
            : Exception(message),
                m_data(data),
                m_name(name),
                m_path(path)
            {}
            
        private:
            const Data& m_data;
            const std::string& m_name;
            const std::string& m_path;
        };
        
        /** \brief %Data could not be deserialized */
        class STROMX_CORE_API DeserializationError : public Exception
        {
        public:
            DeserializationError(const Data& data, const std::string & dataString, const std::string & path, const std::string & message = "DeserializationError")
            : Exception(message),
                m_data(data),
                m_dataString(dataString),
                m_path(path)
            {}
            
        private:
            const Data& m_data;
            const std::string& m_dataString;
            const std::string& m_path;
        };
    }
}

#endif // STROMX_CORE_EXCEPTION_H
