#include "Documentation.h"

namespace stromx
{
    namespace core
    {
        Documentation::Documentation(const std::string& title)
          : m_title(title)
        {
        }

        Documentation::Documentation(const std::string& title, const std::string& details)
          : m_title(title),
            m_details(details)
        {
        }
    }
}