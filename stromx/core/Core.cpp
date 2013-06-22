#include "Core.h"

#include "Add.h"
#include <stromx/runtime/Registry.h>

void stromxRegisterCore(stromx::runtime::Registry& registry)
{
    using namespace stromx::core;
    
    registry.registerOperator(new Add);
}
