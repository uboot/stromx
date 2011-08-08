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
    
    class WrongArgument : public Exception
    {
    public:
        WrongArgument(const std::string & message = "WrongArgument")
          : Exception(message)
        {}
    };  
    
    class WrongId : public WrongArgument
    {
    public:
        WrongId(const std::string & message = "WrongId")
          : WrongArgument(message)
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
    
    class FileAccessFailed : public Exception
    {
    public:
        FileAccessFailed(const std::string & message = "FileAccessFailed")
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
