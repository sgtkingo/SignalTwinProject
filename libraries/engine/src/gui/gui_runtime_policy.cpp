#include "gui_runtime_policy.hpp"

GuiRuntimePolicy::GuiRuntimePolicy(DeviceManager &deviceManager)
    : deviceManager(deviceManager)
{
}

void GuiRuntimePolicy::apply(GuiState targetState)
{
    const bool shouldRunRuntime = targetState == GuiState::VISUALIZATION;
    deviceManager.setRunning(shouldRunRuntime);
}
