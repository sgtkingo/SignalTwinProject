#ifndef LIBRARY_GUI_HPP
#define LIBRARY_GUI_HPP

#include "lvgl.h"

#include "gui_callbacks.hpp"
#include "../managers/device_manager.hpp"

class LibraryGui
{
private:
    DeviceManager &sensorManager;
    bool initialized = false;
    int selectedDeviceIndex = 0;

    lv_obj_t *ui_Widget = nullptr;
    lv_obj_t *ui_DeviceList = nullptr;
    lv_obj_t *ui_Detail = nullptr;
    lv_obj_t *ui_DetailLabel = nullptr;

    void build();
    void populateDeviceList();
    void updateDetail();

public:
    explicit LibraryGui(DeviceManager &sensorManager);
    ~LibraryGui() = default;

    void init();
    bool isInitialized() const { return initialized; }
    void showLibrary();
    void hideLibrary();
    void setSelectedDevice(int index);
};

#endif // LIBRARY_GUI_HPP
