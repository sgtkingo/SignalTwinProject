#include "device_selection_gui.hpp"

#include "../helpers.hpp"

DeviceSelectionGui::DeviceSelectionGui(DeviceCatalog &deviceCatalog, DeviceManager &sensorManager)
    : deviceCatalog(deviceCatalog), sensorManager(sensorManager)
{
}

void DeviceSelectionGui::buildSelectionGui()
{
    ui_SelectionWidget = lv_obj_create(lv_scr_act());
    lv_obj_remove_style_all(ui_SelectionWidget);
    lv_obj_set_size(ui_SelectionWidget, 760, 440);
    lv_obj_set_align(ui_SelectionWidget, LV_ALIGN_CENTER);
    lv_obj_set_style_radius(ui_SelectionWidget, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_SelectionWidget, lv_color_hex(0xF5F5F5), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_SelectionWidget, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_SelectionWidget, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_flag(ui_SelectionWidget, LV_OBJ_FLAG_HIDDEN);

    lv_obj_t *title = lv_label_create(ui_SelectionWidget);
    lv_label_set_text(title, "Selection");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 12);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_AvailableList = lv_list_create(ui_SelectionWidget);
    lv_obj_set_size(ui_AvailableList, 210, 285);
    lv_obj_set_pos(ui_AvailableList, 10, 50);

    lv_obj_t *infoPanel = lv_obj_create(ui_SelectionWidget);
    lv_obj_set_size(infoPanel, 300, 285);
    lv_obj_set_pos(infoPanel, 230, 50);

    ui_DeviceTitle = lv_label_create(infoPanel);
    lv_obj_set_width(ui_DeviceTitle, 270);
    lv_obj_set_pos(ui_DeviceTitle, 10, 10);
    lv_obj_set_style_text_font(ui_DeviceTitle, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_DeviceDescription = lv_label_create(infoPanel);
    lv_obj_set_width(ui_DeviceDescription, 270);
    lv_obj_set_pos(ui_DeviceDescription, 10, 50);
    lv_label_set_long_mode(ui_DeviceDescription, LV_LABEL_LONG_WRAP);

    ui_DeviceSpecs = lv_label_create(infoPanel);
    lv_obj_set_width(ui_DeviceSpecs, 270);
    lv_obj_set_pos(ui_DeviceSpecs, 10, 150);
    lv_label_set_long_mode(ui_DeviceSpecs, LV_LABEL_LONG_WRAP);

    ui_SelectedList = lv_list_create(ui_SelectionWidget);
    lv_obj_set_size(ui_SelectedList, 210, 285);
    lv_obj_set_pos(ui_SelectedList, 540, 50);

    ui_btnBack = lv_btn_create(ui_SelectionWidget);
    lv_obj_set_size(ui_btnBack, 90, 36);
    lv_obj_set_pos(ui_btnBack, 12, 388);
    lv_obj_add_event_cb(ui_btnBack, [](lv_event_t *e) {
        if (lv_event_get_code(e) != LV_EVENT_CLICKED) {
            return;
        }
        auto *self = static_cast<DeviceSelectionGui *>(lv_event_get_user_data(e));
        self->handleBackButtonClick();
    }, LV_EVENT_ALL, this);
    lv_obj_t *backLabel = lv_label_create(ui_btnBack);
    lv_label_set_text(backLabel, "Back");
    lv_obj_center(backLabel);

    ui_btnConfigure = lv_btn_create(ui_SelectionWidget);
    lv_obj_set_size(ui_btnConfigure, 145, 36);
    lv_obj_set_pos(ui_btnConfigure, 315, 344);
    lv_obj_add_event_cb(ui_btnConfigure, [](lv_event_t *e) {
        if (lv_event_get_code(e) != LV_EVENT_CLICKED) {
            return;
        }
        auto *self = static_cast<DeviceSelectionGui *>(lv_event_get_user_data(e));
        self->handleConfigureButtonClick();
    }, LV_EVENT_ALL, this);
    lv_obj_t *configureLabel = lv_label_create(ui_btnConfigure);
    lv_label_set_text(configureLabel, "Configure");
    lv_obj_center(configureLabel);

    ui_btnRemove = lv_btn_create(ui_SelectionWidget);
    lv_obj_set_size(ui_btnRemove, 145, 36);
    lv_obj_set_pos(ui_btnRemove, 315, 388);
    lv_obj_set_style_bg_color(ui_btnRemove, lv_color_hex(0xD96464), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(ui_btnRemove, [](lv_event_t *e) {
        if (lv_event_get_code(e) != LV_EVENT_CLICKED) {
            return;
        }
        auto *self = static_cast<DeviceSelectionGui *>(lv_event_get_user_data(e));
        self->handleRemoveButtonClick();
    }, LV_EVENT_ALL, this);
    lv_obj_t *removeLabel = lv_label_create(ui_btnRemove);
    lv_label_set_text(removeLabel, "Remove");
    lv_obj_center(removeLabel);

    ui_btnStart = lv_btn_create(ui_SelectionWidget);
    lv_obj_set_size(ui_btnStart, 145, 36);
    lv_obj_set_pos(ui_btnStart, 605, 388);
    lv_obj_add_event_cb(ui_btnStart, [](lv_event_t *e) {
        if (lv_event_get_code(e) != LV_EVENT_CLICKED) {
            return;
        }
        auto *self = static_cast<DeviceSelectionGui *>(lv_event_get_user_data(e));
        self->handleStartButtonClick();
    }, LV_EVENT_ALL, this);
    lv_obj_t *startLabel = lv_label_create(ui_btnStart);
    lv_label_set_text(startLabel, "Start Visualization");
    lv_obj_center(startLabel);
}

void DeviceSelectionGui::populateAvailableList()
{
    if (!ui_AvailableList) {
        return;
    }

    lv_obj_clean(ui_AvailableList);
    const auto &sensors = deviceCatalog.getDevices();
    for (size_t i = 0; i < sensors.size(); ++i) {
        BaseDevice *sensor = sensors[i];
        const std::string label = sensor
            ? sensor->getName() + " [" + sensor->getRoleLabel() + "]"
            : "Unknown";
        lv_obj_t *button = lv_list_add_btn(ui_AvailableList, nullptr, label.c_str());
        lv_obj_set_user_data(button, reinterpret_cast<void *>(static_cast<intptr_t>(i)));
        lv_obj_add_event_cb(button, [](lv_event_t *e) {
            if (lv_event_get_code(e) != LV_EVENT_CLICKED) {
                return;
            }

            auto *self = static_cast<DeviceSelectionGui *>(lv_event_get_user_data(e));
            int index = static_cast<int>(reinterpret_cast<intptr_t>(lv_obj_get_user_data(lv_event_get_target(e))));
            self->handleDeviceSelection(index);
        }, LV_EVENT_ALL, this);
    }
}

void DeviceSelectionGui::populateSelectedList()
{
    if (!ui_SelectedList) {
        return;
    }

    lv_obj_clean(ui_SelectedList);
    lv_obj_t *header = lv_label_create(ui_SelectedList);
    lv_label_set_text(header, "Configured devices");

    const auto &sensors = deviceCatalog.getDevices();
    for (BaseDevice *sensor : sensors) {
        if (!sensor || sensor->getPins().empty()) {
            continue;
        }

        std::string entry = sensor->getTypeName() + " [" + sensor->getRoleLabel() + "] -> " + sensor->getPins();
        lv_list_add_text(ui_SelectedList, entry.c_str());
    }
}

void DeviceSelectionGui::updateDeviceInfo()
{
    BaseDevice *sensor = getSelectedDevice();
    if (!sensor) {
        lv_label_set_text(ui_DeviceTitle, "No device selected");
        lv_label_set_text(ui_DeviceDescription, "Choose a device from the list.");
        lv_label_set_text(ui_DeviceSpecs, "");
        return;
    }

    sensorManager.setCurrentSelectionDevice(sensor);
    lv_label_set_text(ui_DeviceTitle, sensor->getName().c_str());
    lv_label_set_text(ui_DeviceDescription, getDeviceInfoText(sensor).c_str());
    lv_label_set_text(ui_DeviceSpecs, getDeviceSpecsText(sensor).c_str());
}

void DeviceSelectionGui::updateStartButtonState()
{
    if (!ui_btnStart) {
        return;
    }

    if (sensorManager.hasAssignedDevices()) {
        lv_obj_clear_state(ui_btnStart, LV_STATE_DISABLED);
        return;
    }

    lv_obj_add_state(ui_btnStart, LV_STATE_DISABLED);
}

void DeviceSelectionGui::handleDeviceSelection(int deviceIndex)
{
    selectedDeviceIndex = deviceIndex;
    updateDeviceInfo();
}

void DeviceSelectionGui::handleConfigureButtonClick()
{
    BaseDevice *sensor = getSelectedDevice();
    if (!sensor) {
        splashMessage("Select a device first.");
        return;
    }

    sensorManager.setCurrentSelectionDevice(sensor);
    switchToMenu();
}

void DeviceSelectionGui::handleRemoveButtonClick()
{
    BaseDevice *sensor = getSelectedDevice();
    if (!sensor) {
        splashMessage("Select a configured device to remove.");
        return;
    }

    sensorManager.unassignAllPinsForDevice(sensor);
    populateSelectedList();
    updateStartButtonState();
}

void DeviceSelectionGui::handleStartButtonClick()
{
    sensorManager.setRunning(false);
    sensorManager.selectDevicesFromPinMap();
    sensorManager.setCurrentSelectionDevice(nullptr);

    if (!sensorManager.hasAssignedDevices()) {
        splashMessage("No devices configured.");
        return;
    }

    if (!sensorManager.connect()) {
        splashMessage("Error during device connection.");
        return;
    }

    switchToVisualization();
}

void DeviceSelectionGui::handleBackButtonClick()
{
    if (shouldSelectionBackGoToMainMenu()) {
        switchToMainMenu();
        return;
    }

    switchToCommunicationSelectionScreen();
}

std::string DeviceSelectionGui::getDeviceInfoText(BaseDevice *sensor)
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

std::string DeviceSelectionGui::getDeviceSpecsText(BaseDevice *sensor)
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

void DeviceSelectionGui::init()
{
    if (initialized) {
        return;
    }

    buildSelectionGui();
    initialized = true;
}

void DeviceSelectionGui::showSelection(int)
{
    if (!initialized) {
        return;
    }

    populateAvailableList();
    populateSelectedList();
    updateDeviceInfo();
    updateStartButtonState();
    lv_obj_clear_flag(ui_SelectionWidget, LV_OBJ_FLAG_HIDDEN);
}

void DeviceSelectionGui::hideSelection()
{
    if (!initialized || !ui_SelectionWidget) {
        return;
    }

    lv_obj_add_flag(ui_SelectionWidget, LV_OBJ_FLAG_HIDDEN);
}

BaseDevice *DeviceSelectionGui::getSelectedDevice()
{
    const auto &sensors = deviceCatalog.getDevices();
    if (sensors.empty() || selectedDeviceIndex < 0 || selectedDeviceIndex >= static_cast<int>(sensors.size())) {
        return nullptr;
    }

    return sensors[selectedDeviceIndex];
}

void DeviceSelectionGui::setSelectedDevice(int index)
{
    selectedDeviceIndex = index;
    updateDeviceInfo();
}
