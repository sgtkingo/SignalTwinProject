#ifndef GUI_RUNTIME_POLICY_HPP
#define GUI_RUNTIME_POLICY_HPP

#include "gui_state.hpp"
#include "../managers/device_manager.hpp"

class GuiRuntimePolicy
{
private:
    DeviceManager &deviceManager;

public:
    explicit GuiRuntimePolicy(DeviceManager &deviceManager);
    void apply(GuiState targetState);
};

#endif // GUI_RUNTIME_POLICY_HPP
