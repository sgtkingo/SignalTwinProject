#ifndef SIGNALS_SETTINGS_PANEL_HPP
#define SIGNALS_SETTINGS_PANEL_HPP

#include "lvgl.h"

class SignalsSettingsPanel
{
private:
    lv_obj_t *ui_SettingsOverlay = nullptr;
    lv_obj_t *ui_SettingsBridgeGroup = nullptr;
    lv_obj_t *ui_SettingsBridge = nullptr;
    lv_obj_t *ui_SettingsBridgeFill = nullptr;
    lv_obj_t *ui_SettingsGroup = nullptr;
    lv_obj_t *ui_SettingsOutlay = nullptr;
    lv_obj_t *ui_SettingsHeaderLine = nullptr;
    lv_obj_t *ui_SettingsHeaderLabel = nullptr;
    lv_obj_t *ui_SettingsDataBundleLabel = nullptr;
    lv_obj_t *ui_SettingsDataBundleCountLabel = nullptr;
    lv_obj_t *ui_SettingsDataBundleShowButton = nullptr;
    lv_obj_t *ui_SettingsDataBundleShowButtonLabel = nullptr;
    lv_obj_t *ui_SettingsDataBundleDeleteAllButton = nullptr;
    lv_obj_t *ui_SettingsDataBundleDeleteAllButtonLabel = nullptr;
    lv_obj_t *ui_SettingsCreditsLabel = nullptr;
    lv_obj_t *ui_SettingsCreditsButton = nullptr;
    lv_obj_t *ui_SettingsCreditsButtonLabel = nullptr;

public:
    bool isVisible() const { return ui_SettingsOverlay != nullptr; }

    void show(lv_obj_t *parentWidget,
              lv_obj_t *recordGroup,
              lv_obj_t *btnSettings,
              void *userData,
              uint32_t bundleAmount,
              bool isBundleFull,
              lv_event_cb_t closeCallback,
              lv_event_cb_t showBundlesCallback,
              lv_event_cb_t deleteAllCallback,
              lv_event_cb_t creditsCallback);

    void hide();
};

#endif
