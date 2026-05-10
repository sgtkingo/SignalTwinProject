#include "signals_toolbar_panel.hpp"

#include "./images/ui_images.h"

namespace
{
constexpr uint32_t ENABLED_COLOR = 0x009BFF;
constexpr uint32_t DISABLED_COLOR = 0x949494;
constexpr uint32_t ACTIVE_COLOR = 0xE55858;
constexpr uint32_t PANEL_COLOR = 0x055DA9;
}

void SignalsToolbarPanel::setButtonEnabled(lv_obj_t *button, bool enabled, lv_color_t enabledColor, lv_color_t disabledColor)
{
    if (!button) {
        return;
    }

    lv_obj_set_style_bg_color(button, enabled ? enabledColor : disabledColor, LV_PART_MAIN | LV_STATE_DEFAULT);
    if (enabled) {
        lv_obj_add_flag(button, LV_OBJ_FLAG_CLICKABLE);
    } else {
        lv_obj_clear_flag(button, LV_OBJ_FLAG_CLICKABLE);
    }
}

void SignalsToolbarPanel::create(lv_obj_t *parentWidget,
                                 void *userData,
                                 lv_event_cb_t prevButtonCallback,
                                 lv_event_cb_t nextButtonCallback,
                                 lv_event_cb_t backButtonCallback,
                                 lv_event_cb_t pauseButtonCallback,
                                 lv_event_cb_t syncButtonCallback,
                                 lv_event_cb_t recordButtonCallback,
                                 lv_event_cb_t clearButtonCallback,
                                 lv_event_cb_t settingsButtonCallback)
{
    if (!parentWidget) {
        return;
    }

    ui_btnPrev = lv_btn_create(parentWidget);
    lv_obj_set_width(ui_btnPrev, 80);
    lv_obj_set_height(ui_btnPrev, 40);
    lv_obj_set_x(ui_btnPrev, 35);
    lv_obj_set_y(ui_btnPrev, -40);
    lv_obj_set_align(ui_btnPrev, LV_ALIGN_BOTTOM_LEFT);
    lv_obj_add_event_cb(ui_btnPrev, prevButtonCallback, LV_EVENT_CLICKED, userData);

    ui_btnPrevLabel = lv_label_create(ui_btnPrev);
    lv_label_set_text(ui_btnPrevLabel, "Prev");
    lv_obj_center(ui_btnPrevLabel);
    lv_obj_set_style_text_font(ui_btnPrevLabel, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_btnNext = lv_btn_create(parentWidget);
    lv_obj_set_width(ui_btnNext, 80);
    lv_obj_set_height(ui_btnNext, 40);
    lv_obj_set_x(ui_btnNext, 183);
    lv_obj_set_y(ui_btnNext, -40);
    lv_obj_set_align(ui_btnNext, LV_ALIGN_BOTTOM_LEFT);
    lv_obj_add_event_cb(ui_btnNext, nextButtonCallback, LV_EVENT_CLICKED, userData);

    ui_btnNextLabel = lv_label_create(ui_btnNext);
    lv_label_set_text(ui_btnNextLabel, "Next");
    lv_obj_center(ui_btnNextLabel);
    lv_obj_set_style_text_font(ui_btnNextLabel, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_btnBackGroup = lv_obj_create(parentWidget);
    lv_obj_remove_style_all(ui_btnBackGroup);
    lv_obj_set_width(ui_btnBackGroup, 100);
    lv_obj_set_height(ui_btnBackGroup, 40);
    lv_obj_clear_flag(ui_btnBackGroup, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);

    ui_btnBackCornerBottomLeft = lv_obj_create(ui_btnBackGroup);
    lv_obj_remove_style_all(ui_btnBackCornerBottomLeft);
    lv_obj_set_width(ui_btnBackCornerBottomLeft, 20);
    lv_obj_set_height(ui_btnBackCornerBottomLeft, 20);
    lv_obj_set_align(ui_btnBackCornerBottomLeft, LV_ALIGN_BOTTOM_LEFT);
    lv_obj_clear_flag(ui_btnBackCornerBottomLeft, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(ui_btnBackCornerBottomLeft, lv_color_hex(ENABLED_COLOR), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_btnBackCornerBottomLeft, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui_btnBackCornerBottomLeft, false, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_btnBackCornerTopRight = lv_obj_create(ui_btnBackGroup);
    lv_obj_remove_style_all(ui_btnBackCornerTopRight);
    lv_obj_set_width(ui_btnBackCornerTopRight, 20);
    lv_obj_set_height(ui_btnBackCornerTopRight, 20);
    lv_obj_set_align(ui_btnBackCornerTopRight, LV_ALIGN_TOP_RIGHT);
    lv_obj_clear_flag(ui_btnBackCornerTopRight, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(ui_btnBackCornerTopRight, lv_color_hex(ENABLED_COLOR), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_btnBackCornerTopRight, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui_btnBackCornerTopRight, false, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_btnBack = lv_btn_create(ui_btnBackGroup);
    lv_obj_set_width(ui_btnBack, 100);
    lv_obj_set_height(ui_btnBack, 40);
    lv_obj_set_align(ui_btnBack, LV_ALIGN_CENTER);
    lv_obj_add_event_cb(ui_btnBack, backButtonCallback, LV_EVENT_CLICKED, userData);

    ui_btnBackLabel = lv_label_create(ui_btnBack);
    lv_label_set_text(ui_btnBackLabel, "Back");
    lv_obj_center(ui_btnBackLabel);
    lv_obj_set_style_text_font(ui_btnBackLabel, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_RecordGroup = lv_obj_create(parentWidget);
    lv_obj_remove_style_all(ui_RecordGroup);
    lv_obj_set_width(ui_RecordGroup, 195);
    lv_obj_set_height(ui_RecordGroup, 45);
    lv_obj_set_x(ui_RecordGroup, -40);
    lv_obj_set_y(ui_RecordGroup, 0);
    lv_obj_set_align(ui_RecordGroup, LV_ALIGN_TOP_RIGHT);
    lv_obj_clear_flag(ui_RecordGroup, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);

    ui_RecordCornerTopLeft = lv_obj_create(ui_RecordGroup);
    lv_obj_remove_style_all(ui_RecordCornerTopLeft);
    lv_obj_set_width(ui_RecordCornerTopLeft, 40);
    lv_obj_set_height(ui_RecordCornerTopLeft, 20);
    lv_obj_clear_flag(ui_RecordCornerTopLeft, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(ui_RecordCornerTopLeft, lv_color_hex(PANEL_COLOR), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_RecordCornerTopLeft, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui_RecordCornerTopLeft, false, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_RecordCornerFillTopLeft = lv_obj_create(ui_RecordGroup);
    lv_obj_remove_style_all(ui_RecordCornerFillTopLeft);
    lv_obj_set_width(ui_RecordCornerFillTopLeft, 30);
    lv_obj_set_height(ui_RecordCornerFillTopLeft, 40);
    lv_obj_set_x(ui_RecordCornerFillTopLeft, -20);
    lv_obj_clear_flag(ui_RecordCornerFillTopLeft, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_radius(ui_RecordCornerFillTopLeft, 1000, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_RecordCornerFillTopLeft, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_RecordCornerFillTopLeft, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui_RecordCornerFillTopLeft, false, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_RecordCornerTopRight = lv_obj_create(ui_RecordGroup);
    lv_obj_remove_style_all(ui_RecordCornerTopRight);
    lv_obj_set_width(ui_RecordCornerTopRight, 40);
    lv_obj_set_height(ui_RecordCornerTopRight, 20);
    lv_obj_set_align(ui_RecordCornerTopRight, LV_ALIGN_TOP_RIGHT);
    lv_obj_clear_flag(ui_RecordCornerTopRight, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(ui_RecordCornerTopRight, lv_color_hex(PANEL_COLOR), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_RecordCornerTopRight, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui_RecordCornerTopRight, false, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_RecordCornerFillTopRight = lv_obj_create(ui_RecordGroup);
    lv_obj_remove_style_all(ui_RecordCornerFillTopRight);
    lv_obj_set_width(ui_RecordCornerFillTopRight, 26);
    lv_obj_set_height(ui_RecordCornerFillTopRight, 26);
    lv_obj_set_x(ui_RecordCornerFillTopRight, 16);
    lv_obj_set_align(ui_RecordCornerFillTopRight, LV_ALIGN_TOP_RIGHT);
    lv_obj_clear_flag(ui_RecordCornerFillTopRight, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_radius(ui_RecordCornerFillTopRight, 1000, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_RecordCornerFillTopRight, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_RecordCornerFillTopRight, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui_RecordCornerFillTopRight, false, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_RecordCornerFillTopRight2 = lv_obj_create(ui_RecordGroup);
    lv_obj_remove_style_all(ui_RecordCornerFillTopRight2);
    lv_obj_set_width(ui_RecordCornerFillTopRight2, 10);
    lv_obj_set_height(ui_RecordCornerFillTopRight2, 10);
    lv_obj_set_y(ui_RecordCornerFillTopRight2, 13);
    lv_obj_set_align(ui_RecordCornerFillTopRight2, LV_ALIGN_TOP_RIGHT);
    lv_obj_clear_flag(ui_RecordCornerFillTopRight2, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_radius(ui_RecordCornerFillTopRight2, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_RecordCornerFillTopRight2, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_RecordCornerFillTopRight2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui_RecordCornerFillTopRight2, false, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_RecordOutlay = lv_obj_create(ui_RecordGroup);
    lv_obj_remove_style_all(ui_RecordOutlay);
    lv_obj_set_width(ui_RecordOutlay, 175);
    lv_obj_set_height(ui_RecordOutlay, 45);
    lv_obj_set_align(ui_RecordOutlay, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_RecordOutlay, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_radius(ui_RecordOutlay, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_RecordOutlay, lv_color_hex(PANEL_COLOR), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_RecordOutlay, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui_RecordOutlay, false, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_btnPause = lv_btn_create(ui_RecordGroup);
    lv_obj_set_width(ui_btnPause, 37);
    lv_obj_set_height(ui_btnPause, 35);
    lv_obj_set_x(ui_btnPause, 15);
    lv_obj_set_y(ui_btnPause, -1);
    lv_obj_set_align(ui_btnPause, LV_ALIGN_LEFT_MID);
    lv_obj_add_flag(ui_btnPause, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_clear_flag(ui_btnPause, LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE |
                                       LV_OBJ_FLAG_SNAPPABLE | LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                                       LV_OBJ_FLAG_SCROLL_CHAIN);
    lv_obj_set_style_radius(ui_btnPause, 7, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(ui_btnPause, pauseButtonCallback, LV_EVENT_CLICKED, userData);

    ui_pauseImage = lv_img_create(ui_btnPause);
    lv_img_set_src(ui_pauseImage, &ui_img_playpauseicon_png);
    lv_obj_set_width(ui_pauseImage, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_pauseImage, LV_SIZE_CONTENT);
    lv_obj_set_x(ui_pauseImage, -1);
    lv_obj_set_align(ui_pauseImage, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_pauseImage, LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE |
                                         LV_OBJ_FLAG_SNAPPABLE | LV_OBJ_FLAG_SCROLLABLE);
    lv_img_set_zoom(ui_pauseImage, 119);

    ui_btnSync = lv_btn_create(ui_RecordGroup);
    lv_obj_set_width(ui_btnSync, 37);
    lv_obj_set_height(ui_btnSync, 35);
    lv_obj_set_x(ui_btnSync, 57);
    lv_obj_set_y(ui_btnSync, -1);
    lv_obj_set_align(ui_btnSync, LV_ALIGN_LEFT_MID);
    lv_obj_add_flag(ui_btnSync, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_clear_flag(ui_btnSync, LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE |
                                      LV_OBJ_FLAG_SNAPPABLE | LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                                      LV_OBJ_FLAG_SCROLL_CHAIN);
    lv_obj_set_style_radius(ui_btnSync, 7, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(ui_btnSync, syncButtonCallback, LV_EVENT_CLICKED, userData);

    ui_syncLabel = lv_label_create(ui_btnSync);
    lv_label_set_text(ui_syncLabel, LV_SYMBOL_REFRESH);
    lv_obj_center(ui_syncLabel);
    lv_obj_set_style_text_font(ui_syncLabel, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_btnRecord = lv_btn_create(ui_RecordGroup);
    lv_obj_set_width(ui_btnRecord, 37);
    lv_obj_set_height(ui_btnRecord, 35);
    lv_obj_set_x(ui_btnRecord, 99);
    lv_obj_set_y(ui_btnRecord, -1);
    lv_obj_set_align(ui_btnRecord, LV_ALIGN_LEFT_MID);
    lv_obj_add_flag(ui_btnRecord, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_clear_flag(ui_btnRecord, LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE |
                                        LV_OBJ_FLAG_SNAPPABLE | LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                                        LV_OBJ_FLAG_SCROLL_CHAIN);
    lv_obj_set_style_radius(ui_btnRecord, 7, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(ui_btnRecord, recordButtonCallback, LV_EVENT_CLICKED, userData);

    ui_recordImage = lv_img_create(ui_btnRecord);
    lv_img_set_src(ui_recordImage, &ui_img_recordicon_png);
    lv_obj_set_width(ui_recordImage, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_recordImage, LV_SIZE_CONTENT);
    lv_obj_set_x(ui_recordImage, -1);
    lv_obj_set_align(ui_recordImage, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_recordImage, LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE |
                                          LV_OBJ_FLAG_SNAPPABLE | LV_OBJ_FLAG_SCROLLABLE);
    lv_img_set_zoom(ui_recordImage, 119);

    ui_btnClear = lv_btn_create(ui_RecordGroup);
    lv_obj_set_width(ui_btnClear, 37);
    lv_obj_set_height(ui_btnClear, 35);
    lv_obj_set_x(ui_btnClear, 141);
    lv_obj_set_y(ui_btnClear, -1);
    lv_obj_set_align(ui_btnClear, LV_ALIGN_LEFT_MID);
    lv_obj_add_flag(ui_btnClear, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_clear_flag(ui_btnClear, LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE |
                                       LV_OBJ_FLAG_SNAPPABLE | LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                                       LV_OBJ_FLAG_SCROLL_CHAIN);
    lv_obj_set_style_radius(ui_btnClear, 7, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(ui_btnClear, clearButtonCallback, LV_EVENT_CLICKED, userData);

    ui_clearImage = lv_img_create(ui_btnClear);
    lv_img_set_src(ui_clearImage, &ui_img_trashicon_png);
    lv_obj_set_width(ui_clearImage, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_clearImage, LV_SIZE_CONTENT);
    lv_obj_set_x(ui_clearImage, -1);
    lv_obj_set_align(ui_clearImage, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_clearImage, LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE |
                                         LV_OBJ_FLAG_SNAPPABLE | LV_OBJ_FLAG_SCROLLABLE);
    lv_img_set_zoom(ui_clearImage, 119);

    ui_btnSettings = lv_btn_create(parentWidget);
    lv_obj_set_width(ui_btnSettings, 37);
    lv_obj_set_height(ui_btnSettings, 36);
    lv_obj_set_x(ui_btnSettings, -7);
    lv_obj_set_y(ui_btnSettings, 4);
    lv_obj_set_align(ui_btnSettings, LV_ALIGN_TOP_RIGHT);
    lv_obj_add_flag(ui_btnSettings, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_clear_flag(ui_btnSettings, LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE |
                                          LV_OBJ_FLAG_SNAPPABLE | LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                                          LV_OBJ_FLAG_SCROLL_CHAIN);
    lv_obj_set_style_radius(ui_btnSettings, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(ui_btnSettings, settingsButtonCallback, LV_EVENT_CLICKED, userData);

    ui_settingsImage = lv_img_create(ui_btnSettings);
    lv_img_set_src(ui_settingsImage, &ui_img_settings_png);
    lv_obj_set_width(ui_settingsImage, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_settingsImage, LV_SIZE_CONTENT);
    lv_obj_set_x(ui_settingsImage, -1);
    lv_obj_set_align(ui_settingsImage, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_settingsImage, LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE |
                                            LV_OBJ_FLAG_SNAPPABLE | LV_OBJ_FLAG_SCROLLABLE);
    lv_img_set_zoom(ui_settingsImage, 119);

    setPaused(false);
    setNavigationEnabled(true);
    setRecordingState(false, false);
}

void SignalsToolbarPanel::setPaused(bool paused)
{
    if (ui_btnPause) {
        lv_obj_set_style_bg_color(ui_btnPause, lv_color_hex(paused ? ACTIVE_COLOR : ENABLED_COLOR), LV_PART_MAIN | LV_STATE_DEFAULT);
    }

    setButtonEnabled(ui_btnSync, paused, lv_color_hex(ENABLED_COLOR), lv_color_hex(DISABLED_COLOR));
}

void SignalsToolbarPanel::setNavigationEnabled(bool enabled)
{
    setButtonEnabled(ui_btnPrev, enabled, lv_color_hex(ENABLED_COLOR), lv_color_hex(DISABLED_COLOR));
    setButtonEnabled(ui_btnNext, enabled, lv_color_hex(ENABLED_COLOR), lv_color_hex(DISABLED_COLOR));
}

void SignalsToolbarPanel::setRecordingState(bool canRecord, bool recording)
{
    setButtonEnabled(
        ui_btnRecord,
        canRecord,
        lv_color_hex(recording ? ACTIVE_COLOR : ENABLED_COLOR),
        lv_color_hex(DISABLED_COLOR));
    setButtonEnabled(ui_btnClear, canRecord, lv_color_hex(ENABLED_COLOR), lv_color_hex(DISABLED_COLOR));
    setNavigationEnabled(!recording);
}
