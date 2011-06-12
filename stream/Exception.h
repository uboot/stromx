#ifndef STREAM_EXCEPTION_H
#define STREAM_EXCEPTION_H

#include <string>
#include <exception>

namespace stream
{
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
    
    class OwnershipException : public Exception
    {
    public:
        OwnershipException(const std::string & message = "OwnershipException")
          : Exception(message)
        {}
    };  
    
    class ArgumentException : public Exception
    {
    public:
        ArgumentException(const std::string & message = "ArgumentException")
          : Exception(message)
        {}
    };  
    
    class WrongIdException : public Exception
    {
    public:
        WrongIdException(const std::string & message = "WrongIdException")
          : Exception(message)
        {}
    };  
    
    class InvalidStateException : public Exception
    {
    public:
        InvalidStateException(const std::string & message = "InvalidStateException")
          : Exception(message)
        {}
    };
    
    class InternalException : public Exception
    {
    public:
        InternalException(const std::string & message = "InternalException")
          : Exception(message)
        {}
    };
    
    class InterruptException : public InvalidStateException
    {
    public:
        InterruptException(const std::string & message = "InterruptException")
          : InvalidStateException(message)
        {}
    };
    
    class FileAccessException : public Exception
    {
    public:
        FileAccessException(const std::string & message = "FileAccessException")
          : Exception(message)
        {}
    };
}

#endif // STREAM_EXCEPTION_H
