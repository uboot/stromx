#include "Cvcore.h"

#include "Absdiff.h"
#include "Add.h"
#include "AddWeighted.h"
#include "Bitwise_and.h"
#include "Bitwise_not.h"
#include "Bitwise_or.h"
#include "Bitwise_xor.h"
#include <stromx/runtime/Registry.h>

void stromxCvcoreRegister(stromx::runtime::Registry& registry)
{
    using namespace stromx::cvcore;
    
    registry.registerOperator(new Absdiff);
    registry.registerOperator(new Add);
    registry.registerOperator(new AddWeighted);
    registry.registerOperator(new Bitwise_and);
    registry.registerOperator(new Bitwise_not);
    registry.registerOperator(new Bitwise_or);
    registry.registerOperator(new Bitwise_xor);
}
