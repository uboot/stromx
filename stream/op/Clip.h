#ifndef STREAM_OPERATOR_CLIP_H
#define STREAM_OPERATOR_CLIP_H

#include "../Operator.h"

namespace stream
{
    namespace op
    {
        class Clip : public Operator
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
            
            virtual void setParameter(unsigned int id, const Data& value);
            virtual void getParameter(unsigned int id, Data& value);
            virtual void execute(DataProvider& provider);
            
        private:
            static const std::vector<Description> setupInputs();
            static const std::vector<Description> setupOutputs();
            static const std::vector<Parameter> setupParameters();
            
            static const std::string NAME;
            static const Version VERSION;
        };
    }
}

#endif // STREAM_OPERATOR_CLIP_H
