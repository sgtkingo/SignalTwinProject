#ifndef MAIN_MENU_GUI_HPP
#define MAIN_MENU_GUI_HPP

#include "lvgl.h"

#include "gui_callbacks.hpp"

class MainMenuGui
{
private:
    bool initialized = false;

    lv_obj_t *ui_Widget = nullptr;
    lv_obj_t *ui_Title = nullptr;
    lv_obj_t *ui_btnVisualization = nullptr;
    lv_obj_t *ui_btnLibrary = nullptr;
    lv_obj_t *ui_btnDatabank = nullptr;
    lv_obj_t *ui_btnSettings = nullptr;

    void build();
    lv_obj_t *createMenuButton(lv_obj_t *parent, const char *text, lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs);

public:
    MainMenuGui() = default;
    ~MainMenuGui() = default;

    void init();
    bool isInitialized() const { return initialized; }
    void showMainMenu();
    void hideMainMenu();
};

#endif // MAIN_MENU_GUI_HPP
