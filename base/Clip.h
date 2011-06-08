#ifndef BASE_OPERATOR_CLIP_H
#define BASE_OPERATOR_CLIP_H

#include <stream/Operator.h>

namespace base
{
    class Clip : public stream::Operator
    {
    public:
        enum InputIds
        {
            INPUT
        };
        enum OutputIds
        {
            OUTPUT
        };
        
        enum ParameterIds
        {
            TOP,
            LEFT,
            WIDTH,
            HEIGHT
        };
        
        Clip();
        
        virtual void setParameter(unsigned int id, const stream::Data& value);
        virtual void getParameter(unsigned int id, stream::Data& value);
        virtual void execute(stream::DataProvider& provider);
        
    private:
        static const std::vector<stream::Description> setupInputs();
        static const std::vector<stream::Description> setupOutputs();
        static const std::vector<stream::Parameter> setupParameters();
        
        static const std::string NAME;
        static const std::string PACKAGE;
        static const stream::Version VERSION;
    };
}

#endif // BASE_OPERATOR_CLIP_H
