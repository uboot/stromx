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
    
    class InvalidArgument : public Exception
    {
    public:
        InvalidArgument(const std::string & message = "InvalidArgument")
          : Exception(message)
        {}
    };  
    
    class WrongId : public InvalidArgument
    {
    public:
        WrongId(const std::string & message = "WrongId")
          : InvalidArgument(message)
        {}
    };  
    
    class InvalidState : public Exception
    {
    public:
        InvalidState(const std::string & message = "InvalidState")
          : Exception(message)
        {}
    };
    
    class InternalError : public Exception
    {
    public:
        InternalError(const std::string & message = "InternalError")
          : Exception(message)
        {}
    };
    
    class Interrupt : public Exception
    {
    public:
        Interrupt(const std::string & message = "Interrupt")
          : Exception(message)
        {}
    };
    
    class FileAccess : public Exception
    {
    public:
        FileAccess(const std::string & message = "FileAccess")
          : Exception(message)
        {}
    };
    
    class OutOfMemory : public Exception
    {
    public:
        OutOfMemory(const std::string & message = "OutOfMemory")
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
