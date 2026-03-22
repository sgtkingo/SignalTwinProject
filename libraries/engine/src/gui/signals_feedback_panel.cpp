#include "signals_feedback_panel.hpp"

void SignalsFeedbackPanel::showShadowOverlay()
{
    hideShadowOverlay();

    ui_ShadowOverlay = lv_obj_create(lv_scr_act());
    lv_obj_clear_flag(ui_ShadowOverlay, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_flag(ui_ShadowOverlay, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_size(ui_ShadowOverlay, lv_pct(100), lv_pct(100));
    lv_obj_align(ui_ShadowOverlay, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_radius(ui_ShadowOverlay, 0, 0);
    lv_obj_set_style_bg_color(ui_ShadowOverlay, lv_color_black(), 0);
    lv_obj_set_style_bg_opa(ui_ShadowOverlay, LV_OPA_50, 0);
    lv_obj_set_style_border_width(ui_ShadowOverlay, 0, 0);
}

void SignalsFeedbackPanel::hideShadowOverlay()
{
    if (ui_ShadowOverlay) {
        lv_obj_del(ui_ShadowOverlay);
        ui_ShadowOverlay = nullptr;
    }
}

void SignalsFeedbackPanel::showAlert(lv_obj_t *parentWidget, void *userData, const char *message, lv_event_cb_t dismissCallback)
{
    if (!parentWidget || !message) {
        return;
    }

    hideAlert();

    ui_Alert = lv_obj_create(parentWidget);
    lv_obj_remove_style_all(ui_Alert);
    lv_obj_set_width(ui_Alert, 400);
    lv_obj_set_height(ui_Alert, 40);
    lv_obj_set_x(ui_Alert, 0);
    lv_obj_set_y(ui_Alert, 10);
    lv_obj_set_align(ui_Alert, LV_ALIGN_TOP_MID);
    lv_obj_clear_flag(ui_Alert, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_radius(ui_Alert, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Alert, lv_color_hex(0x4C9ED3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Alert, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(ui_Alert, dismissCallback, LV_EVENT_CLICKED, userData);
    lv_obj_add_event_cb(
        ui_Alert,
        [](lv_event_t *e) {
            auto *self = static_cast<SignalsFeedbackPanel *>(lv_event_get_user_data(e));
            self->ui_Alert = nullptr;
            self->ui_AlertLabel = nullptr;
        },
        LV_EVENT_DELETE,
        this);

    ui_AlertLabel = lv_label_create(ui_Alert);
    lv_obj_set_width(ui_AlertLabel, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_AlertLabel, LV_SIZE_CONTENT);
    lv_obj_set_align(ui_AlertLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_AlertLabel, message);
    lv_obj_set_style_text_font(ui_AlertLabel, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_AlertLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_del_delayed(ui_Alert, 3000);
}

void SignalsFeedbackPanel::hideAlert()
{
    if (ui_Alert) {
        lv_obj_del(ui_Alert);
        ui_Alert = nullptr;
        ui_AlertLabel = nullptr;
    }
}

void SignalsFeedbackPanel::showConfirmationDialog(const char *title,
                                                  const char *message,
                                                  const char *buttons[],
                                                  void *userData,
                                                  lv_event_cb_t callback)
{
    showShadowOverlay();

    lv_obj_t *confirmDialog = lv_msgbox_create(lv_scr_act(), title, message, buttons, true);
    lv_obj_set_width(confirmDialog, 250);
    lv_obj_center(confirmDialog);
    lv_obj_move_foreground(confirmDialog);
    lv_obj_add_event_cb(confirmDialog, callback, LV_EVENT_ALL, userData);
}

bool SignalsFeedbackPanel::isConfirmationAccepted(lv_event_t *e, const char *buttonText) const
{
    if (!e || lv_event_get_code(e) != LV_EVENT_VALUE_CHANGED) {
        return false;
    }

    lv_obj_t *msgbox = lv_event_get_current_target(e);
    const char *activeText = lv_msgbox_get_active_btn_text(msgbox);
    return activeText && strcmp(activeText, buttonText) == 0;
}

void SignalsFeedbackPanel::closeConfirmationDialog(lv_event_t *e)
{
    hideShadowOverlay();

    if (!e) {
        return;
    }

    lv_obj_t *msgbox = lv_event_get_current_target(e);
    if (lv_event_get_code(e) != LV_EVENT_DELETE && msgbox) {
        lv_obj_del(msgbox);
    }
}
