#include "stromx/cvcore/Cvcore.h"

#include "stromx/cvcore/Absdiff.h"
#include "stromx/cvcore/Add.h"
#include "stromx/cvcore/AddWeighted.h"
#include "stromx/cvcore/Bitwise_and.h"
#include "stromx/cvcore/Bitwise_not.h"
#include "stromx/cvcore/Bitwise_or.h"
#include "stromx/cvcore/Bitwise_xor.h"
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
