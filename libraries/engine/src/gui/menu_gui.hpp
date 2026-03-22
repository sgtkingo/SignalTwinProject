#ifndef MENU_GUI_HPP
#define MENU_GUI_HPP

#include <array>

#include "lvgl.h"

#include "gui_router.hpp"
#include "../managers/device_browser_state.hpp"
#include "../managers/device_manager.hpp"

class ConnectionGui
{
private:
    DeviceBrowserState &browserState;
    GuiRouter &router;
    DeviceManager &deviceManager;
    bool initialized = false;

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
    explicit ConnectionGui(DeviceBrowserState &browserState, GuiRouter &router, DeviceManager &deviceManager);
    ~ConnectionGui() = default;

    void init();
    bool isInitialized() const { return initialized; }
    void showConnection();
    void hideConnection();
    void showMenu() { showConnection(); }
    void hideMenu() { hideConnection(); }
    void updatePinLabels();
    void initializePins();
    uint32_t getPinStateColor(int pinIndex) const;
};

using MenuGui = ConnectionGui;

#endif // MENU_GUI_HPP
