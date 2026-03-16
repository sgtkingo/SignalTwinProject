#include "menu_gui.hpp"

#include "../helpers.hpp"

MenuGui::MenuGui(SensorManager &sensorManager) : sensorManager(sensorManager), activePinIndex(-1)
{
    pinContainers.fill(nullptr);
    pinLabels.fill(nullptr);
}

void MenuGui::init()
{
    if (initialized) {
        return;
    }

    buildMenu();
    initialized = true;
}

void MenuGui::buildMenu()
{
    ui_MenuWidget = lv_obj_create(lv_scr_act());
    lv_obj_remove_style_all(ui_MenuWidget);
    lv_obj_set_size(ui_MenuWidget, 760, 440);
    lv_obj_set_align(ui_MenuWidget, LV_ALIGN_CENTER);
    lv_obj_set_style_radius(ui_MenuWidget, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_MenuWidget, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_MenuWidget, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_MenuWidget, 2, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Title = lv_label_create(ui_MenuWidget);
    lv_label_set_text(ui_Title, "Connection");
    lv_obj_align(ui_Title, LV_ALIGN_TOP_MID, 0, 10);
    lv_obj_set_style_text_font(ui_Title, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Subtitle = lv_label_create(ui_MenuWidget);
    lv_obj_set_width(ui_Subtitle, 700);
    lv_obj_align(ui_Subtitle, LV_ALIGN_TOP_MID, 0, 40);
    lv_obj_set_style_text_align(ui_Subtitle, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_btnBack = lv_btn_create(ui_MenuWidget);
    lv_obj_set_size(ui_btnBack, 90, 36);
    lv_obj_align(ui_btnBack, LV_ALIGN_TOP_LEFT, 12, 10);
    lv_obj_add_event_cb(ui_btnBack, [](lv_event_t *e) {
        if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
            switchToWiki();
        }
    }, LV_EVENT_ALL, nullptr);
    lv_obj_t *backLabel = lv_label_create(ui_btnBack);
    lv_label_set_text(backLabel, "Back");
    lv_obj_center(backLabel);

    ui_btnConnect = lv_btn_create(ui_MenuWidget);
    lv_obj_set_size(ui_btnConnect, 110, 40);
    lv_obj_align(ui_btnConnect, LV_ALIGN_TOP_RIGHT, -12, 10);
    lv_obj_add_event_cb(ui_btnConnect, [](lv_event_t *e) {
        if (lv_event_get_code(e) != LV_EVENT_CLICKED) {
            return;
        }
        auto *self = static_cast<MenuGui *>(lv_event_get_user_data(e));
        self->handleConnectButtonClick();
    }, LV_EVENT_ALL, this);
    lv_obj_t *connectLabel = lv_label_create(ui_btnConnect);
    lv_label_set_text(connectLabel, "Connect");
    lv_obj_center(connectLabel);

    ui_PinScrollContainer = lv_obj_create(ui_MenuWidget);
    lv_obj_set_size(ui_PinScrollContainer, 735, 330);
    lv_obj_align(ui_PinScrollContainer, LV_ALIGN_BOTTOM_MID, 0, -12);
    lv_obj_set_scroll_dir(ui_PinScrollContainer, LV_DIR_VER);
    lv_obj_set_scrollbar_mode(ui_PinScrollContainer, LV_SCROLLBAR_MODE_AUTO);
    lv_obj_set_style_pad_all(ui_PinScrollContainer, 10, LV_PART_MAIN);
    lv_obj_set_style_pad_gap(ui_PinScrollContainer, 10, LV_PART_MAIN);
    lv_obj_set_flex_flow(ui_PinScrollContainer, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(ui_PinScrollContainer, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);

    for (int i = 0; i < NUM_PINS; ++i) {
        pinContainers[i] = lv_btn_create(ui_PinScrollContainer);
        lv_obj_set_size(pinContainers[i], 220, 90);
        lv_obj_set_user_data(pinContainers[i], reinterpret_cast<void *>(static_cast<intptr_t>(i)));
        lv_obj_add_event_cb(pinContainers[i], [](lv_event_t *e) {
            if (lv_event_get_code(e) != LV_EVENT_CLICKED) {
                return;
            }

            auto *self = static_cast<MenuGui *>(lv_event_get_user_data(e));
            int index = static_cast<int>(reinterpret_cast<intptr_t>(lv_obj_get_user_data(lv_event_get_target(e))));
            self->handlePinClick(index);
        }, LV_EVENT_ALL, this);

        pinLabels[i] = lv_label_create(pinContainers[i]);
        lv_obj_set_width(pinLabels[i], 200);
        lv_obj_center(pinLabels[i]);
        lv_obj_set_style_text_align(pinLabels[i], LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    }
}

void MenuGui::updateHeader()
{
    BaseSensor *sensor = sensorManager.getCurrentWikiSensor();
    if (!sensor) {
        lv_label_set_text(ui_Subtitle, "Select a device in Selection first.");
        return;
    }

    std::string subtitle = sensor->getTypeName() + " | Green=assign, Yellow=unassign, Red=used elsewhere, Gray=locked";
    lv_label_set_text(ui_Subtitle, subtitle.c_str());
}

bool MenuGui::isPinAllowedForCurrentSensor(int pinIndex) const
{
    BaseSensor *sensor = sensorManager.getCurrentWikiSensor();
    if (!sensor) {
        return false;
    }

    return sensor->isPinAllowed(sensorManager.getPinNumber(pinIndex));
}

uint32_t MenuGui::getPinStateColor(int pinIndex) const
{
    BaseSensor *selectedSensor = sensorManager.getCurrentWikiSensor();
    BaseSensor *assignedSensor = sensorManager.getAssignedSensor(pinIndex);

    if (sensorManager.isPinLocked(pinIndex) || !isPinAllowedForCurrentSensor(pinIndex)) {
        return 0x9E9E9E;
    }

    if (assignedSensor == nullptr) {
        return 0x00B050;
    }

    if (assignedSensor == selectedSensor) {
        return 0xF2C94C;
    }

    return 0xD9534F;
}

void MenuGui::showMenu()
{
    if (!initialized || !ui_MenuWidget) {
        return;
    }

    updateHeader();
    updatePinVisualStates();
    lv_obj_clear_flag(ui_MenuWidget, LV_OBJ_FLAG_HIDDEN);
}

void MenuGui::hideMenu()
{
    if (!initialized || !ui_MenuWidget) {
        return;
    }

    lv_obj_add_flag(ui_MenuWidget, LV_OBJ_FLAG_HIDDEN);
}

void MenuGui::updatePinLabels()
{
    BaseSensor *selectedSensor = sensorManager.getCurrentWikiSensor();

    for (int i = 0; i < NUM_PINS; i++) {
        if (!pinLabels[i]) {
            continue;
        }

        int gpioNumber = sensorManager.getPinNumber(i);
        BaseSensor *assignedSensor = sensorManager.getAssignedSensor(i);
        std::string labelText = "Pin " + std::to_string(gpioNumber) + "\n";

        if (sensorManager.isPinLocked(i) || !isPinAllowedForCurrentSensor(i)) {
            labelText += "Locked";
        } else if (assignedSensor == nullptr) {
            labelText += "Available";
        } else if (assignedSensor == selectedSensor) {
            labelText += selectedSensor->getTypeName();
        } else {
            labelText += assignedSensor->getTypeName();
        }

        lv_label_set_text(pinLabels[i], labelText.c_str());
    }
}

void MenuGui::updatePinVisualStates()
{
    updatePinLabels();

    for (int i = 0; i < NUM_PINS; i++) {
        if (!pinContainers[i]) {
            continue;
        }

        uint32_t pinStateColor = getPinStateColor(i);
        lv_obj_set_style_bg_color(pinContainers[i], lv_color_hex(pinStateColor), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(pinContainers[i], 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(pinContainers[i], 1, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_color(pinContainers[i], lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);

        if (pinStateColor == 0xF2C94C || pinStateColor == 0x00B050) {
            lv_obj_set_style_text_color(pinLabels[i], lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
        } else {
            lv_obj_set_style_text_color(pinLabels[i], lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
    }
}

void MenuGui::setActivePin(int pinIndex)
{
    activePinIndex = pinIndex;
}

void MenuGui::handleConnectButtonClick()
{
    switchToWiki();
}

void MenuGui::handlePinClick(int pinIndex)
{
    BaseSensor *sensor = sensorManager.getCurrentWikiSensor();
    if (!sensor || pinIndex < 0) {
        splashMessage("No device selected.");
        return;
    }

    setActivePin(pinIndex);

    if (sensorManager.isPinLocked(pinIndex) || !isPinAllowedForCurrentSensor(pinIndex)) {
        splashMessage("This pin cannot be used by the selected device.");
        return;
    }

    BaseSensor *assignedSensor = sensorManager.getAssignedSensor(pinIndex);
    if (assignedSensor == sensor) {
        sensorManager.unassignSensorFromPin(pinIndex);
    } else if (assignedSensor == nullptr) {
        sensorManager.assignSensorToPin(sensor, pinIndex);
    } else {
        splashMessage("This pin is used by another device.");
    }

    initializePins();
}

void MenuGui::initializePins()
{
    updateHeader();
    updatePinVisualStates();
}
