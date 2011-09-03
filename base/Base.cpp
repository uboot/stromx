#include "Base.h"

#include <stream/Registry.h>
#include <stream/Exception.h>

#include "Image.h"
#include "AdjustRgbChannels.h"
#include "Camera.h"
#include "Clip.h"
#include "ConstImage.h"
#include "ConvertPixelType.h"
#include "Counter.h"
#include "PeriodicDelay.h"
#include "Queue.h"
#include "Trigger.h"

void registerBase(stream::Registry*const registry)
{
    if(! registry)
        throw stream::WrongArgument("Passed null pointer.");
    
    using namespace base;
    
    registry->registerData(new Image);
    
    registry->registerOperator(new AdjustRgbChannels);
    registry->registerOperator(new Camera);
    registry->registerOperator(new Clip);
    registry->registerOperator(new ConstImage);
    registry->registerOperator(new ConvertPixelType);
    registry->registerOperator(new Counter);
    registry->registerOperator(new PeriodicDelay);
    registry->registerOperator(new Queue);
    registry->registerOperator(new Trigger);
}
