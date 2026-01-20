/**
 * @file data_bundle_selection_gui.cpp
 * @brief Implementation of the DataBundleSelectionGui class
 *
 * This source file implements the DataBundleSelectionGui functionality for
 * data bundles made from record on visualisation
 *
 * @copyright 2025 MTA
 * @author Ondřej Wrubel
 */

#include "data_bundle_selection_gui.hpp"
#include "../helpers.hpp"
#include "./images/ui_images.h"

DataBundleSelectionGui::DataBundleSelectionGui(DataBundleManager &dataBundleManager) : dataBundleManager(dataBundleManager)
{
    ui_DataBundlesWidget = nullptr;
    ui_DataBundlePageWatcher = nullptr;
    for (int i = 0; i < 5; ++i) 
        ui_DataBundlePageWatcherCell[i] = nullptr;
    for (int i = 0; i < 6; ++i) {
        ui_DataBundle[i] = nullptr;
        ui_DataBundleHeaderGroup[i] = nullptr;
        ui_DataBundleHeaderCornerBottomLeft[i] = nullptr;
        ui_DataBundleHeaderCornerBottomRight[i] = nullptr;
        ui_DataBundleHeader[i] = nullptr;
        ui_DataBundleHeaderLabel[i] = nullptr;
        ui_DataBundleChart[i] = nullptr;
        ui_DataBundleChart_series_1[i] = nullptr;
        ui_DataBundleFooterGroup[i] = nullptr;
        ui_DataBundleFooterBridge[i] = nullptr;
        ui_DataBundleFooterBridgeFill[i] = nullptr;
        ui_DataBundleFooterTimerGroup[i] = nullptr;
        ui_DataBundleFooterDateCornerTopLeft[i] = nullptr;
        ui_DataBundleFooterDateCornerTopRight[i] = nullptr;
        ui_DataBundleFooterDate[i] = nullptr;
        ui_DataBundleFooterLabelDate[i] = nullptr;
        ui_DataBundleFooterLabelTime[i] = nullptr;
        ui_DataBundleFooterButtonsGroup[i] = nullptr;
        ui_DataBundleFooterButtonsCornerTopLeft[i] = nullptr;
        ui_DataBundleFooterButtonsCornerTopRight[i] = nullptr;
        ui_DataBundleFooterButtons[i] = nullptr;
        ui_DataBundleFooterButtonExport[i] = nullptr;
        ui_DataBundleFooterButtonExportImage[i] = nullptr;
        ui_DataBundleFooterButtonClear[i] = nullptr;
        ui_DataBundleFooterButtonClearImage[i] = nullptr;
    }
    ui_LogoGroup = nullptr;
    ui_LogoCornerBottomLeft = nullptr;
    ui_LogoCornerFillBottomLeft = nullptr;
    ui_LogoCornerBottomRight = nullptr;
    ui_LogoCornerFillBottomRight = nullptr;
    ui_LogoOutlay = nullptr;
    ui_LogoImage = nullptr;
}

void DataBundleSelectionGui::init()
{
    if (initialized)
        return;

    try
    {
        // // logMessage("Initializing DataBundleSelectionGui...\n");
        constructDataBundleSelection();
        initialized = true;
        // // logMessage("DataBundleSelectionGui initialization completed!\n");
    }
    catch (const std::exception &e)
    {
        // // logMessage("DataBundleSelectionGui initialization failed: %s\n", e.what());
        initialized = false;
    }
}

