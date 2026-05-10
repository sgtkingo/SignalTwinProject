#ifndef SIGNALS_TOOLBAR_PANEL_HPP
#define SIGNALS_TOOLBAR_PANEL_HPP

#include "lvgl.h"

class SignalsToolbarPanel
{
private:
    lv_obj_t *ui_btnPrev = nullptr;
    lv_obj_t *ui_btnPrevLabel = nullptr;
    lv_obj_t *ui_btnNext = nullptr;
    lv_obj_t *ui_btnNextLabel = nullptr;
    lv_obj_t *ui_btnBackGroup = nullptr;
    lv_obj_t *ui_btnBack = nullptr;
    lv_obj_t *ui_btnBackLabel = nullptr;
    lv_obj_t *ui_btnBackCornerBottomLeft = nullptr;
    lv_obj_t *ui_btnBackCornerTopRight = nullptr;
    lv_obj_t *ui_RecordGroup = nullptr;
    lv_obj_t *ui_RecordCornerTopLeft = nullptr;
    lv_obj_t *ui_RecordCornerFillTopLeft = nullptr;
    lv_obj_t *ui_RecordCornerTopRight = nullptr;
    lv_obj_t *ui_RecordCornerFillTopRight = nullptr;
    lv_obj_t *ui_RecordCornerFillTopRight2 = nullptr;
    lv_obj_t *ui_RecordOutlay = nullptr;
    lv_obj_t *ui_btnPause = nullptr;
    lv_obj_t *ui_pauseImage = nullptr;
    lv_obj_t *ui_btnSync = nullptr;
    lv_obj_t *ui_syncLabel = nullptr;
    lv_obj_t *ui_btnRecord = nullptr;
    lv_obj_t *ui_recordImage = nullptr;
    lv_obj_t *ui_btnClear = nullptr;
    lv_obj_t *ui_clearImage = nullptr;
    lv_obj_t *ui_btnSettings = nullptr;
    lv_obj_t *ui_settingsImage = nullptr;

    static void setButtonEnabled(lv_obj_t *button, bool enabled, lv_color_t enabledColor, lv_color_t disabledColor);

public:
    void create(lv_obj_t *parentWidget,
                void *userData,
                lv_event_cb_t prevButtonCallback,
                lv_event_cb_t nextButtonCallback,
                lv_event_cb_t backButtonCallback,
                lv_event_cb_t pauseButtonCallback,
                lv_event_cb_t syncButtonCallback,
                lv_event_cb_t recordButtonCallback,
                lv_event_cb_t clearButtonCallback,
                lv_event_cb_t settingsButtonCallback);

    void setPaused(bool paused);
    void setNavigationEnabled(bool enabled);
    void setRecordingState(bool canRecord, bool recording);

    lv_obj_t *getRecordGroup() const { return ui_RecordGroup; }
    lv_obj_t *getSettingsButton() const { return ui_btnSettings; }
};

#endif
