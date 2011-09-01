#include "Base.h"

#include <stream/Registry.h>
#include <stream/Exception.h>

#include "Image.h"
#include "Camera.h"

void registerBase(stream::Registry*const registry)
{
    if(! registry)
        throw stream::WrongArgument("Passed null pointer.");
    
    using namespace base;
    
    registry->registerData(new Image);
    registry->registerOperator(new Camera);
}
