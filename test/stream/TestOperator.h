#ifndef STREAM_TESTOPERATOR_H
#define STREAM_TESTOPERATOR_H

#include <stream/Operator.h>

namespace stream
{
    class TestOperator : public Operator
    {
    public:
        static const unsigned int INPUT_1 = 0;
        static const unsigned int INPUT_2 = 1;
        static const unsigned int OUTPUT_1 = 0;
        static const unsigned int OUTPUT_2 = 1;
        static const unsigned int SLEEP_TIME = 0;
        
        TestOperator(const unsigned int id);
        
        virtual void setParameter(unsigned int id, const Data& value);
        virtual void getParameter(unsigned int id, Data& value);
        virtual void execute(DataProvider& provider);
        
    private:
        static const std::vector<Descriptor> setupInputs();
        static const std::vector<Descriptor> setupOutputs();
        static const std::vector<Parameter> setupParameters();
        
        static const std::string NAME;
        static const Version VERSION;
        
        unsigned int m_sleepTime;
    };
}


#endif // STREAM_TESTOPERATOR_H
