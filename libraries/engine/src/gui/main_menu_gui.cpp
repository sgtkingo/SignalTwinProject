#include "main_menu_gui.hpp"

#include "./images/ui_images.h"

#include <string>

namespace
{
const lv_point_t MAIN_MENU_BG_RAY_1[] = {{380, 442}, {235, -12}};
const lv_point_t MAIN_MENU_BG_RAY_2[] = {{380, 442}, {345, -12}};
const lv_point_t MAIN_MENU_BG_RAY_3[] = {{380, 442}, {472, -12}};
const lv_point_t MAIN_MENU_BG_RAY_4[] = {{380, 442}, {625, -12}};
const lv_point_t MAIN_MENU_BG_RAY_5[] = {{-10, 392}, {246, 156}, {330, -12}};
const lv_point_t MAIN_MENU_BG_RAY_6[] = {{770, 364}, {530, 154}, {454, -12}};
const lv_point_t MAIN_MENU_BG_RAY_7[] = {{155, 442}, {282, 248}, {332, 128}};
const lv_point_t MAIN_MENU_BG_RAY_8[] = {{610, 442}, {504, 250}, {466, 126}};

void addMainMenuBackgroundRay(lv_obj_t *parentWidget,
                              const lv_point_t points[],
                              uint16_t pointCount,
                              uint32_t color,
                              lv_coord_t width,
                              lv_opa_t opacity)
{
    lv_obj_t *ray = lv_line_create(parentWidget);
    lv_line_set_points(ray, points, pointCount);
    lv_obj_set_style_line_color(ray, lv_color_hex(color), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_width(ray, width, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(ray, opacity, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_rounded(ray, true, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_clear_flag(ray, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);
}

void addMainMenuBackground(lv_obj_t *parentWidget)
{
    addMainMenuBackgroundRay(parentWidget, MAIN_MENU_BG_RAY_1, 2, 0xE0E5EC, 24, 170);
    addMainMenuBackgroundRay(parentWidget, MAIN_MENU_BG_RAY_2, 2, 0xECEFF4, 18, 220);
    addMainMenuBackgroundRay(parentWidget, MAIN_MENU_BG_RAY_3, 2, 0xDDE3EA, 20, 170);
    addMainMenuBackgroundRay(parentWidget, MAIN_MENU_BG_RAY_4, 2, 0xEEF1F5, 26, 220);
    addMainMenuBackgroundRay(parentWidget, MAIN_MENU_BG_RAY_5, 3, 0xE4E9EF, 16, 200);
    addMainMenuBackgroundRay(parentWidget, MAIN_MENU_BG_RAY_6, 3, 0xD8DFE8, 14, 180);
    addMainMenuBackgroundRay(parentWidget, MAIN_MENU_BG_RAY_7, 3, 0xF0F2F6, 20, 230);
    addMainMenuBackgroundRay(parentWidget, MAIN_MENU_BG_RAY_8, 3, 0xE3E8EF, 18, 190);
}

void addMtaLogoPanelToWidget(lv_obj_t *parentWidget)
{
    lv_obj_t *logoGroup = lv_obj_create(parentWidget);
    lv_obj_remove_style_all(logoGroup);
    lv_obj_set_width(logoGroup, 100);
    lv_obj_set_height(logoGroup, 25);
    lv_obj_set_align(logoGroup, LV_ALIGN_BOTTOM_MID);
    lv_obj_clear_flag(logoGroup, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t *logoCornerBottomLeft = lv_obj_create(logoGroup);
    lv_obj_remove_style_all(logoCornerBottomLeft);
    lv_obj_set_width(logoCornerBottomLeft, 20);
    lv_obj_set_height(logoCornerBottomLeft, 10);
    lv_obj_set_align(logoCornerBottomLeft, LV_ALIGN_BOTTOM_LEFT);
    lv_obj_clear_flag(logoCornerBottomLeft, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(logoCornerBottomLeft, lv_color_hex(0x055DA9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(logoCornerBottomLeft, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(logoCornerBottomLeft, false, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t *logoCornerFillBottomLeft = lv_obj_create(logoGroup);
    lv_obj_remove_style_all(logoCornerFillBottomLeft);
    lv_obj_set_width(logoCornerFillBottomLeft, 15);
    lv_obj_set_height(logoCornerFillBottomLeft, 15);
    lv_obj_set_x(logoCornerFillBottomLeft, -5);
    lv_obj_set_y(logoCornerFillBottomLeft, 0);
    lv_obj_set_align(logoCornerFillBottomLeft, LV_ALIGN_BOTTOM_LEFT);
    lv_obj_clear_flag(logoCornerFillBottomLeft, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_radius(logoCornerFillBottomLeft, 1000, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(logoCornerFillBottomLeft, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(logoCornerFillBottomLeft, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(logoCornerFillBottomLeft, false, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t *logoCornerBottomRight = lv_obj_create(logoGroup);
    lv_obj_remove_style_all(logoCornerBottomRight);
    lv_obj_set_width(logoCornerBottomRight, 20);
    lv_obj_set_height(logoCornerBottomRight, 10);
    lv_obj_set_align(logoCornerBottomRight, LV_ALIGN_BOTTOM_RIGHT);
    lv_obj_clear_flag(logoCornerBottomRight, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(logoCornerBottomRight, lv_color_hex(0x055DA9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(logoCornerBottomRight, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(logoCornerBottomRight, false, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t *logoCornerFillBottomRight = lv_obj_create(logoGroup);
    lv_obj_remove_style_all(logoCornerFillBottomRight);
    lv_obj_set_width(logoCornerFillBottomRight, 15);
    lv_obj_set_height(logoCornerFillBottomRight, 15);
    lv_obj_set_x(logoCornerFillBottomRight, 5);
    lv_obj_set_y(logoCornerFillBottomRight, 0);
    lv_obj_set_align(logoCornerFillBottomRight, LV_ALIGN_BOTTOM_RIGHT);
    lv_obj_clear_flag(logoCornerFillBottomRight, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_radius(logoCornerFillBottomRight, 1000, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(logoCornerFillBottomRight, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(logoCornerFillBottomRight, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(logoCornerFillBottomRight, false, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t *logoOutlay = lv_obj_create(logoGroup);
    lv_obj_remove_style_all(logoOutlay);
    lv_obj_set_width(logoOutlay, 80);
    lv_obj_set_height(logoOutlay, 25);
    lv_obj_set_align(logoOutlay, LV_ALIGN_CENTER);
    lv_obj_clear_flag(logoOutlay, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_radius(logoOutlay, 1000, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(logoOutlay, lv_color_hex(0x055DA9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(logoOutlay, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(logoOutlay, false, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t *logoImage = lv_img_create(logoGroup);
    lv_img_set_src(logoImage, &ui_img_mtalogo_png);
    lv_obj_set_width(logoImage, LV_SIZE_CONTENT);
    lv_obj_set_height(logoImage, LV_SIZE_CONTENT);
    lv_obj_set_align(logoImage, LV_ALIGN_CENTER);
    lv_obj_clear_flag(logoImage, LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE |
                                     LV_OBJ_FLAG_SNAPPABLE | LV_OBJ_FLAG_SCROLLABLE);
    lv_img_set_zoom(logoImage, 80);
}
}

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

    addMainMenuBackground(ui_Widget);

    ui_Title = lv_label_create(ui_Widget);
    lv_label_set_text(ui_Title, "Welcome");
    lv_obj_align(ui_Title, LV_ALIGN_TOP_MID, 0, 16);
    lv_obj_set_style_text_font(ui_Title, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_btnFileTransfer = createIconButton(ui_Widget, &ui_img_export_png, LV_ALIGN_TOP_LEFT, 14, 14);
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
    lv_obj_set_style_bg_color(ui_btnAbout, lv_color_hex(0x808080), LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_btnVisualization = createMenuButton(ui_Widget, "Visualization", LV_ALIGN_TOP_MID, 0, 86);
    lv_obj_add_event_cb(ui_btnVisualization, [](lv_event_t *e) {
        if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
            auto *self = static_cast<MainMenuGui *>(lv_event_get_user_data(e));
            self->router.openVisualizationFlow();
        }
    }, LV_EVENT_ALL, this);;
    lv_obj_set_size(ui_btnVisualization, 300, 128);

    ui_btnLibrary = createMenuButton(ui_Widget, "Library", LV_ALIGN_CENTER, -125, 52);
    lv_obj_add_event_cb(ui_btnLibrary, [](lv_event_t *e) {
        if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
            auto *self = static_cast<MainMenuGui *>(lv_event_get_user_data(e));
            self->router.showLibrary();
        }
    }, LV_EVENT_ALL, this);
    lv_obj_set_style_bg_color(ui_btnLibrary, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_btnDatabank = createMenuButton(ui_Widget, "Databank", LV_ALIGN_CENTER, 125, 52);
    lv_obj_add_event_cb(ui_btnDatabank, [](lv_event_t *e) {
        if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
            auto *self = static_cast<MainMenuGui *>(lv_event_get_user_data(e));
            self->router.openDatabankFromMainMenu();
        }
    }, LV_EVENT_ALL, this);
    lv_obj_set_style_bg_color(ui_btnDatabank, lv_color_hex(0xF77A05), LV_PART_MAIN | LV_STATE_DEFAULT);

    addMtaLogoPanelToWidget(ui_Widget);

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