void DataBundleSelectionGui::constructDataBundleSelection()
{
    if(ui_DataBundlesWidget)
        return; // Already constructed

    // 1. Initialize pointers to nullptr for safety
    for(int i = 0; i < 6; i++) {
        ui_DataBundle[i] = nullptr;
    }

    // 2. Main Container Widget (The Frame)
    ui_DataBundlesWidget = lv_obj_create(lv_scr_act()); 
    lv_obj_remove_style_all(ui_DataBundlesWidget);
    lv_obj_set_width(ui_DataBundlesWidget, 760);
    lv_obj_set_height(ui_DataBundlesWidget, 440);
    lv_obj_set_align(ui_DataBundlesWidget, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_DataBundlesWidget, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_radius(ui_DataBundlesWidget, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_DataBundlesWidget, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_DataBundlesWidget, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_DataBundlesWidget, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_DataBundlesWidget, 2, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Title Label
    lv_obj_t* title_label = lv_label_create(ui_DataBundlesWidget);
    lv_label_set_text(title_label, "Data Bundles");
    lv_obj_set_y(title_label, 10);
    lv_obj_set_align(title_label, LV_ALIGN_TOP_MID);
    lv_obj_set_style_text_font(title_label, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);

    // 3. Page Watcher (Static UI elements)
    ui_DataBundlePageWatcher = lv_obj_create(ui_DataBundlesWidget);
    lv_obj_remove_style_all(ui_DataBundlePageWatcher);
    lv_obj_set_size(ui_DataBundlePageWatcher, 80, 15);
    lv_obj_set_y(ui_DataBundlePageWatcher, -35);
    lv_obj_set_align(ui_DataBundlePageWatcher, LV_ALIGN_BOTTOM_MID);

    // Create watcher cells
    for(int i = 0; i < 5; i++) {
        ui_DataBundlePageWatcherCell[i] = lv_obj_create(ui_DataBundlePageWatcher);
        lv_obj_remove_style_all(ui_DataBundlePageWatcherCell[i]);
        lv_obj_set_size(ui_DataBundlePageWatcherCell[i], 12, 12);
        lv_obj_set_x(ui_DataBundlePageWatcherCell[i], (i - 2) * 15);
        lv_obj_set_align(ui_DataBundlePageWatcherCell[i], LV_ALIGN_BOTTOM_MID);
        lv_obj_set_style_radius(ui_DataBundlePageWatcherCell[i], 100, LV_PART_MAIN);
        lv_obj_set_style_border_width(ui_DataBundlePageWatcherCell[i], 2, LV_PART_MAIN);
        lv_obj_set_style_bg_color(ui_DataBundlePageWatcherCell[i], lv_color_hex(0x009BFF), LV_PART_MAIN);
        lv_obj_set_style_border_color(ui_DataBundlePageWatcherCell[i], lv_color_hex(0x009BFF), LV_PART_MAIN);
        lv_obj_set_style_bg_opa(ui_DataBundlePageWatcherCell[i], 255, LV_PART_MAIN);
    }   

    addControlButtonsToWidget(ui_DataBundlesWidget);
    addLogoPanelToWidget(ui_DataBundlesWidget);
    addNavButtonsToWidget(ui_DataBundlesWidget);
}   

void DataBundleSelectionGui::createDataBundle(unsigned char i, const char *dataBundleName, const char *time, const char *date, std::array<std::string,10> values)
{
    // order of data bundle in current page
    if (i < 0 || i >= 6) return;

    if(!ui_DataBundle[i])
    {
    // Determine Position based on Index
    int x_pos = 0;
    int y_pos = 0;

    switch(i) {
        case 0: x_pos = -250; y_pos = -85; break; // Top Left
        case 1: x_pos = 0;    y_pos = -85; break; // Top Mid
        case 2: x_pos = 250;  y_pos = -85; break; // Top Right
        case 3: x_pos = -250; y_pos = 85;  break; // Bottom Left
        case 4: x_pos = 0;    y_pos = 85;  break; // Bottom Mid
        case 5: x_pos = 250;  y_pos = 85;  break; // Bottom Right
    }

    // 3. Create Main Bundle Container
    ui_DataBundle[i] = lv_obj_create(ui_DataBundlesWidget);
    lv_obj_remove_style_all(ui_DataBundle[i]);
    lv_obj_set_size(ui_DataBundle[i], 200, 160);
    
    // Apply the calculated position
    lv_obj_set_align(ui_DataBundle[i], LV_ALIGN_CENTER);
    lv_obj_set_pos(ui_DataBundle[i], x_pos, y_pos);

    // --- Header Group ---
    ui_DataBundleHeaderGroup[i] = lv_obj_create(ui_DataBundle[i]);
    lv_obj_remove_style_all(ui_DataBundleHeaderGroup[i]);
    lv_obj_set_size(ui_DataBundleHeaderGroup[i], 200, 20);
    lv_obj_set_align(ui_DataBundleHeaderGroup[i], LV_ALIGN_TOP_MID);

    ui_DataBundleHeaderCornerBottomLeft[i] = lv_obj_create(ui_DataBundleHeaderGroup[i]);
    lv_obj_remove_style_all(ui_DataBundleHeaderCornerBottomLeft[i]);
    lv_obj_set_size(ui_DataBundleHeaderCornerBottomLeft[i], 10, 10);
    lv_obj_set_align(ui_DataBundleHeaderCornerBottomLeft[i], LV_ALIGN_BOTTOM_LEFT);
    lv_obj_set_style_bg_color(ui_DataBundleHeaderCornerBottomLeft[i], lv_color_hex(0x007CCC), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(ui_DataBundleHeaderCornerBottomLeft[i], 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_DataBundleHeaderCornerBottomRight[i] = lv_obj_create(ui_DataBundleHeaderGroup[i]);
    lv_obj_remove_style_all(ui_DataBundleHeaderCornerBottomRight[i]);
    lv_obj_set_size(ui_DataBundleHeaderCornerBottomRight[i], 10, 10);
    lv_obj_set_align(ui_DataBundleHeaderCornerBottomRight[i], LV_ALIGN_BOTTOM_RIGHT);
    lv_obj_set_style_bg_color(ui_DataBundleHeaderCornerBottomRight[i], lv_color_hex(0x007CCC), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(ui_DataBundleHeaderCornerBottomRight[i], 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_DataBundleHeader[i] = lv_obj_create(ui_DataBundleHeaderGroup[i]);
    lv_obj_remove_style_all(ui_DataBundleHeader[i]);
    lv_obj_set_size(ui_DataBundleHeader[i], 200, 20);
    lv_obj_set_align(ui_DataBundleHeader[i], LV_ALIGN_TOP_MID);
    lv_obj_set_style_radius(ui_DataBundleHeader[i], 10, LV_PART_MAIN);
    lv_obj_set_style_bg_color(ui_DataBundleHeader[i], lv_color_hex(0x007CCC), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(ui_DataBundleHeader[i], 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_DataBundleHeaderLabel[i] = lv_label_create(ui_DataBundleHeader[i]);
    lv_obj_set_align(ui_DataBundleHeaderLabel[i], LV_ALIGN_CENTER);
    lv_label_set_text(ui_DataBundleHeaderLabel[i], "Bundle Title"); 
    lv_obj_set_style_text_font(ui_DataBundleHeaderLabel[i], &lv_font_montserrat_12, LV_PART_MAIN);

    // --- Chart ---
    ui_DataBundleChart[i] = lv_chart_create(ui_DataBundle[i]);
    lv_obj_set_size(ui_DataBundleChart[i], 200, 110);
    lv_obj_set_y(ui_DataBundleChart[i], -4);
    lv_obj_set_align(ui_DataBundleChart[i], LV_ALIGN_CENTER);
    lv_chart_set_type(ui_DataBundleChart[i], LV_CHART_TYPE_LINE);
    lv_chart_set_div_line_count(ui_DataBundleChart[i], 5, 10);
    lv_obj_set_style_radius(ui_DataBundleChart[i], 0, LV_PART_MAIN);
    lv_obj_set_style_bg_color(ui_DataBundleChart[i], lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    
    ui_DataBundleChart_series_1[i] = lv_chart_add_series(ui_DataBundleChart[i], lv_color_hex(0xFF8200), LV_CHART_AXIS_PRIMARY_Y);

    // --- Footer Group ---
    ui_DataBundleFooterGroup[i] = lv_obj_create(ui_DataBundle[i]);
    lv_obj_remove_style_all(ui_DataBundleFooterGroup[i]);
    lv_obj_set_size(ui_DataBundleFooterGroup[i], 200, 30);
    lv_obj_set_align(ui_DataBundleFooterGroup[i], LV_ALIGN_BOTTOM_MID);

    ui_DataBundleFooterBridge[i] = lv_obj_create(ui_DataBundleFooterGroup[i]);
    lv_obj_remove_style_all(ui_DataBundleFooterBridge[i]);
    lv_obj_set_size(ui_DataBundleFooterBridge[i], 40, 15);
    lv_obj_set_pos(ui_DataBundleFooterBridge[i], 25, -7);
    lv_obj_set_align(ui_DataBundleFooterBridge[i], LV_ALIGN_CENTER);
    lv_obj_set_style_bg_color(ui_DataBundleFooterBridge[i], lv_color_hex(0x055DA9), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(ui_DataBundleFooterBridge[i], 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_DataBundleFooterBridgeFill[i] = lv_obj_create(ui_DataBundleFooterGroup[i]);
    lv_obj_remove_style_all(ui_DataBundleFooterBridgeFill[i]);
    lv_obj_set_size(ui_DataBundleFooterBridgeFill[i], 25, 15);
    lv_obj_set_pos(ui_DataBundleFooterBridgeFill[i], 17, -1);
    lv_obj_set_align(ui_DataBundleFooterBridgeFill[i], LV_ALIGN_CENTER);
    lv_obj_set_style_radius(ui_DataBundleFooterBridgeFill[i], 5, LV_PART_MAIN);
    lv_obj_set_style_bg_color(ui_DataBundleFooterBridgeFill[i], lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(ui_DataBundleFooterBridgeFill[i], 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    // --- Footer Timer ---
    ui_DataBundleFooterTimerGroup[i] = lv_obj_create(ui_DataBundleFooterGroup[i]);
    lv_obj_remove_style_all(ui_DataBundleFooterTimerGroup[i]);
    lv_obj_set_size(ui_DataBundleFooterTimerGroup[i], 105, 20);
    lv_obj_set_pos(ui_DataBundleFooterTimerGroup[i], 0, -9);
    lv_obj_set_align(ui_DataBundleFooterTimerGroup[i], LV_ALIGN_BOTTOM_LEFT);

    ui_DataBundleFooterDateCornerTopLeft[i] = lv_obj_create(ui_DataBundleFooterTimerGroup[i]);
    lv_obj_remove_style_all(ui_DataBundleFooterDateCornerTopLeft[i]);
    lv_obj_set_size(ui_DataBundleFooterDateCornerTopLeft[i], 10, 10);
    lv_obj_set_align(ui_DataBundleFooterDateCornerTopLeft[i], LV_ALIGN_TOP_LEFT);
    lv_obj_set_style_bg_color(ui_DataBundleFooterDateCornerTopLeft[i], lv_color_hex(0x055DA9), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(ui_DataBundleFooterDateCornerTopLeft[i], 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_DataBundleFooterDateCornerTopRight[i] = lv_obj_create(ui_DataBundleFooterTimerGroup[i]);
    lv_obj_remove_style_all(ui_DataBundleFooterDateCornerTopRight[i]);
    lv_obj_set_size(ui_DataBundleFooterDateCornerTopRight[i], 10, 10);
    lv_obj_set_align(ui_DataBundleFooterDateCornerTopRight[i], LV_ALIGN_TOP_RIGHT);
    lv_obj_set_style_bg_color(ui_DataBundleFooterDateCornerTopRight[i], lv_color_hex(0x055DA9), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(ui_DataBundleFooterDateCornerTopRight[i], 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_DataBundleFooterDate[i] = lv_obj_create(ui_DataBundleFooterTimerGroup[i]);
    lv_obj_remove_style_all(ui_DataBundleFooterDate[i]);
    lv_obj_set_size(ui_DataBundleFooterDate[i], 105, 20);
    lv_obj_set_align(ui_DataBundleFooterDate[i], LV_ALIGN_TOP_MID);
    lv_obj_set_style_radius(ui_DataBundleFooterDate[i], 10, LV_PART_MAIN);
    lv_obj_set_style_bg_color(ui_DataBundleFooterDate[i], lv_color_hex(0x055DA9), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(ui_DataBundleFooterDate[i], 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_DataBundleFooterLabelDate[i] = lv_label_create(ui_DataBundleFooterDate[i]);
    lv_label_set_text(ui_DataBundleFooterLabelDate[i], "01.01.2024");
    lv_obj_set_x(ui_DataBundleFooterLabelDate[i], 7);
    lv_obj_set_align(ui_DataBundleFooterLabelDate[i], LV_ALIGN_LEFT_MID);
    lv_obj_set_style_text_font(ui_DataBundleFooterLabelDate[i], &lv_font_montserrat_12, LV_PART_MAIN);

    ui_DataBundleFooterLabelTime[i] = lv_label_create(ui_DataBundleFooterDate[i]);
    lv_label_set_text(ui_DataBundleFooterLabelTime[i], "12:00");
    lv_obj_set_x(ui_DataBundleFooterLabelTime[i], -5);
    lv_obj_set_align(ui_DataBundleFooterLabelTime[i], LV_ALIGN_RIGHT_MID);
    lv_obj_set_style_text_font(ui_DataBundleFooterLabelTime[i], &lv_font_montserrat_12, LV_PART_MAIN);

    // --- Footer Buttons ---
    ui_DataBundleFooterButtonsGroup[i] = lv_obj_create(ui_DataBundleFooterGroup[i]);
    lv_obj_remove_style_all(ui_DataBundleFooterButtonsGroup[i]);
    lv_obj_set_size(ui_DataBundleFooterButtonsGroup[i], 70, 30);
    lv_obj_set_pos(ui_DataBundleFooterButtonsGroup[i], 0, 1);
    lv_obj_set_align(ui_DataBundleFooterButtonsGroup[i], LV_ALIGN_BOTTOM_RIGHT);

    ui_DataBundleFooterButtonsCornerTopLeft[i] = lv_obj_create(ui_DataBundleFooterButtonsGroup[i]);
    lv_obj_remove_style_all(ui_DataBundleFooterButtonsCornerTopLeft[i]);
    lv_obj_set_size(ui_DataBundleFooterButtonsCornerTopLeft[i], 10, 10);
    lv_obj_set_align(ui_DataBundleFooterButtonsCornerTopLeft[i], LV_ALIGN_TOP_LEFT);
    lv_obj_set_style_bg_color(ui_DataBundleFooterButtonsCornerTopLeft[i], lv_color_hex(0x055DA9), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(ui_DataBundleFooterButtonsCornerTopLeft[i], 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_DataBundleFooterButtonsCornerTopRight[i] = lv_obj_create(ui_DataBundleFooterButtonsGroup[i]);
    lv_obj_remove_style_all(ui_DataBundleFooterButtonsCornerTopRight[i]);
    lv_obj_set_size(ui_DataBundleFooterButtonsCornerTopRight[i], 10, 10);
    lv_obj_set_align(ui_DataBundleFooterButtonsCornerTopRight[i], LV_ALIGN_TOP_RIGHT);
    lv_obj_set_style_bg_color(ui_DataBundleFooterButtonsCornerTopRight[i], lv_color_hex(0x055DA9), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(ui_DataBundleFooterButtonsCornerTopRight[i], 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_DataBundleFooterButtons[i] = lv_obj_create(ui_DataBundleFooterButtonsGroup[i]);
    lv_obj_remove_style_all(ui_DataBundleFooterButtons[i]);
    lv_obj_set_size(ui_DataBundleFooterButtons[i], 70, 30);
    lv_obj_set_align(ui_DataBundleFooterButtons[i], LV_ALIGN_TOP_MID);
    lv_obj_set_style_radius(ui_DataBundleFooterButtons[i], 10, LV_PART_MAIN);
    lv_obj_set_style_bg_color(ui_DataBundleFooterButtons[i], lv_color_hex(0x055DA9), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(ui_DataBundleFooterButtons[i], 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_DataBundleFooterButtonExport[i] = lv_btn_create(ui_DataBundleFooterButtons[i]);
    lv_obj_set_size(ui_DataBundleFooterButtonExport[i], 28, 24);
    lv_obj_set_pos(ui_DataBundleFooterButtonExport[i], 5, -1);
    lv_obj_set_align(ui_DataBundleFooterButtonExport[i], LV_ALIGN_LEFT_MID);
    lv_obj_set_style_radius(ui_DataBundleFooterButtonExport[i], 5, LV_PART_MAIN);
    
    ui_DataBundleFooterButtonExportImage[i] = lv_img_create(ui_DataBundleFooterButtonExport[i]);
    lv_img_set_src(ui_DataBundleFooterButtonExportImage[i], &ui_img_export_png);
    lv_obj_set_align(ui_DataBundleFooterButtonExportImage[i], LV_ALIGN_CENTER);
    lv_img_set_zoom(ui_DataBundleFooterButtonExportImage[i], 81);

    ui_DataBundleFooterButtonClear[i] = lv_btn_create(ui_DataBundleFooterButtons[i]);
    lv_obj_set_size(ui_DataBundleFooterButtonClear[i], 28, 24);
    lv_obj_set_pos(ui_DataBundleFooterButtonClear[i], -5, -1);
    lv_obj_set_align(ui_DataBundleFooterButtonClear[i], LV_ALIGN_RIGHT_MID);
    lv_obj_set_style_radius(ui_DataBundleFooterButtonClear[i], 5, LV_PART_MAIN);
    lv_obj_set_user_data(ui_DataBundleFooterButtonClear[i], (void*)(intptr_t)i);
    lv_obj_add_event_cb(ui_DataBundleFooterButtonClear[i], [](lv_event_t *e)
    {
        auto self = static_cast<DataBundleSelectionGui*>(lv_event_get_user_data(e));
        lv_obj_t *btn = lv_event_get_current_target(e);
        int index = (intptr_t)lv_obj_get_user_data(btn);
        self->handleClearButtonClick(index); 
    }, LV_EVENT_CLICKED, this);

    ui_DataBundleFooterButtonClearImage[i] = lv_img_create(ui_DataBundleFooterButtonClear[i]);
    lv_img_set_src(ui_DataBundleFooterButtonClearImage[i], &ui_img_trashicon_png);
    lv_obj_set_align(ui_DataBundleFooterButtonClearImage[i], LV_ALIGN_CENTER);
    lv_img_set_zoom(ui_DataBundleFooterButtonClearImage[i], 81);
    }

    if (ui_DataBundleHeaderLabel[i]) lv_label_set_text(ui_DataBundleHeaderLabel[i], dataBundleName);
    if (ui_DataBundleFooterLabelDate[i]) lv_label_set_text(ui_DataBundleFooterLabelDate[i], date);
    if (ui_DataBundleFooterLabelTime[i]) lv_label_set_text(ui_DataBundleFooterLabelTime[i], time);

if (ui_DataBundleChart[i] && ui_DataBundleChart_series_1[i]) 
    {
        int minVal = 10000;
        int maxVal = -10000;

        for (int j = 0; j < 10; j++)
        {
            int val = 0;

            if(!values[j].empty()) {
                val = atoi(values[j].c_str()); 
            }

            lv_chart_set_next_value(ui_DataBundleChart[i], ui_DataBundleChart_series_1[i], val);

            if (val < minVal) minVal = val;
            if (val > maxVal) maxVal = val;
        }
 
        int margin = 10;

        if (minVal == maxVal) {
            minVal -= 10;
            maxVal += 10;
        }

        lv_chart_set_range(ui_DataBundleChart[i], LV_CHART_AXIS_PRIMARY_Y, minVal - margin, maxVal + margin);
    }
}

void DataBundleSelectionGui::addNavButtonsToWidget(lv_obj_t *parentWidget)
{
    if (!parentWidget)
        return;

    // next button
    lv_obj_t * ui_ButtonNextGroup = lv_obj_create(parentWidget);
    lv_obj_remove_style_all(ui_ButtonNextGroup);
    lv_obj_set_width(ui_ButtonNextGroup, 100);
    lv_obj_set_height(ui_ButtonNextGroup, 40);
    lv_obj_set_x(ui_ButtonNextGroup, 160);
    lv_obj_set_y(ui_ButtonNextGroup, 0);
    lv_obj_set_align(ui_ButtonNextGroup, LV_ALIGN_BOTTOM_MID);
    lv_obj_clear_flag(ui_ButtonNextGroup, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);   /// Flags

    lv_obj_t * ui_ButtonNextCornerBottomLeft = lv_obj_create(ui_ButtonNextGroup);
    lv_obj_remove_style_all(ui_ButtonNextCornerBottomLeft);
    lv_obj_set_width(ui_ButtonNextCornerBottomLeft, 20);
    lv_obj_set_height(ui_ButtonNextCornerBottomLeft, 20);
    lv_obj_set_align(ui_ButtonNextCornerBottomLeft, LV_ALIGN_BOTTOM_LEFT);
    lv_obj_clear_flag(ui_ButtonNextCornerBottomLeft, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);   /// Flags
    lv_obj_set_style_bg_color(ui_ButtonNextCornerBottomLeft, lv_color_hex(0x009BFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_ButtonNextCornerBottomLeft, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui_ButtonNextCornerBottomLeft, false, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t * ui_ButtonNextCornerBottomRight = lv_obj_create(ui_ButtonNextGroup);
    lv_obj_remove_style_all(ui_ButtonNextCornerBottomRight);
    lv_obj_set_width(ui_ButtonNextCornerBottomRight, 20);
    lv_obj_set_height(ui_ButtonNextCornerBottomRight, 20);
    lv_obj_set_align(ui_ButtonNextCornerBottomRight, LV_ALIGN_BOTTOM_RIGHT);
    lv_obj_clear_flag(ui_ButtonNextCornerBottomRight, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);   /// Flags
    lv_obj_set_style_bg_color(ui_ButtonNextCornerBottomRight, lv_color_hex(0x009BFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_ButtonNextCornerBottomRight, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui_ButtonNextCornerBottomRight, false, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t * ui_btnNext = lv_btn_create(ui_ButtonNextGroup);
    lv_obj_set_width(ui_btnNext, 100);
    lv_obj_set_height(ui_btnNext, 40);
    lv_obj_set_align(ui_btnNext, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_btnNext, LV_OBJ_FLAG_EVENT_BUBBLE);   /// Flags
    lv_obj_clear_flag(ui_btnNext, LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE |
           LV_OBJ_FLAG_SNAPPABLE | LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
           LV_OBJ_FLAG_SCROLL_CHAIN);   /// Flags
    lv_obj_add_event_cb(ui_btnNext, [](lv_event_t *e)
                        {
        auto self = static_cast<DataBundleSelectionGui*>(lv_event_get_user_data(e));
        self->goToNextPage(); }, LV_EVENT_CLICKED, this);

    lv_obj_t * ui_btnNextLabel = lv_label_create(ui_btnNext);
    lv_label_set_text(ui_btnNextLabel, "Next");
    lv_obj_center(ui_btnNextLabel);
    lv_obj_set_style_text_font(ui_btnNextLabel, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Previous button
    lv_obj_t * ui_ButtonPrevGroup = lv_obj_create(parentWidget);
    lv_obj_remove_style_all(ui_ButtonPrevGroup);
    lv_obj_set_width(ui_ButtonPrevGroup, 100);
    lv_obj_set_height(ui_ButtonPrevGroup, 40);
    lv_obj_set_x(ui_ButtonPrevGroup, -160);
    lv_obj_set_y(ui_ButtonPrevGroup, 0);
    lv_obj_set_align(ui_ButtonPrevGroup, LV_ALIGN_BOTTOM_MID);
    lv_obj_clear_flag(ui_ButtonPrevGroup, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);   /// Flags

    lv_obj_t * ui_ButtonPrevCornerBottomLeft = lv_obj_create(ui_ButtonPrevGroup);
    lv_obj_remove_style_all(ui_ButtonPrevCornerBottomLeft);
    lv_obj_set_width(ui_ButtonPrevCornerBottomLeft, 20);
    lv_obj_set_height(ui_ButtonPrevCornerBottomLeft, 20);
    lv_obj_set_align(ui_ButtonPrevCornerBottomLeft, LV_ALIGN_BOTTOM_LEFT);
    lv_obj_clear_flag(ui_ButtonPrevCornerBottomLeft, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);   /// Flags
    lv_obj_set_style_bg_color(ui_ButtonPrevCornerBottomLeft, lv_color_hex(0x009BFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_ButtonPrevCornerBottomLeft, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui_ButtonPrevCornerBottomLeft, false, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t * ui_ButtonPrevCornerBottomRight = lv_obj_create(ui_ButtonPrevGroup);
    lv_obj_remove_style_all(ui_ButtonPrevCornerBottomRight);
    lv_obj_set_width(ui_ButtonPrevCornerBottomRight, 20);
    lv_obj_set_height(ui_ButtonPrevCornerBottomRight, 20);
    lv_obj_set_align(ui_ButtonPrevCornerBottomRight, LV_ALIGN_BOTTOM_RIGHT);
    lv_obj_clear_flag(ui_ButtonPrevCornerBottomRight, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);   /// Flags
    lv_obj_set_style_bg_color(ui_ButtonPrevCornerBottomRight, lv_color_hex(0x009BFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_ButtonPrevCornerBottomRight, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui_ButtonPrevCornerBottomRight, false, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t * ui_btnPrev = lv_btn_create(ui_ButtonPrevGroup);
    lv_obj_set_width(ui_btnPrev, 100);
    lv_obj_set_height(ui_btnPrev, 40);
    lv_obj_set_align(ui_btnPrev, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_btnPrev, LV_OBJ_FLAG_EVENT_BUBBLE);   /// Flags
    lv_obj_clear_flag(ui_btnPrev, LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE |
             LV_OBJ_FLAG_SNAPPABLE | LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
             LV_OBJ_FLAG_SCROLL_CHAIN);   /// Flags
    lv_obj_add_event_cb(ui_btnPrev, [](lv_event_t *e)
                        {
        auto self = static_cast<DataBundleSelectionGui*>(lv_event_get_user_data(e));
        self->goToPreviousPage(); }, LV_EVENT_CLICKED, this);

    lv_obj_t * ui_btnPrevLabel = lv_label_create(ui_btnPrev);
    lv_label_set_text(ui_btnPrevLabel, "Prev");
    lv_obj_center(ui_btnPrevLabel);
    lv_obj_set_style_text_font(ui_btnPrevLabel, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);

    //logMessage("Navigation buttons added to widget\n");
}

void DataBundleSelectionGui::addControlButtonsToWidget(lv_obj_t *parentWidget)
{
    if (!parentWidget)
        return;

    lv_obj_t * ui_btnBackGroup = lv_obj_create(parentWidget);
    lv_obj_remove_style_all(ui_btnBackGroup);
    lv_obj_set_width(ui_btnBackGroup, 100);
    lv_obj_set_height(ui_btnBackGroup, 40);
    lv_obj_clear_flag(ui_btnBackGroup, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE); /// Flags

    lv_obj_t * ui_btnBackCornerBottomLeft = lv_obj_create(ui_btnBackGroup);
    lv_obj_remove_style_all(ui_btnBackCornerBottomLeft);
    lv_obj_set_width(ui_btnBackCornerBottomLeft, 20);
    lv_obj_set_height(ui_btnBackCornerBottomLeft, 20);
    lv_obj_set_align(ui_btnBackCornerBottomLeft, LV_ALIGN_BOTTOM_LEFT);
    lv_obj_clear_flag(ui_btnBackCornerBottomLeft, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_set_style_bg_color(ui_btnBackCornerBottomLeft, lv_color_hex(0x009BFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_btnBackCornerBottomLeft, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui_btnBackCornerBottomLeft, false, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t * ui_btnBackCornerTopRight = lv_obj_create(ui_btnBackGroup);
    lv_obj_remove_style_all(ui_btnBackCornerTopRight);
    lv_obj_set_width(ui_btnBackCornerTopRight, 20);
    lv_obj_set_height(ui_btnBackCornerTopRight, 20);
    lv_obj_set_align(ui_btnBackCornerTopRight, LV_ALIGN_TOP_RIGHT);
    lv_obj_clear_flag(ui_btnBackCornerTopRight, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_set_style_bg_color(ui_btnBackCornerTopRight, lv_color_hex(0x009BFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_btnBackCornerTopRight, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui_btnBackCornerTopRight, false, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Back button for returning to menu
    lv_obj_t * ui_btnBack = lv_btn_create(ui_btnBackGroup);
    lv_obj_set_width(ui_btnBack, 100);
    lv_obj_set_height(ui_btnBack, 40);
    lv_obj_set_align(ui_btnBack, LV_ALIGN_CENTER);
    lv_obj_add_event_cb(ui_btnBack, [](lv_event_t *e)
                        {
        auto self = static_cast<DataBundleSelectionGui*>(lv_event_get_user_data(e));
        // // logMessage("Back button pressed - returning to menu\n");
        switchToVisualization(); }, LV_EVENT_CLICKED, this);

    lv_obj_t * ui_btnBackLabel = lv_label_create(ui_btnBack);
    lv_label_set_text(ui_btnBackLabel, "Back");
    lv_obj_center(ui_btnBackLabel);
    lv_obj_set_style_text_font(ui_btnBackLabel, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);

    // // logMessage("Control buttons added to widget\n");
}

void DataBundleSelectionGui::showShadowOverlay()
{
    if (ui_ShadowOverlay)
    {
        lv_obj_del(ui_ShadowOverlay);
        ui_ShadowOverlay = nullptr;
    }

    ui_ShadowOverlay = lv_obj_create(lv_scr_act());
    lv_obj_clear_flag(ui_ShadowOverlay, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_flag(ui_ShadowOverlay, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_size(ui_ShadowOverlay, lv_pct(100), lv_pct(100));
    lv_obj_align(ui_ShadowOverlay, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_radius(ui_ShadowOverlay, 0, 0);

    lv_obj_set_style_bg_color(ui_ShadowOverlay, lv_color_black(), 0);
    lv_obj_set_style_bg_opa(ui_ShadowOverlay, LV_OPA_50, 0);
    lv_obj_set_style_border_width(ui_ShadowOverlay, 0, 0);
}

void DataBundleSelectionGui::hideShadowOverlay()
{
    if (ui_ShadowOverlay)
    {
        lv_obj_del(ui_ShadowOverlay);
        ui_ShadowOverlay = nullptr;
    }
}

void DataBundleSelectionGui::handleClearButtonClick(unsigned char index)
{
    // Add buttons
    static const char *btns[] = {"Yes", ""};
    // Show confirmation dialog before clearing history
    showShadowOverlay();
    lv_obj_t *confirmDialog = lv_msgbox_create(lv_scr_act(), "Confirm Deletion", "Are you sure you want to delete this data bundle?", btns, true);
    lv_obj_set_width(confirmDialog, 250);
    lv_obj_center(confirmDialog);
    lv_obj_move_foreground(confirmDialog);
    lv_obj_add_event_cb(confirmDialog, [](lv_event_t *e)
                        {
        auto self = static_cast<DataBundleSelectionGui*>(lv_event_get_user_data(e));
        int index = (intptr_t)lv_obj_get_user_data(lv_event_get_target(e));
        lv_event_code_t code = lv_event_get_code(e);

        if (code == LV_EVENT_VALUE_CHANGED)
        {
            lv_obj_t *msgbox = lv_event_get_current_target(e);
            const char *btnText = lv_msgbox_get_active_btn_text(msgbox);
            if (btnText && strcmp(btnText, "Yes") == 0)
            {
                self->handleClearConfirmButtonClick(index);
            }
            self->hideShadowOverlay();
            lv_obj_del(msgbox);
        }
        else if (code == LV_EVENT_DELETE)
        {
            self->hideShadowOverlay();
        } }, LV_EVENT_ALL, this);

        lv_obj_set_user_data(confirmDialog, (void *)(intptr_t)index);
}

void DataBundleSelectionGui::handleClearConfirmButtonClick(unsigned char index)
{
    dataBundleManager.deleteDataBundle(currentPage * 6 + index);
    updateBundles();
}

void DataBundleSelectionGui::addLogoPanelToWidget(lv_obj_t *parentWidget)
{
ui_LogoGroup = lv_obj_create(parentWidget);
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
lv_obj_clear_flag(ui_LogoCornerBottomLeft, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE); /// Flags
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
lv_obj_clear_flag(ui_LogoCornerFillBottomLeft, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE); /// Flags
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
lv_obj_clear_flag(ui_LogoCornerFillBottomRight, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE); /// Flags
lv_obj_set_style_radius(ui_LogoCornerFillBottomRight, 1000, LV_PART_MAIN | LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_LogoCornerFillBottomRight, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
lv_obj_set_style_bg_opa(ui_LogoCornerFillBottomRight, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
lv_obj_set_style_clip_corner(ui_LogoCornerFillBottomRight, false, LV_PART_MAIN | LV_STATE_DEFAULT);

ui_LogoOutlay = lv_obj_create(ui_LogoGroup);
lv_obj_remove_style_all(ui_LogoOutlay);
lv_obj_set_width(ui_LogoOutlay, 80);
lv_obj_set_height(ui_LogoOutlay, 25);
lv_obj_set_align(ui_LogoOutlay, LV_ALIGN_CENTER);
lv_obj_clear_flag(ui_LogoOutlay, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE); /// Flags
lv_obj_set_style_radius(ui_LogoOutlay, 1000, LV_PART_MAIN | LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_LogoOutlay, lv_color_hex(0x055DA9), LV_PART_MAIN | LV_STATE_DEFAULT);
lv_obj_set_style_bg_opa(ui_LogoOutlay, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
lv_obj_set_style_clip_corner(ui_LogoOutlay, false, LV_PART_MAIN | LV_STATE_DEFAULT);

ui_LogoImage = lv_img_create(ui_LogoGroup);
lv_img_set_src(ui_LogoImage, &ui_img_mtalogo_png);
lv_obj_set_width(ui_LogoImage, LV_SIZE_CONTENT); /// 1
lv_obj_set_height(ui_LogoImage, LV_SIZE_CONTENT); /// 1
lv_obj_set_align(ui_LogoImage, LV_ALIGN_CENTER);
lv_obj_clear_flag(ui_LogoImage, LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE |
                                LV_OBJ_FLAG_SNAPPABLE | LV_OBJ_FLAG_SCROLLABLE); /// Flags
lv_img_set_zoom(ui_LogoImage, 80);
}

void DataBundleSelectionGui::showDataBundles()
{
    if (!initialized || !ui_DataBundlesWidget)
        return;

    lv_obj_clear_flag(ui_DataBundlesWidget, LV_OBJ_FLAG_HIDDEN);

    dataBundleManager.loadAllDataBundleNames();
    dataBundleManager.listAllBundles();

    updateBundles();
}

void DataBundleSelectionGui::hideDataBundles()
{
    if (!initialized || !ui_DataBundlesWidget)
        return;

    lv_obj_add_flag(ui_DataBundlesWidget, LV_OBJ_FLAG_HIDDEN);
}

void DataBundleSelectionGui::hideSpecificDataBundle(unsigned char index){
    if(ui_DataBundle[index])
        lv_obj_del(ui_DataBundle[index]);

    ui_DataBundle[index] = nullptr;
    ui_DataBundleHeaderGroup[index] = nullptr;
    ui_DataBundleHeaderCornerBottomLeft[index] = nullptr;
    ui_DataBundleHeaderCornerBottomRight[index] = nullptr;
    ui_DataBundleHeader[index] = nullptr;
    ui_DataBundleHeaderLabel[index] = nullptr;
    ui_DataBundleChart[index] = nullptr;
    ui_DataBundleChart_series_1[index] = nullptr;
    ui_DataBundleFooterGroup[index] = nullptr;
    ui_DataBundleFooterBridge[index] = nullptr;
    ui_DataBundleFooterBridgeFill[index] = nullptr;
    ui_DataBundleFooterTimerGroup[index] = nullptr;
    ui_DataBundleFooterDateCornerTopLeft[index] = nullptr;
    ui_DataBundleFooterDateCornerTopRight[index] = nullptr;
    ui_DataBundleFooterDate[index] = nullptr;
    ui_DataBundleFooterLabelDate[index] = nullptr;
    ui_DataBundleFooterLabelTime[index] = nullptr;
    ui_DataBundleFooterButtonsGroup[index] = nullptr;
    ui_DataBundleFooterButtonsCornerTopLeft[index] = nullptr;
    ui_DataBundleFooterButtonsCornerTopRight[index] = nullptr;
    ui_DataBundleFooterButtons[index] = nullptr;
    ui_DataBundleFooterButtonExport[index] = nullptr;
    ui_DataBundleFooterButtonExportImage[index] = nullptr;
    ui_DataBundleFooterButtonClear[index] = nullptr;
    ui_DataBundleFooterButtonClearImage[index] = nullptr;
}

void DataBundleSelectionGui::goToNextPage(){
    if((dataBundleManager.getDataBundleAmount()-1)/6 != 0){
        unsigned char availablePages = ((dataBundleManager.getDataBundleAmount()-1)/6);
        currentPage = (currentPage+1)%(availablePages+1);
    }
    //logMessage("Current page is: %d", currentPage);
    updateBundles();
}

void DataBundleSelectionGui::goToPreviousPage(){
    if((dataBundleManager.getDataBundleAmount()-1)/6 != 0){
        unsigned char availablePages = ((dataBundleManager.getDataBundleAmount()-1)/6);
        currentPage = (currentPage == 0) ? availablePages : currentPage - 1;
    }
    //logMessage("Current page is: %d", currentPage);
    updateBundles();
}

void DataBundleSelectionGui::updateWatcherCells(){
    for(unsigned char i=0;i<5;i++){
        if(i == currentPage){
            lv_obj_set_style_bg_color(ui_DataBundlePageWatcherCell[i], lv_color_hex(0x009BFF), LV_PART_MAIN);
            lv_obj_set_style_border_color(ui_DataBundlePageWatcherCell[i], lv_color_hex(0x009BFF), LV_PART_MAIN);
            continue;
        }
        else if(i <= ((dataBundleManager.getDataBundleAmount()-1)/6)){
            lv_obj_set_style_bg_color(ui_DataBundlePageWatcherCell[i], lv_color_hex(0xFFFFFF), LV_PART_MAIN);
            lv_obj_set_style_border_color(ui_DataBundlePageWatcherCell[i], lv_color_hex(0x009BFF), LV_PART_MAIN);
            continue;
        }
        else{
            lv_obj_set_style_bg_color(ui_DataBundlePageWatcherCell[i], lv_color_hex(0xAAAAAA), LV_PART_MAIN);
            lv_obj_set_style_border_color(ui_DataBundlePageWatcherCell[i], lv_color_hex(0xAAAAAA), LV_PART_MAIN);
            continue;
        }
    }
}

void DataBundleSelectionGui::updateBundles()
{
    try 
    {
        std::array<DataBundleBuffer, 6> currentDataBundles = dataBundleManager.getDataBundles(currentPage);

        for (unsigned char i = 0; i < 6; i++)
        {
            if (currentDataBundles[i].metaBuffer.sensorName.empty())
            {
                if (ui_DataBundle[i])
                {
                    hideSpecificDataBundle(i);
                }
                continue;
            }

            const char* sName = currentDataBundles[i].metaBuffer.sensorName.c_str();
            const char* sTime = currentDataBundles[i].startTime.empty() ? "00:00" : currentDataBundles[i].startTime.c_str();
            const char* sDate = currentDataBundles[i].metaBuffer.startDate.empty() ? "01.01.2000" : currentDataBundles[i].metaBuffer.startDate.c_str();

            createDataBundle(
                i,
                sName,
                sTime,
                sDate,
                currentDataBundles[i].dataBuffer
            );
        }

        updateWatcherCells();
    }
    catch (const std::exception& e)
    {
        logMessage("CRASH in updateBundles: %s", e.what());
    }
    catch (...)
    {
        logMessage("CRASH in updateBundles: Unknown Error");
    }
}