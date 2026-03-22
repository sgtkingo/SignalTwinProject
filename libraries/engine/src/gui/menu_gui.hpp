#ifndef MENU_GUI_HPP
#define MENU_GUI_HPP

#include <array>

#include "lvgl.h"

#include "gui_callbacks.hpp"
#include "../managers/device_manager.hpp"

class MenuGui
{
private:
    DeviceManager &sensorManager;
    bool initialized = false;
    int activePinIndex = -1;

    lv_obj_t *ui_MenuWidget = nullptr;
    lv_obj_t *ui_PinScrollContainer = nullptr;
    lv_obj_t *ui_Title = nullptr;
    lv_obj_t *ui_Subtitle = nullptr;
    lv_obj_t *ui_btnBack = nullptr;
    lv_obj_t *ui_btnConnect = nullptr;
    std::array<lv_obj_t *, NUM_PINS> pinContainers;
    std::array<lv_obj_t *, NUM_PINS> pinLabels;

    void buildMenu();
    void updatePinVisualStates();
    void updateHeader();
    void handleConnectButtonClick();
    void handlePinClick(int pinIndex);
    bool isPinAllowedForCurrentDevice(int pinIndex) const;

public:
    explicit MenuGui(DeviceManager &sensorManager);
    ~MenuGui() = default;

    void init();
    bool isInitialized() const { return initialized; }
    void showMenu();
    void hideMenu();
    void updatePinLabels();
    void setActivePin(int pinIndex);
    int getActivePin() const { return activePinIndex; }
    void initializePins();
    uint32_t getPinStateColor(int pinIndex) const;
};

#endif // MENU_GUI_HPP
