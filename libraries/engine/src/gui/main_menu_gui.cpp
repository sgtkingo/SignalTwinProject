#include "main_menu_gui.hpp"

#include "./images/ui_images.h"

#include <string>

MainMenuGui::MainMenuGui(GuiRouter &router) : router(router)
{
}

lv_obj_t *MainMenuGui::createMenuButton(lv_obj_t *parent, const char *text, lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs)
{
    lv_obj_t *button = lv_btn_create(parent);
    lv_obj_set_size(button, 230, 64);
    lv_obj_align(button, align, x_ofs, y_ofs);
    lv_obj_set_style_radius(button, 8, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t *label = lv_label_create(button);
    lv_label_set_text(label, text);
    lv_obj_center(label);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);

    return button;
}

lv_obj_t *MainMenuGui::createIconButton(lv_obj_t *parent, const void *imageSource, lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs)
{
    lv_obj_t *button = lv_btn_create(parent);
    lv_obj_set_size(button, 38, 38);
    lv_obj_align(button, align, x_ofs, y_ofs);
    lv_obj_set_style_radius(button, 19, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t *image = lv_img_create(button);
    lv_img_set_src(image, imageSource);
    lv_img_set_zoom(image, 120);
    lv_obj_center(image);
    lv_obj_clear_flag(image, LV_OBJ_FLAG_CLICKABLE);

    return button;
}

void MainMenuGui::build()
{
    ui_Widget = lv_obj_create(lv_scr_act());
    lv_obj_remove_style_all(ui_Widget);
    lv_obj_set_size(ui_Widget, 760, 440);
    lv_obj_set_align(ui_Widget, LV_ALIGN_CENTER);
    lv_obj_set_style_radius(ui_Widget, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Widget, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Widget, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_Widget, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_Widget, 2, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Title = lv_label_create(ui_Widget);
    lv_label_set_text(ui_Title, "Main Menu");
    lv_obj_align(ui_Title, LV_ALIGN_TOP_MID, 0, 16);
    lv_obj_set_style_text_font(ui_Title, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_btnFileTransfer = createIconButton(ui_Widget, &ui_img_cable_png, LV_ALIGN_TOP_LEFT, 14, 14);
    lv_obj_add_event_cb(ui_btnFileTransfer, [](lv_event_t *e) {
        if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
            auto *self = static_cast<MainMenuGui *>(lv_event_get_user_data(e));
            self->router.showFileTransfer();
        }
    }, LV_EVENT_ALL, this);

    ui_btnSettings = createIconButton(ui_Widget, &ui_img_settings_png, LV_ALIGN_TOP_RIGHT, -14, 14);
    lv_obj_add_event_cb(ui_btnSettings, [](lv_event_t *e) {
        if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
            auto *self = static_cast<MainMenuGui *>(lv_event_get_user_data(e));
            self->router.showSettings();
        }
    }, LV_EVENT_ALL, this);

    ui_btnAbout = createIconButton(ui_Widget, &ui_img_info_png, LV_ALIGN_TOP_RIGHT, -58, 14);
    lv_obj_add_event_cb(ui_btnAbout, [](lv_event_t *e) {
        if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
            auto *self = static_cast<MainMenuGui *>(lv_event_get_user_data(e));
            self->router.showCreditsScreen();
        }
    }, LV_EVENT_ALL, this);

    ui_btnVisualization = createMenuButton(ui_Widget, "Visualization", LV_ALIGN_TOP_MID, 0, 86);
    lv_obj_add_event_cb(ui_btnVisualization, [](lv_event_t *e) {
        if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
            auto *self = static_cast<MainMenuGui *>(lv_event_get_user_data(e));
            self->router.openVisualizationFlow();
        }
    }, LV_EVENT_ALL, this);

    ui_btnLibrary = createMenuButton(ui_Widget, "Library", LV_ALIGN_CENTER, -125, 52);
    lv_obj_add_event_cb(ui_btnLibrary, [](lv_event_t *e) {
        if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
            auto *self = static_cast<MainMenuGui *>(lv_event_get_user_data(e));
            self->router.showLibrary();
        }
    }, LV_EVENT_ALL, this);

    ui_btnDatabank = createMenuButton(ui_Widget, "Databank", LV_ALIGN_CENTER, 125, 52);
    lv_obj_add_event_cb(ui_btnDatabank, [](lv_event_t *e) {
        if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
            auto *self = static_cast<MainMenuGui *>(lv_event_get_user_data(e));
            self->router.openDatabankFromMainMenu();
        }
    }, LV_EVENT_ALL, this);

    ui_VersionLabel = lv_label_create(ui_Widget);
    lv_obj_align(ui_VersionLabel, LV_ALIGN_BOTTOM_LEFT, 18, -14);
    lv_obj_set_style_text_color(ui_VersionLabel, lv_color_hex(0x6C7680), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_VersionLabel, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text(ui_VersionLabel, "");
}

void MainMenuGui::refresh()
{
    if (!ui_VersionLabel) {
        return;
    }

    const std::string version = router.getAppVersion();
    if (version.empty()) {
        lv_label_set_text(ui_VersionLabel, "");
        return;
    }

    const std::string label = "v" + version;
    lv_label_set_text(ui_VersionLabel, label.c_str());
}

void MainMenuGui::init()
{
    if (initialized) {
        return;
    }

    build();
    initialized = true;
}

void MainMenuGui::showMainMenu()
{
    if (!initialized || !ui_Widget) {
        return;
    }

    refresh();
    lv_obj_clear_flag(ui_Widget, LV_OBJ_FLAG_HIDDEN);
}

void MainMenuGui::hideMainMenu()
{
    if (!initialized || !ui_Widget) {
        return;
    }

    lv_obj_add_flag(ui_Widget, LV_OBJ_FLAG_HIDDEN);
}
