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
    
    class ReferenceCountException : public Exception
    {
    public:
        ReferenceCountException(const std::string & message = "ReferenceCountException")
          : Exception(message)
        {}
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
    
    class IsDeactivatingException : public Exception
    {
    public:
        IsDeactivatingException(const std::string & message = "IsDeactivatingException")
          : Exception(message)
        {}
    };
}

#endif // STREAM_EXCEPTION_H
