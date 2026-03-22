#include "library_editor_gui.hpp"

#include "../helpers.hpp"

LibraryEditorGui::LibraryEditorGui(DeviceManager &sensorManager) : sensorManager(sensorManager)
{
}

void LibraryEditorGui::build()
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

    ui_Title = lv_label_create(ui_Widget);
    lv_obj_align(ui_Title, LV_ALIGN_TOP_MID, 0, 14);
    lv_obj_set_style_text_font(ui_Title, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Text = lv_textarea_create(ui_Widget);
    lv_obj_set_size(ui_Text, 700, 300);
    lv_obj_align(ui_Text, LV_ALIGN_CENTER, 0, 10);
    lv_textarea_set_one_line(ui_Text, false);

    lv_obj_t *back = lv_btn_create(ui_Widget);
    lv_obj_set_size(back, 90, 36);
    lv_obj_align(back, LV_ALIGN_BOTTOM_LEFT, 16, -14);
    lv_obj_add_event_cb(back, [](lv_event_t *e) {
        if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
            switchToLibrary();
        }
    }, LV_EVENT_ALL, nullptr);
    lv_obj_t *backLabel = lv_label_create(back);
    lv_label_set_text(backLabel, "Back");
    lv_obj_center(backLabel);

    lv_obj_t *save = lv_btn_create(ui_Widget);
    lv_obj_set_size(save, 90, 36);
    lv_obj_align(save, LV_ALIGN_BOTTOM_RIGHT, -16, -14);
    lv_obj_add_event_cb(save, [](lv_event_t *e) {
        if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
            splashMessage("Library persistence is planned in the next stage.");
        }
    }, LV_EVENT_ALL, nullptr);
    lv_obj_t *saveLabel = lv_label_create(save);
    lv_label_set_text(saveLabel, "Save");
    lv_obj_center(saveLabel);
}

void LibraryEditorGui::refresh()
{
    BaseDevice *sensor = sensorManager.getCurrentLibraryDevice();
    if (!sensor) {
        lv_label_set_text(ui_Title, "New Entity");
        lv_textarea_set_text(ui_Text, "Name:\n\nDescription:\n\nValues:\n\nConfigs:\n");
        return;
    }

    lv_label_set_text(ui_Title, "Edit Entity");
    std::string text = "Name:\n" + sensor->getName() + "\n\n";
    text += "Role:\n" + sensor->getRoleLabel() + "\n\n";
    text += "Description:\n" + sensor->getDescription() + "\n\n";
    text += "Values:\n";
    for (const auto &key : sensor->getValuesKeys()) {
        text += "- " + key + "\n";
    }
    text += "\nConfigs:\n";
    for (const auto &key : sensor->getConfigsKeys()) {
        text += "- " + key + "\n";
    }
    lv_textarea_set_text(ui_Text, text.c_str());
}

void LibraryEditorGui::init()
{
    if (initialized) {
        return;
    }

    build();
    initialized = true;
}

void LibraryEditorGui::showEditor()
{
    if (!initialized || !ui_Widget) {
        return;
    }

    refresh();
    lv_obj_clear_flag(ui_Widget, LV_OBJ_FLAG_HIDDEN);
}

void LibraryEditorGui::hideEditor()
{
    if (!initialized || !ui_Widget) {
        return;
    }

    lv_obj_add_flag(ui_Widget, LV_OBJ_FLAG_HIDDEN);
}
