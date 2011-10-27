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

#ifndef STROM_EXCEPTION_H
#define STROM_EXCEPTION_H

#include <exception>
#include <string>

namespace strom
{
    class Data;
    
    /** \brief Base class of all %strom exceptions */
    class Exception : public std::exception
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
    class WrongArgument : public Exception
    {
    public:
        WrongArgument(const std::string & message = "WrongArgument")
          : Exception(message)
        {}
    };  
    
    /** \brief An object was identified by a wrong ID */
    class WrongId : public WrongArgument
    {
    public:
        WrongId(const std::string & message = "WrongId")
          : WrongArgument(message)
        {}
    };  
    
    /** \brief The current state does not allow a specific operation */
    class InvalidState : public Exception
    {
    public:
        InvalidState(const std::string & message = "InvalidState")
          : Exception(message)
        {}
    };
    
    /** \brief An internal, unexpected error occurred */
    class InternalError : public Exception
    {
    public:
        InternalError(const std::string & message = "InternalError")
          : Exception(message)
        {}
    };
    
    /** \brief The current thread was stopped */
    class Interrupt : public Exception
    {
    public:
        Interrupt(const std::string & message = "Interrupt")
          : Exception(message)
        {}
    };
    
    /** \brief An error occurred during file access */
    class FileAccessFailed : public Exception
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
    class OutOfMemory : public Exception
    {
    public:
        OutOfMemory(const std::string & message = "OutOfMemory")
          : Exception(message)
        {}
    };
    
    /** \brief An operation timed out */
    class Timeout : public Exception
    {
    public:
        Timeout(const std::string & message = "Timeout")
          : Exception(message)
        {}
    };
    
    /** \brief An impossible cast was attempted */
    class BadCast : public Exception
    {
    public:
        BadCast(const std::string & message = "BadCast")
          : Exception(message)
        {}
    };
    
    class WrongState : public Exception
    {
    public:
        WrongState(const std::string & message = "WrongState")
          : Exception(message)
        {}
    };
    
    class SerializationError : public Exception
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
    
    class DeserializationError : public Exception
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

#endif // STROM_EXCEPTION_H
