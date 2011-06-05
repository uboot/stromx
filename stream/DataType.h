#ifndef STREAM_DATATYPE_H
#define STREAM_DATATYPE_H


namespace stream
{
    class DataType
    {
    public:
        enum Types
        {
            BOOL,
            INT_32,
            UINT_32
        };
        
        DataType(const unsigned int type = 0) : m_type(type) {}
        
        virtual const bool is(const DataType& type) const;
    
    private:
        
        unsigned int m_type;
    };
}

#endif // STREAM_DATATYPE_H