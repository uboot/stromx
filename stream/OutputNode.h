#ifndef STREAM_OUTPUTNODE_H
#define STREAM_OUTPUTNODE_H

namespace stream
{
    class OperatorWrapper;
    
    class OutputNode
    {
    private:
        OperatorWrapper* m_operator;
    };
}

#endif // STREAM_OUTPUTNODE_H