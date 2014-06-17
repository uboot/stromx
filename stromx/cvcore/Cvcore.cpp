#include <boost/locale.hpp>

#include "stromx/cvcore/Cvcore.h"

#include "stromx/cvcore/Absdiff.h"
#include "stromx/cvcore/Add.h"
#include "stromx/cvcore/AddWeighted.h"
#include "stromx/cvcore/Bitwise_and.h"
#include "stromx/cvcore/Bitwise_not.h"
#include "stromx/cvcore/Bitwise_or.h"
#include "stromx/cvcore/Bitwise_xor.h"
#include <stromx/runtime/Registry.h>

namespace stromx
{
    namespace cvcore
    {
        std::locale locale;
    }
}

void stromxCvcoreRegister(stromx::runtime::Registry& registry)
{
    using namespace stromx::cvcore;
    
    boost::locale::generator gen;
    gen.add_messages_path(STROMX_CVCORE_LOCALE_DIR);
    gen.add_messages_domain(STROMX_CVCORE_LOCALE_DOMAIN);
    locale = gen.generate("");
    
    registry.registerOperator(new Absdiff);
    registry.registerOperator(new Add);
    registry.registerOperator(new AddWeighted);
    registry.registerOperator(new Bitwise_and);
    registry.registerOperator(new Bitwise_not);
    registry.registerOperator(new Bitwise_or);
    registry.registerOperator(new Bitwise_xor);
}
