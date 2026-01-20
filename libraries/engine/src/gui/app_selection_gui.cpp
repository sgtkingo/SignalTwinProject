/**
 * @file app_selection.cpp
 * @brief Implementation of the AppSelectionGui class
 * This source file implements the AppSelectionGui functionality for
 * application selection.
 * 
 * @copyright 2025 MTA
 * @author Ondřej Wrubel
 */

#include "app_selection_gui.hpp"
#include "./images/ui_images.h"

AppSelectionGui::AppSelectionGui()
{
    lv_obj_t *ui_Widget = nullptr;
    lv_obj_t *ui_LogoGroup = nullptr;
    lv_obj_t *ui_LogoCornerBottomLeft = nullptr;
    lv_obj_t *ui_LogoCornerFillBottomLeft = nullptr;
    lv_obj_t *ui_LogoCornerBottomRight = nullptr;
    lv_obj_t *ui_LogoCornerFillBottomRight = nullptr;
    lv_obj_t *ui_LogoOutlay = nullptr;
    lv_obj_t *ui_LogoImage = nullptr;
    lv_obj_t *ui_EduPanelLabel = nullptr;
    lv_obj_t *ui_AppGroup = nullptr;
    lv_obj_t *ui_AppImage = nullptr;
    lv_obj_t *ui_AppLabel = nullptr;
    lv_obj_t *ui_ComingSoonGroup = nullptr;
    lv_obj_t *ui_ComingSoonImage = nullptr;
    lv_obj_t *ui_ComingSoonLabel = nullptr;
}

void AppSelectionGui::init()
{
    if (initialized)
        return;

    try
    {
        // // logMessage("Initializing AppSelectionGui...\n");
        constructAppSelection();
        initialized = true;
        // // logMessage("AppSelectionGui initialization completed!\n");
    }
    catch (const std::exception &e)
    {
        // // logMessage("AppSelectionGui initialization failed: %s\n", e.what());
        initialized = false;
    }
}

