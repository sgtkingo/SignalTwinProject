#ifndef MAIN_MENU_GUI_HPP
#define MAIN_MENU_GUI_HPP

#include "lvgl.h"

#include "gui_router.hpp"

class MainMenuGui
{
private:
    GuiRouter &router;
    bool initialized = false;

    lv_obj_t *ui_Widget = nullptr;
    lv_obj_t *ui_Title = nullptr;
    lv_obj_t *ui_btnVisualization = nullptr;
    lv_obj_t *ui_btnLibrary = nullptr;
    lv_obj_t *ui_btnDatabank = nullptr;
    lv_obj_t *ui_btnFileTransfer = nullptr;
    lv_obj_t *ui_btnSettings = nullptr;
    lv_obj_t *ui_btnAbout = nullptr;
    lv_obj_t *ui_VersionLabel = nullptr;

    void build();
    void refresh();
    lv_obj_t *createMenuButton(lv_obj_t *parent, const char *text, lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs);
    lv_obj_t *createIconButton(lv_obj_t *parent, const void *imageSource, lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs);

public:
    explicit MainMenuGui(GuiRouter &router);
    ~MainMenuGui() = default;

    void init();
    bool isInitialized() const { return initialized; }
    void showMainMenu();
    void hideMainMenu();
};

#endif // MAIN_MENU_GUI_HPP
