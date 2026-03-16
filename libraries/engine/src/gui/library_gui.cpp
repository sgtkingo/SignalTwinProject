#include "library_gui.hpp"

LibraryGui::LibraryGui(SensorManager &sensorManager) : sensorManager(sensorManager)
{
}

void LibraryGui::build()
{
    ui_Widget = lv_obj_create(lv_scr_act());
    lv_obj_remove_style_all(ui_Widget);
    lv_obj_set_size(ui_Widget, 760, 440);
    lv_obj_set_align(ui_Widget, LV_ALIGN_CENTER);
    lv_obj_set_style_radius(ui_Widget, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Widget, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Widget, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_Widget, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_Widget, 2, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t *title = lv_label_create(ui_Widget);
    lv_label_set_text(title, "Library");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 14);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t *back = lv_btn_create(ui_Widget);
    lv_obj_set_size(back, 90, 36);
    lv_obj_align(back, LV_ALIGN_BOTTOM_LEFT, 16, -14);
    lv_obj_add_event_cb(back, [](lv_event_t *e) {
        if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
            switchToMainMenu();
        }
    }, LV_EVENT_ALL, nullptr);
    lv_obj_t *backLabel = lv_label_create(back);
    lv_label_set_text(backLabel, "Back");
    lv_obj_center(backLabel);

    lv_obj_t *edit = lv_btn_create(ui_Widget);
    lv_obj_set_size(edit, 120, 36);
    lv_obj_align(edit, LV_ALIGN_BOTTOM_RIGHT, -140, -14);
    lv_obj_add_event_cb(edit, [](lv_event_t *e) {
        if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
            switchToLibraryEditor();
        }
    }, LV_EVENT_ALL, nullptr);
    lv_obj_t *editLabel = lv_label_create(edit);
    lv_label_set_text(editLabel, "Edit Entity");
    lv_obj_center(editLabel);

    lv_obj_t *create = lv_btn_create(ui_Widget);
    lv_obj_set_size(create, 120, 36);
    lv_obj_align(create, LV_ALIGN_BOTTOM_RIGHT, -12, -14);
    lv_obj_add_event_cb(create, [](lv_event_t *e) {
        if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
            prepareNewLibraryEntity();
            switchToLibraryEditor();
        }
    }, LV_EVENT_ALL, nullptr);
    lv_obj_t *createLabel = lv_label_create(create);
    lv_label_set_text(createLabel, "New Entity");
    lv_obj_center(createLabel);

    ui_SensorList = lv_list_create(ui_Widget);
    lv_obj_set_size(ui_SensorList, 250, 320);
    lv_obj_align(ui_SensorList, LV_ALIGN_LEFT_MID, 15, 18);

    ui_Detail = lv_obj_create(ui_Widget);
    lv_obj_set_size(ui_Detail, 450, 320);
    lv_obj_align(ui_Detail, LV_ALIGN_RIGHT_MID, -15, 18);

    ui_DetailLabel = lv_label_create(ui_Detail);
    lv_obj_set_width(ui_DetailLabel, lv_pct(100));
    lv_label_set_long_mode(ui_DetailLabel, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui_DetailLabel, LV_ALIGN_TOP_LEFT, 10, 10);
}

void LibraryGui::populateSensorList()
{
    if (!ui_SensorList) {
        return;
    }

    lv_obj_clean(ui_SensorList);

    const auto &sensors = sensorManager.getSensors();
    for (size_t i = 0; i < sensors.size(); ++i) {
        BaseSensor *sensor = sensors[i];
        lv_obj_t *button = lv_list_add_btn(ui_SensorList, nullptr, sensor ? sensor->getName().c_str() : "Unknown");
        lv_obj_add_event_cb(button, [](lv_event_t *e) {
            if (lv_event_get_code(e) != LV_EVENT_CLICKED) {
                return;
            }

            auto *self = static_cast<LibraryGui *>(lv_event_get_user_data(e));
            int index = static_cast<int>(reinterpret_cast<intptr_t>(lv_obj_get_user_data(lv_event_get_target(e))));
            self->setSelectedSensor(index);
        }, LV_EVENT_ALL, this);
        lv_obj_set_user_data(button, reinterpret_cast<void *>(static_cast<intptr_t>(i)));
    }
}

void LibraryGui::updateDetail()
{
    if (!ui_DetailLabel) {
        return;
    }

    const auto &sensors = sensorManager.getSensors();
    if (sensors.empty() || selectedSensorIndex < 0 || selectedSensorIndex >= static_cast<int>(sensors.size())) {
        lv_label_set_text(ui_DetailLabel, "No entity selected.");
        return;
    }

    BaseSensor *sensor = sensors[selectedSensorIndex];
    sensorManager.setCurrentLibrarySensor(sensor);

    std::string detail = "Entity\n";
    detail += sensor->getName() + "\n\n";
    detail += "Description:\n" + sensor->getDescription() + "\n\n";
    detail += "Values:\n";
    for (const auto &key : sensor->getValuesKeys()) {
        detail += "- " + key + " [" + sensor->getValueUnits(key) + "]\n";
    }
    detail += "\nConfigs:\n";
    for (const auto &key : sensor->getConfigsKeys()) {
        detail += "- " + key + "\n";
    }

    lv_label_set_text(ui_DetailLabel, detail.c_str());
}

void LibraryGui::init()
{
    if (initialized) {
        return;
    }

    build();
    initialized = true;
}

void LibraryGui::showLibrary()
{
    if (!initialized || !ui_Widget) {
        return;
    }

    populateSensorList();
    updateDetail();
    lv_obj_clear_flag(ui_Widget, LV_OBJ_FLAG_HIDDEN);
}

void LibraryGui::hideLibrary()
{
    if (!initialized || !ui_Widget) {
        return;
    }

    lv_obj_add_flag(ui_Widget, LV_OBJ_FLAG_HIDDEN);
}

void LibraryGui::setSelectedSensor(int index)
{
    selectedSensorIndex = index;
    updateDetail();
}
