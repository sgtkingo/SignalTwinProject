#ifndef SENSOR_WIKI_GUI_HPP
#define SENSOR_WIKI_GUI_HPP

#include <string>

#include "lvgl.h"

#include "gui_callbacks.hpp"
#include "../managers/manager.hpp"

class SensorWikiGui
{
private:
    SensorManager &sensorManager;
    bool initialized = false;
    int selectedSensorIndex = 0;

    lv_obj_t *ui_WikiWidget = nullptr;
    lv_obj_t *ui_AvailableList = nullptr;
    lv_obj_t *ui_SelectedList = nullptr;
    lv_obj_t *ui_SensorTitle = nullptr;
    lv_obj_t *ui_SensorDescription = nullptr;
    lv_obj_t *ui_SensorSpecs = nullptr;
    lv_obj_t *ui_btnConfigure = nullptr;
    lv_obj_t *ui_btnRemove = nullptr;
    lv_obj_t *ui_btnStart = nullptr;
    lv_obj_t *ui_btnBack = nullptr;

    void buildWikiGui();
    void populateAvailableList();
    void populateSelectedList();
    void updateSensorInfo();
    void updateStartButtonState();
    void handleSensorSelection(int sensorIndex);
    void handleConfigureButtonClick();
    void handleRemoveButtonClick();
    void handleStartButtonClick();
    void handleBackButtonClick();
    std::string getSensorInfoText(BaseSensor *sensor);
    std::string getSensorSpecsText(BaseSensor *sensor);

public:
    explicit SensorWikiGui(SensorManager &sensorManager);
    ~SensorWikiGui() = default;

    void init();
    bool isInitialized() const { return initialized; }
    void showWiki(int pinIndex = -1);
    void hideWiki();
    BaseSensor *getSelectedSensor();
    void setSelectedSensor(int index);
};

#endif // SENSOR_WIKI_GUI_HPP
