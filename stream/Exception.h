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
    
    class InterruptException : public Exception
    {
    public:
        InterruptException(const std::string & message = "InterruptException")
          : Exception(message)
        {}
    };
    
    class FileAccessException : public Exception
    {
    public:
        FileAccessException(const std::string & message = "FileAccessException")
          : Exception(message)
        {}
    };
    
    class OutOfMemoryException : public Exception
    {
    public:
        OutOfMemoryException(const std::string & message = "OutOfMemoryException")
          : Exception(message)
        {}
    };
    
    class Timeout : public Exception
    {
    public:
        Timeout(const std::string & message = "Timeout")
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
}

#endif // STREAM_EXCEPTION_H
