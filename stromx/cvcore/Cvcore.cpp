#include "Cvcore.h"

#include "Absdiff.h"
#include "Add.h"
#include "AddWeighted.h"
#include <stromx/runtime/Registry.h>

void stromxCvcoreRegister(stromx::runtime::Registry& registry)
{
    using namespace stromx::cvcore;
    
    registry.registerOperator(new Absdiff);
    registry.registerOperator(new Add);
    registry.registerOperator(new AddWeighted);
}
