#include "stromx/cvobjdetect/Cvobjdetect.h"

#include "stromx/cvobjdetect/CascadeClassifier.h"
#include <stromx/runtime/Locale.h>
#include <stromx/runtime/Registry.h>

namespace stromx
{
    namespace cvobjdetect
    {
        std::locale locale;
    }
}

int stromxRegisterCvobjdetect(stromx::runtime::Registry* registry)
{
    using namespace stromx::cvobjdetect;
    
    locale = stromx::runtime::Locale::generate(STROMX_CVOBJDETECT_LOCALE_DIR, STROMX_CVOBJDETECT_LOCALE_DOMAIN);
    
    try
    {
        registry->registerOperator(new CascadeClassifier);
    }
    catch(stromx::runtime::Exception & e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
