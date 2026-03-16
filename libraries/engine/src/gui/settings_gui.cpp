#include "settings_gui.hpp"

#include <string>

void SettingsGui::addModeButton(const char *text, DefaultCommunicationMode mode, lv_coord_t y)
{
    lv_obj_t *button = lv_btn_create(ui_Widget);
    lv_obj_set_size(button, 260, 40);
    lv_obj_align(button, LV_ALIGN_TOP_MID, 0, y);
    lv_obj_add_event_cb(button, [](lv_event_t *e) {
        if (lv_event_get_code(e) != LV_EVENT_CLICKED) {
            return;
        }

        auto *self = static_cast<SettingsGui *>(lv_event_get_user_data(e));
        auto mode = static_cast<DefaultCommunicationMode>(reinterpret_cast<intptr_t>(lv_obj_get_user_data(lv_event_get_target(e))));
        setDefaultCommunicationMode(mode);
        self->showSettings();
    }, LV_EVENT_ALL, this);
    lv_obj_set_user_data(button, reinterpret_cast<void *>(static_cast<intptr_t>(mode)));

    lv_obj_t *label = lv_label_create(button);
    lv_label_set_text(label, text);
    lv_obj_center(label);
}

void SettingsGui::build()
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
    lv_label_set_text(title, "Settings");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 16);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_CurrentMode = lv_label_create(ui_Widget);
    lv_obj_align(ui_CurrentMode, LV_ALIGN_TOP_MID, 0, 60);
    lv_obj_set_style_text_font(ui_CurrentMode, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);

    addModeButton("Ask every time", DefaultCommunicationMode::ASK, 110);
    addModeButton("Cable (UART)", DefaultCommunicationMode::CABLE, 160);
    addModeButton("Wireless Auto", DefaultCommunicationMode::WIRELESS_AUTO, 210);
    addModeButton("Wireless Manual", DefaultCommunicationMode::WIRELESS_MANUAL, 260);

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
}

void SettingsGui::refresh()
{
    if (!ui_CurrentMode) {
        return;
    }

    const char *modeText = "Ask every time";
    switch (getDefaultCommunicationMode()) {
    case DefaultCommunicationMode::CABLE:
        modeText = "Cable (UART)";
        break;
    case DefaultCommunicationMode::WIRELESS_AUTO:
        modeText = "Wireless Auto";
        break;
    case DefaultCommunicationMode::WIRELESS_MANUAL:
        modeText = "Wireless Manual";
        break;
    default:
        break;
    }

    std::string label = "Default communication: ";
    label += modeText;
    lv_label_set_text(ui_CurrentMode, label.c_str());
}

void SettingsGui::init()
{
    if (initialized) {
        return;
    }

    build();
    initialized = true;
}

void SettingsGui::showSettings()
{
    if (!initialized || !ui_Widget) {
        return;
    }

    refresh();
    lv_obj_clear_flag(ui_Widget, LV_OBJ_FLAG_HIDDEN);
}

void SettingsGui::hideSettings()
{
    if (!initialized || !ui_Widget) {
        return;
    }

    lv_obj_add_flag(ui_Widget, LV_OBJ_FLAG_HIDDEN);
}
