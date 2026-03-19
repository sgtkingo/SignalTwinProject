#include "sensor_wiki_gui.hpp"

#include "../helpers.hpp"

SensorWikiGui::SensorWikiGui(SensorManager &sensorManager) : sensorManager(sensorManager)
{
}

void SensorWikiGui::buildWikiGui()
{
    ui_WikiWidget = lv_obj_create(lv_scr_act());
    lv_obj_remove_style_all(ui_WikiWidget);
    lv_obj_set_size(ui_WikiWidget, 760, 440);
    lv_obj_set_align(ui_WikiWidget, LV_ALIGN_CENTER);
    lv_obj_set_style_radius(ui_WikiWidget, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_WikiWidget, lv_color_hex(0xF5F5F5), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_WikiWidget, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_WikiWidget, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_flag(ui_WikiWidget, LV_OBJ_FLAG_HIDDEN);

    lv_obj_t *title = lv_label_create(ui_WikiWidget);
    lv_label_set_text(title, "Selection");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 12);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_AvailableList = lv_list_create(ui_WikiWidget);
    lv_obj_set_size(ui_AvailableList, 210, 285);
    lv_obj_set_pos(ui_AvailableList, 10, 50);

    lv_obj_t *infoPanel = lv_obj_create(ui_WikiWidget);
    lv_obj_set_size(infoPanel, 300, 285);
    lv_obj_set_pos(infoPanel, 230, 50);

    ui_SensorTitle = lv_label_create(infoPanel);
    lv_obj_set_width(ui_SensorTitle, 270);
    lv_obj_set_pos(ui_SensorTitle, 10, 10);
    lv_obj_set_style_text_font(ui_SensorTitle, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_SensorDescription = lv_label_create(infoPanel);
    lv_obj_set_width(ui_SensorDescription, 270);
    lv_obj_set_pos(ui_SensorDescription, 10, 50);
    lv_label_set_long_mode(ui_SensorDescription, LV_LABEL_LONG_WRAP);

    ui_SensorSpecs = lv_label_create(infoPanel);
    lv_obj_set_width(ui_SensorSpecs, 270);
    lv_obj_set_pos(ui_SensorSpecs, 10, 150);
    lv_label_set_long_mode(ui_SensorSpecs, LV_LABEL_LONG_WRAP);

    ui_SelectedList = lv_list_create(ui_WikiWidget);
    lv_obj_set_size(ui_SelectedList, 210, 285);
    lv_obj_set_pos(ui_SelectedList, 540, 50);

    ui_btnBack = lv_btn_create(ui_WikiWidget);
    lv_obj_set_size(ui_btnBack, 90, 36);
    lv_obj_set_pos(ui_btnBack, 12, 388);
    lv_obj_add_event_cb(ui_btnBack, [](lv_event_t *e) {
        if (lv_event_get_code(e) != LV_EVENT_CLICKED) {
            return;
        }
        auto *self = static_cast<SensorWikiGui *>(lv_event_get_user_data(e));
        self->handleBackButtonClick();
    }, LV_EVENT_ALL, this);
    lv_obj_t *backLabel = lv_label_create(ui_btnBack);
    lv_label_set_text(backLabel, "Back");
    lv_obj_center(backLabel);

    ui_btnConfigure = lv_btn_create(ui_WikiWidget);
    lv_obj_set_size(ui_btnConfigure, 145, 36);
    lv_obj_set_pos(ui_btnConfigure, 315, 344);
    lv_obj_add_event_cb(ui_btnConfigure, [](lv_event_t *e) {
        if (lv_event_get_code(e) != LV_EVENT_CLICKED) {
            return;
        }
        auto *self = static_cast<SensorWikiGui *>(lv_event_get_user_data(e));
        self->handleConfigureButtonClick();
    }, LV_EVENT_ALL, this);
    lv_obj_t *configureLabel = lv_label_create(ui_btnConfigure);
    lv_label_set_text(configureLabel, "Configure");
    lv_obj_center(configureLabel);

    ui_btnRemove = lv_btn_create(ui_WikiWidget);
    lv_obj_set_size(ui_btnRemove, 145, 36);
    lv_obj_set_pos(ui_btnRemove, 315, 388);
    lv_obj_set_style_bg_color(ui_btnRemove, lv_color_hex(0xD96464), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(ui_btnRemove, [](lv_event_t *e) {
        if (lv_event_get_code(e) != LV_EVENT_CLICKED) {
            return;
        }
        auto *self = static_cast<SensorWikiGui *>(lv_event_get_user_data(e));
        self->handleRemoveButtonClick();
    }, LV_EVENT_ALL, this);
    lv_obj_t *removeLabel = lv_label_create(ui_btnRemove);
    lv_label_set_text(removeLabel, "Remove");
    lv_obj_center(removeLabel);

    ui_btnStart = lv_btn_create(ui_WikiWidget);
    lv_obj_set_size(ui_btnStart, 145, 36);
    lv_obj_set_pos(ui_btnStart, 605, 388);
    lv_obj_add_event_cb(ui_btnStart, [](lv_event_t *e) {
        if (lv_event_get_code(e) != LV_EVENT_CLICKED) {
            return;
        }
        auto *self = static_cast<SensorWikiGui *>(lv_event_get_user_data(e));
        self->handleStartButtonClick();
    }, LV_EVENT_ALL, this);
    lv_obj_t *startLabel = lv_label_create(ui_btnStart);
    lv_label_set_text(startLabel, "Start Visualization");
    lv_obj_center(startLabel);
}

void SensorWikiGui::populateAvailableList()
{
    if (!ui_AvailableList) {
        return;
    }

    lv_obj_clean(ui_AvailableList);
    const auto &sensors = sensorManager.getSensors();
    for (size_t i = 0; i < sensors.size(); ++i) {
        BaseSensor *sensor = sensors[i];
        const std::string label = sensor
            ? sensor->getName() + " [" + sensor->getRoleLabel() + "]"
            : "Unknown";
        lv_obj_t *button = lv_list_add_btn(ui_AvailableList, nullptr, label.c_str());
        lv_obj_set_user_data(button, reinterpret_cast<void *>(static_cast<intptr_t>(i)));
        lv_obj_add_event_cb(button, [](lv_event_t *e) {
            if (lv_event_get_code(e) != LV_EVENT_CLICKED) {
                return;
            }

            auto *self = static_cast<SensorWikiGui *>(lv_event_get_user_data(e));
            int index = static_cast<int>(reinterpret_cast<intptr_t>(lv_obj_get_user_data(lv_event_get_target(e))));
            self->handleSensorSelection(index);
        }, LV_EVENT_ALL, this);
    }
}

void SensorWikiGui::populateSelectedList()
{
    if (!ui_SelectedList) {
        return;
    }

    lv_obj_clean(ui_SelectedList);
    lv_obj_t *header = lv_label_create(ui_SelectedList);
    lv_label_set_text(header, "Configured devices");

    const auto &sensors = sensorManager.getSensors();
    for (BaseSensor *sensor : sensors) {
        if (!sensor || sensor->getPins().empty()) {
            continue;
        }

        std::string entry = sensor->getTypeName() + " [" + sensor->getRoleLabel() + "] -> " + sensor->getPins();
        lv_list_add_text(ui_SelectedList, entry.c_str());
    }
}

void SensorWikiGui::updateSensorInfo()
{
    BaseSensor *sensor = getSelectedSensor();
    if (!sensor) {
        lv_label_set_text(ui_SensorTitle, "No device selected");
        lv_label_set_text(ui_SensorDescription, "Choose a device from the list.");
        lv_label_set_text(ui_SensorSpecs, "");
        return;
    }

    sensorManager.setCurrentWikiSensor(sensor);
    lv_label_set_text(ui_SensorTitle, sensor->getName().c_str());
    lv_label_set_text(ui_SensorDescription, getSensorInfoText(sensor).c_str());
    lv_label_set_text(ui_SensorSpecs, getSensorSpecsText(sensor).c_str());
}

void SensorWikiGui::updateStartButtonState()
{
    if (!ui_btnStart) {
        return;
    }

    if (sensorManager.hasAssignedSensors()) {
        lv_obj_clear_state(ui_btnStart, LV_STATE_DISABLED);
        return;
    }

    lv_obj_add_state(ui_btnStart, LV_STATE_DISABLED);
}

void SensorWikiGui::handleSensorSelection(int sensorIndex)
{
    selectedSensorIndex = sensorIndex;
    updateSensorInfo();
}

void SensorWikiGui::handleConfigureButtonClick()
{
    BaseSensor *sensor = getSelectedSensor();
    if (!sensor) {
        splashMessage("Select a device first.");
        return;
    }

    sensorManager.setCurrentWikiSensor(sensor);
    switchToMenu();
}

void SensorWikiGui::handleRemoveButtonClick()
{
    BaseSensor *sensor = getSelectedSensor();
    if (!sensor) {
        splashMessage("Select a configured device to remove.");
        return;
    }

    sensorManager.unassignAllPinsForSensor(sensor);
    populateSelectedList();
    updateStartButtonState();
}

void SensorWikiGui::handleStartButtonClick()
{
    sensorManager.setRunning(false);
    sensorManager.selectSensorsFromPinMap();
    sensorManager.setCurrentWikiSensor(nullptr);

    if (!sensorManager.hasAssignedSensors()) {
        splashMessage("No devices configured.");
        return;
    }

    if (!sensorManager.connect()) {
        splashMessage("Error during device connection.");
        return;
    }

    switchToVisualization();
}

void SensorWikiGui::handleBackButtonClick()
{
    if (shouldSelectionBackGoToMainMenu()) {
        switchToMainMenu();
        return;
    }

    switchToCommunicationSelectionScreen();
}

std::string SensorWikiGui::getSensorInfoText(BaseSensor *sensor)
{
    if (!sensor) {
        return "No device information available.";
    }

    std::string info = sensor->getDescription();
    if (info.empty()) {
        info = "No description available.";
    }

    info += "\nRole: " + sensor->getRoleLabel();
    info += "\n\nAllowed Pins: ";
    const auto allowedPins = sensor->getAllowedPinsList();
    if (allowedPins.empty()) {
        info += "Any";
    } else {
        for (size_t i = 0; i < allowedPins.size(); ++i) {
            if (i > 0) {
                info += ", ";
            }
            info += std::to_string(allowedPins[i]);
        }
    }

    return info;
}

std::string SensorWikiGui::getSensorSpecsText(BaseSensor *sensor)
{
    if (!sensor) {
        return "";
    }

    const auto valueKeys = sensor->getValuesKeys();
    const auto configKeys = sensor->getConfigsKeys();

    std::string specs = "Values:\n";
    if (valueKeys.empty()) {
        specs += "- none\n";
    }
    for (const auto &key : valueKeys) {
        specs += "- " + key + " [" + sensor->getValueUnits(key) + "]\n";
    }

    specs += "\nConfigs:\n";
    if (configKeys.empty()) {
        specs += "- none\n";
    }
    for (const auto &key : configKeys) {
        specs += "- " + key + "\n";
    }

    return specs;
}

void SensorWikiGui::init()
{
    if (initialized) {
        return;
    }

    buildWikiGui();
    initialized = true;
}

void SensorWikiGui::showWiki(int)
{
    if (!initialized) {
        return;
    }

    populateAvailableList();
    populateSelectedList();
    updateSensorInfo();
    updateStartButtonState();
    lv_obj_clear_flag(ui_WikiWidget, LV_OBJ_FLAG_HIDDEN);
}

void SensorWikiGui::hideWiki()
{
    if (!initialized || !ui_WikiWidget) {
        return;
    }

    lv_obj_add_flag(ui_WikiWidget, LV_OBJ_FLAG_HIDDEN);
}

BaseSensor *SensorWikiGui::getSelectedSensor()
{
    const auto &sensors = sensorManager.getSensors();
    if (sensors.empty() || selectedSensorIndex < 0 || selectedSensorIndex >= static_cast<int>(sensors.size())) {
        return nullptr;
    }

    return sensors[selectedSensorIndex];
}

void SensorWikiGui::setSelectedSensor(int index)
{
    selectedSensorIndex = index;
    updateSensorInfo();
}
