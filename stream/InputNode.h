#ifndef STREAM_INPUTNODE_H
#define STREAM_INPUTNODE_H

namespace stream
{
    class OutputNode;
    
    class InputNode
    {
    public:
        const OutputNode& source() const { return *m_source; }
        void connect(OutputNode* const output);
        
    private:
        OutputNode *m_source;
    };
}

#endif // STREAM_INPUTNODE_H
