#include "Strom.h"

#include "Registry.h"
#include "Primitive.h"
#include "Enum.h"
#include "None.h"
#include "Exception.h"

void registerStrom(strom::Registry*const registry)
{
    if(! registry)
        throw strom::WrongArgument("Passed null pointer.");
    
    using namespace strom;
    
    registry->registerData(new Bool);
    
    registry->registerData(new Int8);
    registry->registerData(new UInt8);
    
    registry->registerData(new Int16);
    registry->registerData(new UInt16);
    
    registry->registerData(new Int32);
    registry->registerData(new UInt32);
    
    registry->registerData(new Double);
    
    registry->registerData(new Enum);
    
    registry->registerData(new None);
}
