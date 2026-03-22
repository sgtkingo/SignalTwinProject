#ifndef DEVICE_SELECTION_GUI_HPP
#define DEVICE_SELECTION_GUI_HPP

#include <string>

#include "lvgl.h"

#include "gui_callbacks.hpp"
#include "../managers/device_manager.hpp"

class DeviceSelectionGui
{
private:
    DeviceManager &sensorManager;
    bool initialized = false;
    int selectedDeviceIndex = 0;

    lv_obj_t *ui_SelectionWidget = nullptr;
    lv_obj_t *ui_AvailableList = nullptr;
    lv_obj_t *ui_SelectedList = nullptr;
    lv_obj_t *ui_DeviceTitle = nullptr;
    lv_obj_t *ui_DeviceDescription = nullptr;
    lv_obj_t *ui_DeviceSpecs = nullptr;
    lv_obj_t *ui_btnConfigure = nullptr;
    lv_obj_t *ui_btnRemove = nullptr;
    lv_obj_t *ui_btnStart = nullptr;
    lv_obj_t *ui_btnBack = nullptr;

    void buildSelectionGui();
    void populateAvailableList();
    void populateSelectedList();
    void updateDeviceInfo();
    void updateStartButtonState();
    void handleDeviceSelection(int deviceIndex);
    void handleConfigureButtonClick();
    void handleRemoveButtonClick();
    void handleStartButtonClick();
    void handleBackButtonClick();
    std::string getDeviceInfoText(BaseDevice *sensor);
    std::string getDeviceSpecsText(BaseDevice *sensor);

public:
    explicit DeviceSelectionGui(DeviceManager &sensorManager);
    ~DeviceSelectionGui() = default;

    void init();
    bool isInitialized() const { return initialized; }
    void showSelection(int pinIndex = -1);
    void hideSelection();
    BaseDevice *getSelectedDevice();
    void setSelectedDevice(int index);
};

#endif // DEVICE_SELECTION_GUI_HPP
