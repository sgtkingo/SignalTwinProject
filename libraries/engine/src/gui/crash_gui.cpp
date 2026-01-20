/**
 * @file crash_gui.cpp
 * @brief Implementation of the CrashGui class for displaying system crash information
 *
 * This file implements the CrashGui functionality for showing crash screens
 * with error information and restart capability.
 *
 * @copyright 2025 MTA
 * @author Ing. Jiri Konecny, Ondřej Wrubel
 */

#include "crash_gui.hpp"
#include "../helpers.hpp"
#include <expt.hpp>

#ifdef ESP32
    #include <ESP.h>
#elif defined(ARDUINO)
    #include <Arduino.h>
#else
    #include <cstdlib>
#endif

CrashGui::CrashGui() 
    : initialized(false),
      ui_CrashWidget(nullptr), ui_CrashTitle(nullptr), 
      ui_ReasonLabel(nullptr), ui_RestartButton(nullptr),
      ui_RestartButtonLabel(nullptr) {
}

CrashGui::~CrashGui() {
    if (ui_CrashWidget) {
        lv_obj_del(ui_CrashWidget);
    }
}

void CrashGui::init() {
    if (initialized) return;
    
    try {
        // logMessage("Initializing CrashGui...\n");
        buildCrashGui();
        initialized = true;
        // logMessage("CrashGui initialization completed!\n");
    }
    catch (const std::exception &e) {
        // logMessage("CrashGui initialization failed: %s\n", e.what());
        initialized = false;
    }
}

void CrashGui::buildCrashGui() {
    // logMessage("\t>building crash gui...\n");
    
    // Main crash container - full screen with red background
    ui_CrashWidget = lv_obj_create(lv_scr_act());
    lv_obj_set_width(ui_CrashWidget, lv_pct(100));
    lv_obj_set_height(ui_CrashWidget, lv_pct(100));
    lv_obj_set_align(ui_CrashWidget, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_CrashWidget, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_radius(ui_CrashWidget, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_flag(ui_CrashWidget, LV_OBJ_FLAG_HIDDEN); // Start hidden
    
    // Set red background
    lv_obj_set_style_bg_color(ui_CrashWidget, lv_color_hex(0xFF0000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_CrashWidget, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_CrashWidget, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    
    // "CRASH!" title - large white text at top
    ui_CrashTitle = lv_label_create(ui_CrashWidget);
    lv_label_set_text(ui_CrashTitle, "CRASH!");
    lv_obj_set_width(ui_CrashTitle, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_CrashTitle, LV_SIZE_CONTENT);
    lv_obj_set_align(ui_CrashTitle, LV_ALIGN_TOP_MID);
    lv_obj_set_y(ui_CrashTitle, 50);
    
    // White text styling for title
    lv_obj_set_style_text_color(ui_CrashTitle, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_CrashTitle, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_CrashTitle, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    
    // Reason text label - smaller white text in center
    ui_ReasonLabel = lv_label_create(ui_CrashWidget);
    lv_label_set_text(ui_ReasonLabel, "System crashed");
    lv_obj_set_width(ui_ReasonLabel, lv_pct(80));
    lv_obj_set_height(ui_ReasonLabel, LV_SIZE_CONTENT);
    lv_obj_set_align(ui_ReasonLabel, LV_ALIGN_CENTER);
    lv_obj_set_y(ui_ReasonLabel, 0);
    
    // White text styling for reason
    lv_obj_set_style_text_color(ui_ReasonLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_ReasonLabel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_ReasonLabel, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_ReasonLabel, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_long_mode(ui_ReasonLabel, LV_LABEL_LONG_WRAP);
    
    // Restart button - white button with black text at bottom
    ui_RestartButton = lv_btn_create(ui_CrashWidget);
    lv_obj_set_width(ui_RestartButton, 120);
    lv_obj_set_height(ui_RestartButton, 50);
    lv_obj_set_align(ui_RestartButton, LV_ALIGN_BOTTOM_MID);
    lv_obj_set_y(ui_RestartButton, -50);
    
    // White button styling
    lv_obj_set_style_bg_color(ui_RestartButton, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_RestartButton, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_RestartButton, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_RestartButton, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_RestartButton, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui_RestartButton, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    
    // Button event handler
    lv_obj_add_event_cb(ui_RestartButton, [](lv_event_t *e) {
        auto self = static_cast<CrashGui*>(lv_event_get_user_data(e));
        self->handleRestartButtonClick();
    }, LV_EVENT_CLICKED, this);
    
    // Restart button label - black text
    ui_RestartButtonLabel = lv_label_create(ui_RestartButton);
    lv_label_set_text(ui_RestartButtonLabel, "Restart");
    lv_obj_center(ui_RestartButtonLabel);
    
    // Black text for button label
    lv_obj_set_style_text_color(ui_RestartButtonLabel, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_RestartButtonLabel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_RestartButtonLabel, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    
    // logMessage("\t>crash gui built successfully!\n");
}

void CrashGui::showCrash(const std::string& reason) {
    if (!initialized || !ui_CrashWidget) 
    {
        init();
    }
    
    // Update reason text
    if (ui_ReasonLabel) {
        lv_label_set_text(ui_ReasonLabel, reason.c_str());
    }
    
    // Show the crash screen
    lv_obj_clear_flag(ui_CrashWidget, LV_OBJ_FLAG_HIDDEN);
    
    // logMessage("Showing crash screen: %s\n", reason.c_str());
}

void CrashGui::hideCrash() {
    if (!initialized || !ui_CrashWidget) return;
    
    lv_obj_add_flag(ui_CrashWidget, LV_OBJ_FLAG_HIDDEN);
    // logMessage("Hiding crash screen\n");
}

void CrashGui::handleRestartButtonClick() {
    // logMessage("Restart button pressed - triggering system reset\n");
    
    // Trigger system reset
    #ifdef ESP32
        ESP.restart();
    #elif defined(ARDUINO)
        // For other Arduino platforms
        void(* resetFunc) (void) = 0; // declare reset function @ address 0
        resetFunc(); // call reset
    #else
        // For PC/simulation - just exit
        std::exit(0);
    #endif
}