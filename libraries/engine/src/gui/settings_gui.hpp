#ifndef SETTINGS_GUI_HPP
#define SETTINGS_GUI_HPP

#include "lvgl.h"

#include "app_settings.hpp"
#include "gui_router.hpp"

class SettingsGui
{
private:
    GuiRouter &router;
    bool initialized = false;

    lv_obj_t *ui_Widget = nullptr;
    lv_obj_t *ui_CurrentMode = nullptr;
    lv_obj_t *ui_ApplicationInput = nullptr;
    lv_obj_t *ui_VersionInput = nullptr;
    lv_obj_t *ui_MetadataStatus = nullptr;

    void build();
    void refresh();
    void addModeButton(const char *text, DefaultCommunicationMode mode, lv_coord_t y);
    lv_obj_t *createSingleLineInput(lv_obj_t *parent, lv_coord_t x, lv_coord_t y, lv_coord_t width);

public:
    explicit SettingsGui(GuiRouter &router);
    ~SettingsGui() = default;

    void init();
    bool isInitialized() const { return initialized; }
    void showSettings();
    void hideSettings();
};

#endif // SETTINGS_GUI_HPP
