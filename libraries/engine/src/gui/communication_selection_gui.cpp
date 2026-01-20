/**
 * @file communication_selection.cpp
 * @brief Implementation of the CommunicationSelectionGui class for managing the communication selection GUI.
 * This class handles the initialization, construction, and hiding of the communication selection interface.
 * 
 * @copyright 2025 MTA
 * @author Ondřej Wrubel
 */

#include "communication_selection_gui.hpp"
#include "./images/ui_images.h"

CommunicationSelectionGui::CommunicationSelectionGui()
{
    ui_Widget = nullptr;
    ui_ConnectionLabel = nullptr;
    ui_CableButton = nullptr;
    ui_CableImage = nullptr;
    ui_BluetoothButton = nullptr;
    ui_BluetoothImage = nullptr;
}

void CommunicationSelectionGui::init(void)
{
    if (initialized)
    {
        return;
    }

    try
    {
        // // logMessage("Initializing CommunicationSelectionGui...\n");
        constructCommunicationSelection();
        initialized = true;
        // // logMessage("CommunicationSelectionGui initialization completed!\n");
    }
    catch (const std::exception &e)
    {
        // // logMessage("CommunicationSelectionGui initialization failed: %s\n", e.what());
        initialized = false;
    }
}

void CommunicationSelectionGui::constructCommunicationSelection(void)
{
    ui_Widget = lv_obj_create(lv_scr_act());
    lv_obj_remove_style_all(ui_Widget);
    lv_obj_set_width(ui_Widget, 800);
    lv_obj_set_height(ui_Widget, 480);
    lv_obj_set_align(ui_Widget, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Widget, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE |
                      LV_OBJ_FLAG_GESTURE_BUBBLE | LV_OBJ_FLAG_SNAPPABLE | LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC |
                      LV_OBJ_FLAG_SCROLL_MOMENTUM | LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_bg_color(ui_Widget, lv_color_hex(0x055DA9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Widget, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_Widget, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Widget, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_Widget, 2, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_ConnectionLabel = lv_label_create(ui_Widget);
    lv_obj_set_width(ui_ConnectionLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_ConnectionLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_ConnectionLabel, 0);
    lv_obj_set_y(ui_ConnectionLabel, 40);
    lv_obj_set_align(ui_ConnectionLabel, LV_ALIGN_TOP_MID);
    lv_label_set_text(ui_ConnectionLabel, "Select your connection");
    lv_obj_clear_flag(ui_ConnectionLabel,
                      LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE | LV_OBJ_FLAG_SNAPPABLE |
                      LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                      LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_text_color(ui_ConnectionLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_ConnectionLabel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_ConnectionLabel, &lv_font_montserrat_40, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_CableButton = lv_btn_create(ui_Widget);
    lv_obj_set_width(ui_CableButton, 220);
    lv_obj_set_height(ui_CableButton, 220);
    lv_obj_set_x(ui_CableButton, -150);
    lv_obj_set_y(ui_CableButton, 0);
    lv_obj_set_align(ui_CableButton, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_CableButton, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_CableButton, LV_OBJ_FLAG_SCROLLABLE);    /// Flags
    lv_obj_add_event_cb(ui_CableButton, [](lv_event_t * e) {
        if (e->code == LV_EVENT_CLICKED) {
            switchToWiki();
        }
    }, LV_EVENT_ALL, nullptr);

    ui_CableImage = lv_img_create(ui_CableButton);
    lv_img_set_src(ui_CableImage, &ui_img_cable_png);
    lv_obj_set_width(ui_CableImage, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_CableImage, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_CableImage, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_CableImage, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_CableImage, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_img_set_zoom(ui_CableImage, 340);

    ui_BluetoothButton = lv_btn_create(ui_Widget);
    lv_obj_set_width(ui_BluetoothButton, 220);
    lv_obj_set_height(ui_BluetoothButton, 220);
    lv_obj_set_x(ui_BluetoothButton, 150);
    lv_obj_set_y(ui_BluetoothButton, 0);
    lv_obj_set_align(ui_BluetoothButton, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_BluetoothButton, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_BluetoothButton, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_BluetoothButton, lv_color_hex(0x0078C7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_BluetoothButton, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(ui_BluetoothButton, [](lv_event_t * e) {
        if (e->code == LV_EVENT_CLICKED) {
            splashMessage("Bluetooth connection\nis not implemented yet.");
        }
    }, LV_EVENT_ALL, nullptr);

    ui_BluetoothImage = lv_img_create(ui_BluetoothButton);
    lv_img_set_src(ui_BluetoothImage, &ui_img_bluetooth_png);
    lv_obj_set_width(ui_BluetoothImage, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_BluetoothImage, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_BluetoothImage, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_BluetoothImage, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_BluetoothImage, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_img_set_zoom(ui_BluetoothImage, 480);
    lv_obj_set_style_img_recolor(ui_BluetoothImage, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor_opa(ui_BluetoothImage, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
}

void CommunicationSelectionGui::hideCommunicationSelection(void)
{
    if(!initialized)
    {
        return;
    }

    if(ui_Widget) lv_obj_del(ui_Widget);

    ui_Widget = NULL;
    ui_ConnectionLabel = NULL;
    ui_CableButton = NULL;
    ui_CableImage = NULL;
    ui_BluetoothButton = NULL;
    ui_BluetoothImage = NULL;

    initialized = false;
}
