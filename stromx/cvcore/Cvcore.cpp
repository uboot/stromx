#include "stromx/cvcore/Cvcore.h"

#include "stromx/cvcore/Absdiff.h"
#include "stromx/cvcore/Add.h"
#include "stromx/cvcore/AddWeighted.h"
#include "stromx/cvcore/Bitwise_and.h"
#include "stromx/cvcore/Bitwise_not.h"
#include "stromx/cvcore/Bitwise_or.h"
#include "stromx/cvcore/Bitwise_xor.h"
#include "stromx/cvcore/Mean.h"
#include "stromx/cvcore/Sum.h"
#include <stromx/runtime/Locale.h>
#include <stromx/runtime/Registry.h>

namespace stromx
{
    namespace cvcore
    {
        std::locale locale;
    }
}

int stromxRegisterCvcore(stromx::runtime::Registry* registry)
{
    using namespace stromx::cvcore;
    
    locale = stromx::runtime::Locale::generate(STROMX_CVCORE_LOCALE_DIR, STROMX_CVCORE_LOCALE_DOMAIN);
    
    try
    {
        registry->registerOperator(new Absdiff);
        registry->registerOperator(new Add);
        registry->registerOperator(new AddWeighted);
        registry->registerOperator(new Bitwise_and);
        registry->registerOperator(new Bitwise_not);
        registry->registerOperator(new Bitwise_or);
        registry->registerOperator(new Bitwise_xor);
        registry->registerOperator(new Mean);
        registry->registerOperator(new Sum);
    }
    catch(stromx::runtime::Exception & e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
