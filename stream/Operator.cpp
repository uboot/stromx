#include "Operator.h"

#include "Exception.h"

#include <set>
#include <boost/lexical_cast.hpp>

namespace stream
{
    Operator::Operator (const unsigned int id,
                        const std::string & name,
                        const Version & version,
                        const std::vector<Description>& inputs,
                        const std::vector<Description>& outputs,
                        const std::vector<Parameter>& parameters)
      : m_id(id),
        m_name(name),
        m_version(version),
        m_inputs(inputs),
        m_outputs(outputs),
        m_parameters(parameters)
    {
        validate(inputs);
        validate(outputs);
        validate(parameters);
    }
    
    Description& Operator::getParameterDescription(unsigned int id)
    {
        if(id >= m_parameters.size())
            throw WrongIdException("No parameter with ID " + id);
        
        return m_parameters[id];
    }
    
    void Operator::validate(const std::vector<Description>& descriptors)
    {
        std::set<unsigned int> ids;
        
        for(std::vector<Description>::const_iterator iter = descriptors.begin();
            iter != descriptors.end();
            ++iter)
        {
            if(ids.count(iter->id()))
                throw ArgumentException("ID " + boost::lexical_cast<std::string>(iter->id()) + " appears twice.");
        }
    }
    
    void Operator::validate(const std::vector<Parameter>& descriptors)
    {
        std::set<unsigned int> ids;
        
        for(std::vector<Parameter>::const_iterator iter = descriptors.begin();
            iter != descriptors.end();
            ++iter)
        {
            if(ids.count(iter->id()))
                throw ArgumentException("ID " + boost::lexical_cast<std::string>(iter->id()) + " appears twice.");
        }
    }
}
