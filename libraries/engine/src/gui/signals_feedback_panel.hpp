#ifndef SIGNALS_FEEDBACK_PANEL_HPP
#define SIGNALS_FEEDBACK_PANEL_HPP

#include "lvgl.h"

class SignalsFeedbackPanel
{
private:
    lv_obj_t *ui_ShadowOverlay = nullptr;
    lv_obj_t *ui_Alert = nullptr;
    lv_obj_t *ui_AlertLabel = nullptr;

public:
    void showShadowOverlay();
    void hideShadowOverlay();

    void showAlert(lv_obj_t *parentWidget, void *userData, const char *message, lv_event_cb_t dismissCallback);
    void hideAlert();
};

#endif
