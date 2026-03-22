#include "communication_selection_gui.hpp"

#include "../helpers.hpp"

CommunicationSelectionGui::CommunicationSelectionGui(GuiRouter &router) : router(router)
{
}

void CommunicationSelectionGui::createOptionButton(const char *text, lv_coord_t x, lv_coord_t y, DefaultCommunicationMode mode)
{
    lv_obj_t *button = lv_btn_create(ui_Widget);
    lv_obj_set_size(button, 220, 80);
    lv_obj_set_pos(button, x, y);
    lv_obj_add_event_cb(button, [](lv_event_t *e) {
        if (lv_event_get_code(e) != LV_EVENT_CLICKED) {
            return;
        }

        auto *self = static_cast<CommunicationSelectionGui *>(lv_event_get_user_data(e));
        DefaultCommunicationMode mode = static_cast<DefaultCommunicationMode>(reinterpret_cast<intptr_t>(lv_obj_get_user_data(lv_event_get_target(e))));
        self->router.completeCommunicationSelection(mode);
    }, LV_EVENT_ALL, this);
    lv_obj_set_user_data(button, reinterpret_cast<void *>(static_cast<intptr_t>(mode)));

    lv_obj_t *label = lv_label_create(button);
    lv_label_set_text(label, text);
    lv_obj_center(label);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
}

void CommunicationSelectionGui::init(void)
{
    if (initialized) {
        return;
    }

    constructCommunicationSelection();
    initialized = true;
}

void CommunicationSelectionGui::constructCommunicationSelection(void)
{
    ui_Widget = lv_obj_create(lv_scr_act());
    lv_obj_remove_style_all(ui_Widget);
    lv_obj_set_width(ui_Widget, 760);
    lv_obj_set_height(ui_Widget, 440);
    lv_obj_set_align(ui_Widget, LV_ALIGN_CENTER);
    lv_obj_set_style_radius(ui_Widget, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Widget, lv_color_hex(0x055DA9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Widget, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_Widget, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_Widget, 2, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t *title = lv_label_create(ui_Widget);
    lv_label_set_text(title, "Communication");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 30);
    lv_obj_set_style_text_color(title, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_40, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t *subtitle = lv_label_create(ui_Widget);
    lv_label_set_text(subtitle, "Choose how the target platform should be reached");
    lv_obj_align(subtitle, LV_ALIGN_TOP_MID, 0, 75);
    lv_obj_set_style_text_color(subtitle, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);

    createOptionButton("Cable (UART)", 70, 130, DefaultCommunicationMode::CABLE);
    createOptionButton("Wireless Auto", 395, 130, DefaultCommunicationMode::WIRELESS_AUTO);
    createOptionButton("Wireless Manual", 230, 235, DefaultCommunicationMode::WIRELESS_MANUAL);

    lv_obj_t *back = lv_btn_create(ui_Widget);
    lv_obj_set_size(back, 90, 36);
    lv_obj_align(back, LV_ALIGN_BOTTOM_LEFT, 16, -14);
    lv_obj_add_event_cb(back, [](lv_event_t *e) {
        if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
            auto *self = static_cast<CommunicationSelectionGui *>(lv_event_get_user_data(e));
            self->router.showMainMenu();
        }
    }, LV_EVENT_ALL, this);
    lv_obj_t *backLabel = lv_label_create(back);
    lv_label_set_text(backLabel, "Back");
    lv_obj_center(backLabel);

    lv_obj_t *hint = lv_label_create(ui_Widget);
    lv_label_set_text(hint, "Bluetooth pairing is represented as a flow option in this stage.");
    lv_obj_align(hint, LV_ALIGN_BOTTOM_MID, 0, -16);
    lv_obj_set_style_text_color(hint, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
}

void CommunicationSelectionGui::hideCommunicationSelection(void)
{
    if (!initialized) {
        return;
    }

    if (ui_Widget) {
        lv_obj_del(ui_Widget);
    }

    ui_Widget = nullptr;
    initialized = false;
}
