#include "Cvcore.h"

#include "Add.h"
#include <stromx/runtime/Registry.h>

void stromxRegisterCvcore(stromx::runtime::Registry& registry)
{
    using namespace stromx::cvcore;
    
    registry.registerOperator(new Add);
}
