#include "Operator.h"

#include "Exception.h"

#include <set>
#include <boost/lexical_cast.hpp>

namespace stream
{
    Operator::Operator (const unsigned int id,
                        const std::string & name,
                        const Version & version,
                        const std::vector<Descriptor>& inputs,
                        const std::vector<Descriptor>& outputs,
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
    
    void Operator::validate(const std::vector<Descriptor>& descriptors)
    {
        std::set<unsigned int> ids;
        
        for(std::vector<Descriptor>::const_iterator iter = descriptors.begin();
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
