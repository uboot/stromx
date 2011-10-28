#include "Base.h"

#include <stromx/core/Registry.h>
#include <stromx/core/Exception.h>

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

void registerBase(core::Registry& registry)
{
    using namespace base;
    
    registry.registerData(new Image);
    
    registry.registerOperator(new AdjustRgbChannels);
    registry.registerOperator(new Camera);
    registry.registerOperator(new Clip);
    registry.registerOperator(new ConstImage);
    registry.registerOperator(new ConvertPixelType);
    registry.registerOperator(new Counter);
    registry.registerOperator(new PeriodicDelay);
    registry.registerOperator(new Queue);
    registry.registerOperator(new Trigger);
}
