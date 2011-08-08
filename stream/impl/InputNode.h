#ifndef STREAM_INPUTNODE_H
#define STREAM_INPUTNODE_H

namespace stream
{
    class OutputNode;
    class DataContainer;
    class Operator;
    
    class InputNode
    {
    public:
        InputNode(Operator* const op, const unsigned int inputId);
        
        const unsigned int inputId() const { return m_inputId; }
        const Operator* const op() { return m_operator; }
        const bool isConnected() const { return m_source == 0; }
        const OutputNode& source() const { return *m_source; }
        void connect(OutputNode* const output);
        void disconnect();
        void setInputData();
        
    private:
        OutputNode *m_source;
        unsigned int m_inputId;
        Operator* m_operator;
    };
}

#endif // STREAM_INPUTNODE_H
