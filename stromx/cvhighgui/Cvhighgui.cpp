#include "stromx/cvhighgui/Cvhighgui.h"

#include "stromx/cvhighgui/VideoCapture.h"
#include <stromx/runtime/Registry.h>

void stromxCvhighguiRegister(stromx::runtime::Registry& registry)
{
    using namespace stromx::cvhighgui;
    
    registry.registerOperator(new VideoCapture);
}
