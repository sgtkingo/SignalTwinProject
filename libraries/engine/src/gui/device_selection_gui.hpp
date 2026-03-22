#ifndef DEVICE_SELECTION_GUI_HPP
#define DEVICE_SELECTION_GUI_HPP

#include <string>

#include "lvgl.h"

#include "device_catalog_browser.hpp"
#include "gui_callbacks.hpp"
#include "../managers/device_browser_state.hpp"
#include "../managers/device_catalog.hpp"
#include "../managers/device_manager.hpp"
#include "../managers/device_visualization_session.hpp"

class DeviceSelectionGui
{
private:
    DeviceCatalogBrowserModel catalogBrowser;
    DeviceBrowserState &browserState;
    DeviceManager &sensorManager;
    DeviceVisualizationSession &visualizationSession;
    bool initialized = false;

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

public:
    explicit DeviceSelectionGui(DeviceCatalog &deviceCatalog, DeviceBrowserState &browserState, DeviceManager &sensorManager, DeviceVisualizationSession &visualizationSession);
    ~DeviceSelectionGui() = default;

    void init();
    bool isInitialized() const { return initialized; }
    void showSelection();
    void hideSelection();
    BaseDevice *getSelectedDevice();
    void setSelectedDevice(int index);
};

#endif // DEVICE_SELECTION_GUI_HPP
