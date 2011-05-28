#ifndef STREAM_OUTPUTNODE_H
#define STREAM_OUTPUTNODE_H

#include <boost/thread/mutex.hpp>

namespace stream
{
    class OperatorInterface;
    class DataContainer;
    
    class OutputNode
    {
    public:
        OutputNode(OperatorInterface* const op, const unsigned int outputId);
        
        DataContainer* const getOutputData();
        void incrementConnectedInputs();
        void decrementConnectedInputs();
        
    private:
        typedef boost::lock_guard<boost::mutex> lock_t;
        
        OperatorInterface* m_operator;
        unsigned int m_outputId;
        unsigned int m_connectedInputs;
        unsigned int m_remainingCopies;
        
        boost::mutex m_mutex;  
    };
}

#endif // STREAM_OUTPUTNODE_H