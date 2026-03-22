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

    void build();
    void refresh();
    void addModeButton(const char *text, DefaultCommunicationMode mode, lv_coord_t y);

public:
    explicit SettingsGui(GuiRouter &router);
    ~SettingsGui() = default;

    void init();
    bool isInitialized() const { return initialized; }
    void showSettings();
    void hideSettings();
};

#endif // SETTINGS_GUI_HPP
