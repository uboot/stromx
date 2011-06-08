#include "Clip.h"



namespace stream
{
    namespace op
    {
        const std::string Clip::NAME("Clip");
        const std::string Clip::PACKAGE("Base");
        const Version Clip::VERSION(0, 1);
        
        Clip::Clip()
        : Operator(NAME, PACKAGE, VERSION, setupInputs(), setupOutputs(), setupParameters())
        {
        }

        void Clip::setParameter(unsigned int id, const stream::Data& value)
        {
        }

        void Clip::getParameter(unsigned int id, Data& value)
        {
        }  
        
        void Clip::execute(DataProvider& provider)
        {
        }

        
        const std::vector< Description > Clip::setupInputs()
        {
            std::vector<Description> inputs;
            
            return inputs;
        }
        
        const std::vector< Description > Clip::setupOutputs()
        {
            std::vector<Description> outputs;
            
            return outputs;
        }
        
        const std::vector< Parameter > Clip::setupParameters()
        {
            std::vector<Parameter> parameters;
                                        
            return parameters;
        }
    } 
}
