#ifndef LIBRARY_GUI_HPP
#define LIBRARY_GUI_HPP

#include "lvgl.h"

#include "gui_callbacks.hpp"
#include "../managers/manager.hpp"

class LibraryGui
{
private:
    SensorManager &sensorManager;
    bool initialized = false;
    int selectedSensorIndex = 0;

    lv_obj_t *ui_Widget = nullptr;
    lv_obj_t *ui_SensorList = nullptr;
    lv_obj_t *ui_Detail = nullptr;
    lv_obj_t *ui_DetailLabel = nullptr;

    void build();
    void populateSensorList();
    void updateDetail();

public:
    explicit LibraryGui(SensorManager &sensorManager);
    ~LibraryGui() = default;

    void init();
    bool isInitialized() const { return initialized; }
    void showLibrary();
    void hideLibrary();
    void setSelectedSensor(int index);
};

#endif // LIBRARY_GUI_HPP
