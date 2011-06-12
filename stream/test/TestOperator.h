#ifndef STREAM_TESTOPERATOR_H
#define STREAM_TESTOPERATOR_H

#include <stream/Operator.h>
#include <stream/Primitive.h>

namespace stream
{
    class TestOperator : public Operator
    {
    public:
        static const unsigned int INPUT_1 = 0;
        static const unsigned int INPUT_2 = 1;
        static const unsigned int OUTPUT_1 = 2;
        static const unsigned int OUTPUT_2 = 3;
        static const unsigned int SLEEP_TIME = 0;
        
        TestOperator();
        
        virtual void setParameter(unsigned int id, const Data& value);
        const Data& getParameter(unsigned int id);
        virtual void execute(DataProvider& provider);
        
        const unsigned int numExecutes() { return m_numExecutes; }
        
    private:
        static const std::vector<Description> setupInputs();
        static const std::vector<Description> setupOutputs();
        static const std::vector<Parameter> setupParameters();
        
        static const std::string NAME;
        static const std::string PACKAGE;
        static const Version VERSION;
        
        UInt32 m_sleepTime;
        unsigned int m_numExecutes;
    };
}


#endif // STREAM_TESTOPERATOR_H
