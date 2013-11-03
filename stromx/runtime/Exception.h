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

#ifndef STROMX_RUNTIME_EXCEPTION_H
#define STROMX_RUNTIME_EXCEPTION_H

#include <exception>
#include <string>
#include "stromx/runtime/Config.h"

namespace stromx
{
    namespace runtime
    {
        class Data;
        
        /** \brief Abstract stromx exception. */
        class STROMX_RUNTIME_API Exception : public std::exception
        {
        public:
            /** Constructs an exception. */
            explicit Exception(const std::string & message);
            
            virtual ~Exception() throw() {}
            
            virtual const char* what() const throw() { return m_message.c_str(); }
            
            /** Returns the exception message. */
            virtual const std::string & message() const { return m_message; }
            
            /** Returns the call stack at the point where the exception was thrown. */
            virtual const std::string & callStack() const { return m_callStack; }
                        
        private:
            std::string m_message;
            std::string m_callStack;
        };  
        
        /** \brief A wrong argument was passed to a function. */
        class WrongArgument : public Exception
        {
        public:
            WrongArgument(const std::string & message = "WrongArgument")
              : Exception(message)
            {}
        };  
        
        /** \brief An object was identified by a wrong ID. */
        class WrongId : public WrongArgument
        {
        public:
            WrongId(const std::string & message = "WrongId")
              : WrongArgument(message)
            {}
        };  
        
        /** \brief The current state does not allow a specific operation. */
        class WrongState : public Exception
        {
        public:
            WrongState(const std::string & message = "WrongState")
              : Exception(message)
            {}
        };
        
        /** \brief An internal, unexpected error occurred. */
        class InternalError : public Exception
        {
        public:
            InternalError(const std::string & message = "InternalError")
              : Exception(message)
            {}
        };
        
        /** \brief The current thread was stopped. */
        class Interrupt : public Exception
        {
        public:
            Interrupt(const std::string & message = "Interrupt")
            : Exception(message)
            {}
        };
        
        /** \brief A file related exception. */
        class FileException : public Exception
        {
        public:
            FileException(const std::string & filename, const std::string & container, 
                          const std::string & message)
              : Exception(message),
                m_filename(filename),
                m_container(container)
            {}
            
            virtual ~FileException() throw() {}
            
            const std::string & filename() const { return m_filename; }
            const std::string & container() const { return m_container; }
            
            void setContainer(const std::string & container) { m_container = container; }
            
        private:
            const std::string m_filename;
            std::string m_container;
        };
        
        /** \brief An error occurred during file access. */
        class FileAccessFailed : public FileException
        {
        public:
            
            FileAccessFailed(const std::string & filename,
                             const std::string & container,
                             const std::string & message = "")
              : FileException(filename, container, message)
            {}
        };
        
        /** \brief The file format is invalid */
        class InvalidFileFormat : public FileException
        {
        public:
            InvalidFileFormat(const std::string & filename,
                             const std::string & message)
              : FileException(filename, "", message)
            {}
            
            InvalidFileFormat(const std::string & filename,
                             const std::string & container,
                             const std::string & message)
              : FileException(filename, container, message)
            {}
        };
        
        /** \brief The file content is not consistent */
        class InconsistentFileContent : public FileException
        {
        public:
            InconsistentFileContent(const std::string & filename,
                             const std::string & message)
              : FileException(filename, "", message)
            {}
            
            InconsistentFileContent(const std::string & filename,
                             const std::string & container,
                             const std::string & message)
              : FileException(filename, container, message)
            {}
        };
        
        /** \brief Not enought memory. */
        class OutOfMemory : public Exception
        {
        public:
            OutOfMemory(const std::string & message = "OutOfMemory")
              : Exception(message)
            {}
        };
        
        /** \brief An operation timed out. */
        class Timeout : public Exception
        {
        public:
            Timeout(const std::string & message = "Timeout")
              : Exception(message)
            {}
        };
        
        /** \brief An impossible cast was attempted. */
        class BadCast : public Exception
        {
        public:
            BadCast(const std::string & message = "BadCast")
              : Exception(message)
            {}
        };
        
        /** \brief A function which is not implemented was called. */
        class NotImplemented : public Exception
        {
        public:
            NotImplemented(const std::string & message = "NotImplemented")
              : Exception(message)
            {}
        };
        
        /** \brief Tried to get the data of an empty read or write access. */
        class AccessEmpty : public Exception
        {
        public:
            AccessEmpty(const std::string & message = "AccessEmpty")
              : Exception(message)
            {}
        };
        
        /** \brief Error related to a %Data object. */
        class DataException : public Exception
        {
        public:
            
            virtual ~DataException() throw() {}
            
            const std::string & package() const { return m_package; }
            const std::string & type() const { return m_package; }
            
        protected:
            DataException(const std::string & package, const std::string & type,
                               const std::string & message = "DataException")
              : Exception(message),
                m_package(package),
                m_type(type)
            {}
            
        private:
            const std::string m_package;
            const std::string m_type;
        };
        
        /** \brief %Data could not be serialized. */
        class SerializationError : public DataException
        {
        public:
            SerializationError(const std::string & package, const std::string & type,
                               const std::string & message = "SerializationError")
              : DataException(package, type, message)
            {}
        };
        
        /** \brief %Data could not be deserialized. */
        class DeserializationError : public DataException
        {
        public:
            DeserializationError(const std::string & package, const std::string & type,
                                 const std::string & message = "DeserializationError")
              : DataException(package, type, message)
            {}
        };
        
        /** \brief Error related to a data and operator factory. */
        class FactoryException : public Exception
        {
        public:
            
            virtual ~FactoryException() throw() {}
            
            const std::string & package() const { return m_package; }
            const std::string & type() const { return m_type; }
            
        protected:
            FactoryException(const std::string & package, const std::string & type,
                               const std::string & message = "SerializationError")
              : Exception(message),
                m_package(package),
                m_type(type)
            {}
            
        private:
            const std::string m_package;
            const std::string m_type;
        };
        
        /** \brief %Operator could not be allocated. */
        class OperatorAllocationFailed : public FactoryException
        {
        public:
            OperatorAllocationFailed(const std::string & package, const std::string & type,
                                     const std::string & message = "OperatorAllocationFailed")
              : FactoryException(package, type, message)
            {}
        };
        
        /** \brief %Data could not be allocated. */
        class DataAllocationFailed : public FactoryException
        {
        public:
            DataAllocationFailed(const std::string & package, const std::string & type,
                                 const std::string & message = "DataAllocationFailed")
              : FactoryException(package, type, message)
            {}
        };
        
        /** \brief The input provider has no input file. */
        class NoInputFile : public Exception
        {
        public:
            NoInputFile(const std::string & message = "NoInputFile")
              : Exception(message)
            {}
        };     
    }
}

#endif // STROMX_RUNTIME_EXCEPTION_H
