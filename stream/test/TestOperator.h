#ifndef STREAM_TESTOPERATOR_H
#define STREAM_TESTOPERATOR_H

#include <stream/OperatorKernel.h>
#include <stream/Primitive.h>

namespace stream
{
    class TestOperator : public OperatorKernel
    {
    public:
        static const unsigned int INPUT_1 = 0;
        static const unsigned int INPUT_2 = 1;
        static const unsigned int OUTPUT_1 = 2;
        static const unsigned int OUTPUT_2 = 3;
        static const unsigned int BUFFER_SIZE = 0;
        static const unsigned int SLEEP_TIME = 1;
        
        TestOperator();
        
        virtual OperatorKernel* const clone() const { return new TestOperator; }
        virtual void setParameter(unsigned int id, const Data& value);
        const Data& getParameter(unsigned int id);
        virtual void execute(DataProvider& provider);
        virtual void initialize();
        
        const unsigned int numExecutes() { return m_numExecutes; }
        
    private:
        static const std::vector<const Description*> setupInputs();
        static const std::vector<const Description*> setupOutputs();
        static const std::vector<const Parameter*> setupInitParameters();
        static const std::vector<const Parameter*> setupParameters();
        
        static const std::string NAME;
        static const std::string PACKAGE;
        static const Version VERSION;
        
        UInt32 m_sleepTime;
        UInt32 m_bufferSize;
        unsigned int m_numExecutes;
    };
}


#endif // STREAM_TESTOPERATOR_H