void AppSelectionGui::constructAppSelection(void)
{
    ui_Widget = lv_obj_create(lv_scr_act());
    lv_obj_remove_style_all(ui_Widget);
    lv_obj_set_width(ui_Widget, 760);
    lv_obj_set_height(ui_Widget, 440);
    lv_obj_set_align(ui_Widget, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Widget, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE |
                                     LV_OBJ_FLAG_GESTURE_BUBBLE | LV_OBJ_FLAG_SNAPPABLE | LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC |
                                     LV_OBJ_FLAG_SCROLL_MOMENTUM | LV_OBJ_FLAG_SCROLL_CHAIN); /// Flags
    lv_obj_set_style_radius(ui_Widget, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Widget, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Widget, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_Widget, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Widget, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_Widget, 2, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LogoGroup = lv_obj_create(ui_Widget);
    lv_obj_remove_style_all(ui_LogoGroup);
    lv_obj_set_width(ui_LogoGroup, 100);
    lv_obj_set_height(ui_LogoGroup, 25);
    lv_obj_set_align(ui_LogoGroup, LV_ALIGN_BOTTOM_MID);
    lv_obj_clear_flag(ui_LogoGroup, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE); /// Flags

    ui_LogoCornerBottomLeft = lv_obj_create(ui_LogoGroup);
    lv_obj_remove_style_all(ui_LogoCornerBottomLeft);
    lv_obj_set_width(ui_LogoCornerBottomLeft, 20);
    lv_obj_set_height(ui_LogoCornerBottomLeft, 10);
    lv_obj_set_align(ui_LogoCornerBottomLeft, LV_ALIGN_BOTTOM_LEFT);
    lv_obj_clear_flag(ui_LogoCornerBottomLeft, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_LogoCornerBottomLeft, lv_color_hex(0x055DA9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_LogoCornerBottomLeft, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui_LogoCornerBottomLeft, false, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LogoCornerFillBottomLeft = lv_obj_create(ui_LogoGroup);
    lv_obj_remove_style_all(ui_LogoCornerFillBottomLeft);
    lv_obj_set_width(ui_LogoCornerFillBottomLeft, 15);
    lv_obj_set_height(ui_LogoCornerFillBottomLeft, 15);
    lv_obj_set_x(ui_LogoCornerFillBottomLeft, -5);
    lv_obj_set_y(ui_LogoCornerFillBottomLeft, 0);
    lv_obj_set_align(ui_LogoCornerFillBottomLeft, LV_ALIGN_BOTTOM_LEFT);
    lv_obj_clear_flag(ui_LogoCornerFillBottomLeft, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_LogoCornerFillBottomLeft, 1000, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_LogoCornerFillBottomLeft, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_LogoCornerFillBottomLeft, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui_LogoCornerFillBottomLeft, false, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LogoCornerBottomRight = lv_obj_create(ui_LogoGroup);
    lv_obj_remove_style_all(ui_LogoCornerBottomRight);
    lv_obj_set_width(ui_LogoCornerBottomRight, 20);
    lv_obj_set_height(ui_LogoCornerBottomRight, 10);
    lv_obj_set_align(ui_LogoCornerBottomRight, LV_ALIGN_BOTTOM_RIGHT);
    lv_obj_clear_flag(ui_LogoCornerBottomRight, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_set_style_bg_color(ui_LogoCornerBottomRight, lv_color_hex(0x055DA9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_LogoCornerBottomRight, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui_LogoCornerBottomRight, false, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LogoCornerFillBottomRight = lv_obj_create(ui_LogoGroup);
    lv_obj_remove_style_all(ui_LogoCornerFillBottomRight);
    lv_obj_set_width(ui_LogoCornerFillBottomRight, 15);
    lv_obj_set_height(ui_LogoCornerFillBottomRight, 15);
    lv_obj_set_x(ui_LogoCornerFillBottomRight, 5);
    lv_obj_set_y(ui_LogoCornerFillBottomRight, 0);
    lv_obj_set_align(ui_LogoCornerFillBottomRight, LV_ALIGN_BOTTOM_RIGHT);
    lv_obj_clear_flag(ui_LogoCornerFillBottomRight, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_LogoCornerFillBottomRight, 1000, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_LogoCornerFillBottomRight, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_LogoCornerFillBottomRight, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui_LogoCornerFillBottomRight, false, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_LogoOutlay = lv_obj_create(ui_LogoGroup);
    lv_obj_remove_style_all(ui_LogoOutlay);
    lv_obj_set_width(ui_LogoOutlay, 80);
    lv_obj_set_height(ui_LogoOutlay, 25);
    lv_obj_set_align(ui_LogoOutlay, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_LogoOutlay, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_LogoOutlay, 1000, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_LogoOutlay, lv_color_hex(0x055DA9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_LogoOutlay, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui_LogoOutlay, false, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LogoImage = lv_img_create(ui_LogoGroup);
    lv_img_set_src(ui_LogoImage, &ui_img_mtalogo_png);
    lv_obj_set_width(ui_LogoImage, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LogoImage, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_LogoImage, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_LogoImage, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_LogoImage, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_img_set_zoom(ui_LogoImage, 80);

    ui_EduPanelLabel = lv_label_create(ui_Widget);
    lv_obj_set_width(ui_EduPanelLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_EduPanelLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_EduPanelLabel, 0);
    lv_obj_set_y(ui_EduPanelLabel, 10);
    lv_obj_set_align(ui_EduPanelLabel, LV_ALIGN_TOP_MID);
    lv_label_set_text(ui_EduPanelLabel, "EduPanel");
    lv_obj_clear_flag(ui_EduPanelLabel, LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE |
                      LV_OBJ_FLAG_SNAPPABLE | LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                      LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_text_color(ui_EduPanelLabel, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_EduPanelLabel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_EduPanelLabel, &lv_font_montserrat_40, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_AppGroup = lv_obj_create(ui_Widget);
    lv_obj_remove_style_all(ui_AppGroup);
    lv_obj_set_width(ui_AppGroup, 100);
    lv_obj_set_height(ui_AppGroup, 130);
    lv_obj_set_x(ui_AppGroup, 40);
    lv_obj_set_y(ui_AppGroup, 80);
    lv_obj_clear_flag(ui_AppGroup, LV_OBJ_FLAG_SCROLLABLE);     /// Flags
    lv_obj_add_event_cb(ui_AppGroup, [](lv_event_t * e) {
        if (e->code == LV_EVENT_CLICKED) {
            switchToCommunicationSelectionScreen();
        }
    }, LV_EVENT_ALL, nullptr);

    ui_AppImage = lv_img_create(ui_AppGroup);
    lv_img_set_src(ui_AppImage, &ui_img_visensors_png);
    lv_obj_set_width(ui_AppImage, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_AppImage, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_AppImage, 0);
    lv_obj_set_y(ui_AppImage, -20);
    lv_obj_set_align(ui_AppImage, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_AppImage, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_AppImage, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_AppImage, 20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_AppLabel = lv_label_create(ui_AppGroup);
    lv_obj_set_width(ui_AppLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_AppLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_AppLabel, 0);
    lv_obj_set_y(ui_AppLabel, 35);
    lv_obj_set_align(ui_AppLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_AppLabel, "ViSensors");
    lv_obj_set_style_text_color(ui_AppLabel, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_AppLabel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_AppLabel, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_ComingSoonGroup = lv_obj_create(ui_Widget);
    lv_obj_remove_style_all(ui_ComingSoonGroup);
    lv_obj_set_width(ui_ComingSoonGroup, 100);
    lv_obj_set_height(ui_ComingSoonGroup, 130);
    lv_obj_set_x(ui_ComingSoonGroup, 160);
    lv_obj_set_y(ui_ComingSoonGroup, 80);
    lv_obj_clear_flag(ui_ComingSoonGroup, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE); 

    ui_ComingSoonImage = lv_img_create(ui_ComingSoonGroup);
    lv_img_set_src(ui_ComingSoonImage, &ui_img_comingsoon_png);
    lv_obj_set_width(ui_ComingSoonImage, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_ComingSoonImage, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_ComingSoonImage, 0);
    lv_obj_set_y(ui_ComingSoonImage, -20);
    lv_obj_set_align(ui_ComingSoonImage, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_ComingSoonImage, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_ComingSoonImage, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_img_set_zoom(ui_ComingSoonImage, 170);
    lv_obj_set_style_bg_color(ui_ComingSoonImage, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_ComingSoonImage, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_ComingSoonLabel = lv_label_create(ui_ComingSoonGroup);
    lv_obj_set_width(ui_ComingSoonLabel, 103);
    lv_obj_set_height(ui_ComingSoonLabel, 45);
    lv_obj_set_x(ui_ComingSoonLabel, 0);
    lv_obj_set_y(ui_ComingSoonLabel, 55);
    lv_obj_set_align(ui_ComingSoonLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_ComingSoonLabel, "More coming soon...");
    lv_obj_set_style_text_color(ui_ComingSoonLabel, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_ComingSoonLabel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
}

void AppSelectionGui::hideAppSelection(void)
{
    if(!initialized)
    {
        return;
    }

    if (ui_Widget)
        lv_obj_del(ui_Widget);

    ui_Widget = nullptr;
    ui_LogoGroup = nullptr;
    ui_LogoCornerBottomLeft = nullptr;
    ui_LogoCornerFillBottomLeft = nullptr;
    ui_LogoCornerBottomRight = nullptr;
    ui_LogoCornerFillBottomRight = nullptr;
    ui_LogoOutlay = nullptr;
    ui_LogoImage = nullptr;
    ui_EduPanelLabel = nullptr;
    ui_AppGroup = nullptr;
    ui_AppImage = nullptr;
    ui_AppLabel = nullptr;
    ui_ComingSoonGroup = nullptr;
    ui_ComingSoonImage = nullptr;
    ui_ComingSoonLabel = nullptr;

    initialized = false;
}
