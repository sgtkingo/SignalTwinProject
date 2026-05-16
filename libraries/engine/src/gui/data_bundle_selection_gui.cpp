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
#include "expt.hpp"
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <cstdlib>

extern int touch_point_count __attribute__((weak));
extern int touch_last_x __attribute__((weak));
extern int touch_last_y __attribute__((weak));
extern int touch_second_x __attribute__((weak));
extern int touch_second_y __attribute__((weak));

namespace
{
constexpr uint32_t BUNDLE_VIEWER_COLORS[] = {
    0x009BFF,
    0xFF6B35,
    0x00B894,
    0x9B51E0,
    0xF2C94C,
    0xEB5757
};

std::vector<std::string> splitSemicolonLine(const std::string &line)
{
    std::vector<std::string> result;
    size_t start = 0;
    while (start <= line.size())
    {
        const size_t separator = line.find(';', start);
        if (separator == std::string::npos)
        {
            result.push_back(line.substr(start));
            break;
        }

        result.push_back(line.substr(start, separator - start));
        start = separator + 1;
    }
    return result;
}

bool parseNumericValue(const std::string &text, double &value)
{
    if (text.empty())
    {
        return false;
    }

    char *end = nullptr;
    value = std::strtod(text.c_str(), &end);
    return end && *end == '\0';
}

uint32_t getBundleViewerColor(uint8_t index)
{
    return BUNDLE_VIEWER_COLORS[index % (sizeof(BUNDLE_VIEWER_COLORS) / sizeof(BUNDLE_VIEWER_COLORS[0]))];
}
}

DataBundleSelectionGui::DataBundleSelectionGui(GuiRouter &router, DataBundleManager &dataBundleManager) : router(router), dataBundleManager(dataBundleManager)
{
    ui_DataBundlesWidget = nullptr;
    ui_DataBundlePageWatcher = nullptr;
    ui_ShadowOverlay = nullptr;
    ui_BundleViewerOverlay = nullptr;
    ui_BundleViewerPanel = nullptr;
    ui_DeleteAllButtonGroup = nullptr;
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
        Exception("DataBundleSelectionGui::init", e.what()).print();
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
    lv_obj_set_size(ui_DataBundle[i], 200, 165);
    bindBundleOpenEvent(ui_DataBundle[i], i);
    
    // Apply the calculated position
    lv_obj_set_align(ui_DataBundle[i], LV_ALIGN_CENTER);
    lv_obj_set_pos(ui_DataBundle[i], x_pos, y_pos);

    // --- Header Group ---
    ui_DataBundleHeaderGroup[i] = lv_obj_create(ui_DataBundle[i]);
    lv_obj_remove_style_all(ui_DataBundleHeaderGroup[i]);
    lv_obj_set_size(ui_DataBundleHeaderGroup[i], 200, 20);
    lv_obj_set_align(ui_DataBundleHeaderGroup[i], LV_ALIGN_TOP_MID);
    bindBundleOpenEvent(ui_DataBundleHeaderGroup[i], i);

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
    bindBundleOpenEvent(ui_DataBundleHeader[i], i);

    ui_DataBundleHeaderLabel[i] = lv_label_create(ui_DataBundleHeader[i]);
    lv_obj_set_align(ui_DataBundleHeaderLabel[i], LV_ALIGN_CENTER);
    lv_label_set_text(ui_DataBundleHeaderLabel[i], "Bundle Title"); 
    lv_obj_set_style_text_font(ui_DataBundleHeaderLabel[i], &lv_font_montserrat_12, LV_PART_MAIN);
    lv_obj_set_style_text_color(ui_DataBundleHeaderLabel[i], lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    bindBundleOpenEvent(ui_DataBundleHeaderLabel[i], i);

    // --- Chart ---
    ui_DataBundleChart[i] = lv_chart_create(ui_DataBundle[i]);
    lv_obj_set_size(ui_DataBundleChart[i], 200, 110);
    lv_obj_set_y(ui_DataBundleChart[i], -6);
    lv_obj_set_align(ui_DataBundleChart[i], LV_ALIGN_CENTER);
    lv_chart_set_type(ui_DataBundleChart[i], LV_CHART_TYPE_LINE);
    lv_chart_set_point_count(ui_DataBundleChart[i], 10);
    lv_chart_set_div_line_count(ui_DataBundleChart[i], 5, 10);
    lv_obj_set_style_radius(ui_DataBundleChart[i], 0, LV_PART_MAIN);
    lv_obj_set_style_bg_color(ui_DataBundleChart[i], lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    bindBundleOpenEvent(ui_DataBundleChart[i], i);
    
    ui_DataBundleChart_series_1[i] = lv_chart_add_series(ui_DataBundleChart[i], lv_color_hex(0xFF8200), LV_CHART_AXIS_PRIMARY_Y);

    // --- Footer Group ---
    ui_DataBundleFooterGroup[i] = lv_obj_create(ui_DataBundle[i]);
    lv_obj_remove_style_all(ui_DataBundleFooterGroup[i]);
    lv_obj_set_size(ui_DataBundleFooterGroup[i], 200, 35);
    lv_obj_set_align(ui_DataBundleFooterGroup[i], LV_ALIGN_BOTTOM_MID);

    ui_DataBundleFooterBridge[i] = lv_obj_create(ui_DataBundleFooterGroup[i]);
    lv_obj_remove_style_all(ui_DataBundleFooterBridge[i]);
    lv_obj_set_size(ui_DataBundleFooterBridge[i], 40, 15);
    lv_obj_set_pos(ui_DataBundleFooterBridge[i], 25, -9);
    lv_obj_set_align(ui_DataBundleFooterBridge[i], LV_ALIGN_CENTER);
    lv_obj_set_style_bg_color(ui_DataBundleFooterBridge[i], lv_color_hex(0x007CCC), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(ui_DataBundleFooterBridge[i], 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_DataBundleFooterBridgeFill[i] = lv_obj_create(ui_DataBundleFooterGroup[i]);
    lv_obj_remove_style_all(ui_DataBundleFooterBridgeFill[i]);
    lv_obj_set_size(ui_DataBundleFooterBridgeFill[i], 22, 15);
    lv_obj_set_pos(ui_DataBundleFooterBridgeFill[i], 19, -4);
    lv_obj_set_align(ui_DataBundleFooterBridgeFill[i], LV_ALIGN_CENTER);
    lv_obj_set_style_radius(ui_DataBundleFooterBridgeFill[i], 5, LV_PART_MAIN);
    lv_obj_set_style_bg_color(ui_DataBundleFooterBridgeFill[i], lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(ui_DataBundleFooterBridgeFill[i], 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    // --- Footer Timer ---
    ui_DataBundleFooterTimerGroup[i] = lv_obj_create(ui_DataBundleFooterGroup[i]);
    lv_obj_remove_style_all(ui_DataBundleFooterTimerGroup[i]);
    lv_obj_set_size(ui_DataBundleFooterTimerGroup[i], 108, 25);
    lv_obj_set_pos(ui_DataBundleFooterTimerGroup[i], 0, -9);
    lv_obj_set_align(ui_DataBundleFooterTimerGroup[i], LV_ALIGN_BOTTOM_LEFT);
    bindBundleOpenEvent(ui_DataBundleFooterTimerGroup[i], i);

    ui_DataBundleFooterDateCornerTopLeft[i] = lv_obj_create(ui_DataBundleFooterTimerGroup[i]);
    lv_obj_remove_style_all(ui_DataBundleFooterDateCornerTopLeft[i]);
    lv_obj_set_size(ui_DataBundleFooterDateCornerTopLeft[i], 10, 10);
    lv_obj_set_align(ui_DataBundleFooterDateCornerTopLeft[i], LV_ALIGN_TOP_LEFT);
    lv_obj_set_style_bg_color(ui_DataBundleFooterDateCornerTopLeft[i], lv_color_hex(0x007CCC), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(ui_DataBundleFooterDateCornerTopLeft[i], 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_DataBundleFooterDateCornerTopRight[i] = lv_obj_create(ui_DataBundleFooterTimerGroup[i]);
    lv_obj_remove_style_all(ui_DataBundleFooterDateCornerTopRight[i]);
    lv_obj_set_size(ui_DataBundleFooterDateCornerTopRight[i], 10, 10);
    lv_obj_set_align(ui_DataBundleFooterDateCornerTopRight[i], LV_ALIGN_TOP_RIGHT);
    lv_obj_set_style_bg_color(ui_DataBundleFooterDateCornerTopRight[i], lv_color_hex(0x007CCC), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(ui_DataBundleFooterDateCornerTopRight[i], 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_DataBundleFooterDate[i] = lv_obj_create(ui_DataBundleFooterTimerGroup[i]);
    lv_obj_remove_style_all(ui_DataBundleFooterDate[i]);
    lv_obj_set_size(ui_DataBundleFooterDate[i], 108, 25);
    lv_obj_set_align(ui_DataBundleFooterDate[i], LV_ALIGN_TOP_MID);
    lv_obj_set_style_radius(ui_DataBundleFooterDate[i], 10, LV_PART_MAIN);  
    lv_obj_set_style_bg_color(ui_DataBundleFooterDate[i], lv_color_hex(0x007CCC), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(ui_DataBundleFooterDate[i], 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    bindBundleOpenEvent(ui_DataBundleFooterDate[i], i);

    ui_DataBundleFooterLabelDate[i] = lv_label_create(ui_DataBundleFooterDate[i]);
    lv_label_set_text(ui_DataBundleFooterLabelDate[i], "01.01.2024");
    lv_obj_set_x(ui_DataBundleFooterLabelDate[i], 5);
    lv_obj_set_align(ui_DataBundleFooterLabelDate[i], LV_ALIGN_LEFT_MID);
    lv_obj_set_style_text_font(ui_DataBundleFooterLabelDate[i], &lv_font_montserrat_12, LV_PART_MAIN);
    lv_obj_set_style_text_color(ui_DataBundleFooterLabelDate[i], lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    bindBundleOpenEvent(ui_DataBundleFooterLabelDate[i], i);

    ui_DataBundleFooterLabelTime[i] = lv_label_create(ui_DataBundleFooterDate[i]);
    lv_label_set_text(ui_DataBundleFooterLabelTime[i], "12:00");
    lv_obj_set_x(ui_DataBundleFooterLabelTime[i], -3);
    lv_obj_set_align(ui_DataBundleFooterLabelTime[i], LV_ALIGN_RIGHT_MID);
    lv_obj_set_style_text_font(ui_DataBundleFooterLabelTime[i], &lv_font_montserrat_12, LV_PART_MAIN);
    lv_obj_set_style_text_color(ui_DataBundleFooterLabelTime[i], lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    bindBundleOpenEvent(ui_DataBundleFooterLabelTime[i], i);

    // --- Footer Buttons ---
    ui_DataBundleFooterButtonsGroup[i] = lv_obj_create(ui_DataBundleFooterGroup[i]);
    lv_obj_remove_style_all(ui_DataBundleFooterButtonsGroup[i]);
    lv_obj_set_size(ui_DataBundleFooterButtonsGroup[i], 70, 35);
    lv_obj_set_pos(ui_DataBundleFooterButtonsGroup[i], 0, 1);
    lv_obj_set_align(ui_DataBundleFooterButtonsGroup[i], LV_ALIGN_BOTTOM_RIGHT);

    ui_DataBundleFooterButtonsCornerTopLeft[i] = lv_obj_create(ui_DataBundleFooterButtonsGroup[i]);
    lv_obj_remove_style_all(ui_DataBundleFooterButtonsCornerTopLeft[i]);
    lv_obj_set_size(ui_DataBundleFooterButtonsCornerTopLeft[i], 10, 10);
    lv_obj_set_align(ui_DataBundleFooterButtonsCornerTopLeft[i], LV_ALIGN_TOP_LEFT);
    lv_obj_set_style_bg_color(ui_DataBundleFooterButtonsCornerTopLeft[i], lv_color_hex(0x007CCC), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(ui_DataBundleFooterButtonsCornerTopLeft[i], 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_DataBundleFooterButtonsCornerTopRight[i] = lv_obj_create(ui_DataBundleFooterButtonsGroup[i]);
    lv_obj_remove_style_all(ui_DataBundleFooterButtonsCornerTopRight[i]);
    lv_obj_set_size(ui_DataBundleFooterButtonsCornerTopRight[i], 10, 10);
    lv_obj_set_align(ui_DataBundleFooterButtonsCornerTopRight[i], LV_ALIGN_TOP_RIGHT);
    lv_obj_set_style_bg_color(ui_DataBundleFooterButtonsCornerTopRight[i], lv_color_hex(0x007CCC), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(ui_DataBundleFooterButtonsCornerTopRight[i], 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_DataBundleFooterButtons[i] = lv_obj_create(ui_DataBundleFooterButtonsGroup[i]);
    lv_obj_remove_style_all(ui_DataBundleFooterButtons[i]);
    lv_obj_set_size(ui_DataBundleFooterButtons[i], 70, 35);
    lv_obj_set_align(ui_DataBundleFooterButtons[i], LV_ALIGN_TOP_MID);
    lv_obj_set_style_radius(ui_DataBundleFooterButtons[i], 10, LV_PART_MAIN);
    lv_obj_set_style_bg_color(ui_DataBundleFooterButtons[i], lv_color_hex(0x007CCC), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(ui_DataBundleFooterButtons[i], 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_DataBundleFooterButtonExport[i] = lv_btn_create(ui_DataBundleFooterButtons[i]);
    lv_obj_set_size(ui_DataBundleFooterButtonExport[i], 28, 24);
    lv_obj_set_pos(ui_DataBundleFooterButtonExport[i], 5, -1);
    lv_obj_set_align(ui_DataBundleFooterButtonExport[i], LV_ALIGN_LEFT_MID);
    lv_obj_set_style_radius(ui_DataBundleFooterButtonExport[i], 5, LV_PART_MAIN);
    lv_obj_add_event_cb(ui_DataBundleFooterButtonExport[i], [](lv_event_t *e)
    {
        auto self = static_cast<DataBundleSelectionGui*>(lv_event_get_user_data(e));
        self->router.showFileTransfer();
    }, LV_EVENT_CLICKED, this);
    
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
        lv_chart_set_all_value(ui_DataBundleChart[i], ui_DataBundleChart_series_1[i], LV_CHART_POINT_NONE);

        for (int j = 0; j < 10; j++)
        {
            int val = 0;

            if(!values[j].empty()) {
                val = static_cast<int>(atof(values[j].c_str()) * 100.0);
            }

            lv_chart_set_value_by_id(ui_DataBundleChart[i], ui_DataBundleChart_series_1[i], j, val);

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
        self->router.navigateBackFromDatabank(); }, LV_EVENT_CLICKED, this);

    lv_obj_t * ui_btnBackLabel = lv_label_create(ui_btnBack);
    lv_label_set_text(ui_btnBackLabel, "Back");
    lv_obj_center(ui_btnBackLabel);
    lv_obj_set_style_text_font(ui_btnBackLabel, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_DeleteAllButtonGroup = lv_obj_create(parentWidget);
    lv_obj_remove_style_all(ui_DeleteAllButtonGroup);
    lv_obj_set_width(ui_DeleteAllButtonGroup, 120);
    lv_obj_set_height(ui_DeleteAllButtonGroup, 34);
    lv_obj_align(ui_DeleteAllButtonGroup, LV_ALIGN_TOP_RIGHT, -18, 16);
    lv_obj_clear_flag(ui_DeleteAllButtonGroup, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t *ui_btnDeleteAll = lv_btn_create(ui_DeleteAllButtonGroup);
    lv_obj_set_width(ui_btnDeleteAll, 120);
    lv_obj_set_height(ui_btnDeleteAll, 34);
    lv_obj_set_align(ui_btnDeleteAll, LV_ALIGN_CENTER);
    lv_obj_set_style_radius(ui_btnDeleteAll, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_btnDeleteAll, lv_color_hex(0xE55858), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_btnDeleteAll, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(ui_btnDeleteAll, [](lv_event_t *e)
                        {
        auto self = static_cast<DataBundleSelectionGui*>(lv_event_get_user_data(e));
        self->handleDeleteAllButtonClick(); }, LV_EVENT_CLICKED, this);

    lv_obj_t *ui_btnDeleteAllLabel = lv_label_create(ui_btnDeleteAll);
    lv_label_set_text(ui_btnDeleteAllLabel, "Delete All");
    lv_obj_center(ui_btnDeleteAllLabel);
    lv_obj_set_style_text_font(ui_btnDeleteAllLabel, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);

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

void DataBundleSelectionGui::bindBundleOpenEvent(lv_obj_t *object, unsigned char index)
{
    if (!object)
    {
        return;
    }

    lv_obj_add_flag(object, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_user_data(object, reinterpret_cast<void *>(static_cast<intptr_t>(index)));
    lv_obj_add_event_cb(object, [](lv_event_t *e)
                        {
        if (lv_event_get_code(e) != LV_EVENT_CLICKED) {
            return;
        }

        auto self = static_cast<DataBundleSelectionGui*>(lv_event_get_user_data(e));
        lv_obj_t *target = lv_event_get_current_target(e);
        const unsigned char index = static_cast<unsigned char>(reinterpret_cast<intptr_t>(lv_obj_get_user_data(target)));
        self->showBundleViewer(index); }, LV_EVENT_CLICKED, this);
}

void DataBundleSelectionGui::showBundleViewer(unsigned char index)
{
    closeBundleViewer();

    const unsigned char bundleIndex = static_cast<unsigned char>(currentPage * 6 + index);
    const std::string csvText = dataBundleManager.getBundleCsvText(bundleIndex, 16000);
    if (csvText.empty())
    {
        splashMessage("DataBundle could not be opened.");
        return;
    }

    const char *titleText = "DataBundle";
    if (index < 6 && ui_DataBundleHeaderLabel[index])
    {
        titleText = lv_label_get_text(ui_DataBundleHeaderLabel[index]);
    }

    parseBundleViewerCsv(csvText);

    ui_BundleViewerOverlay = lv_obj_create(lv_scr_act());
    lv_obj_clear_flag(ui_BundleViewerOverlay, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_flag(ui_BundleViewerOverlay, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_size(ui_BundleViewerOverlay, lv_pct(100), lv_pct(100));
    lv_obj_center(ui_BundleViewerOverlay);
    lv_obj_set_style_radius(ui_BundleViewerOverlay, 0, LV_PART_MAIN);
    lv_obj_set_style_bg_color(ui_BundleViewerOverlay, lv_color_black(), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(ui_BundleViewerOverlay, LV_OPA_50, LV_PART_MAIN);
    lv_obj_set_style_border_width(ui_BundleViewerOverlay, 0, LV_PART_MAIN);

    ui_BundleViewerPanel = lv_obj_create(ui_BundleViewerOverlay);
    lv_obj_clear_flag(ui_BundleViewerPanel, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(ui_BundleViewerPanel, 780, 460);
    lv_obj_center(ui_BundleViewerPanel);
    lv_obj_set_style_radius(ui_BundleViewerPanel, 0, LV_PART_MAIN);
    lv_obj_set_style_bg_color(ui_BundleViewerPanel, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(ui_BundleViewerPanel, 255, LV_PART_MAIN);
    lv_obj_set_style_border_width(ui_BundleViewerPanel, 2, LV_PART_MAIN);
    lv_obj_set_style_border_color(ui_BundleViewerPanel, lv_color_hex(0x007CCC), LV_PART_MAIN);

    lv_obj_t *title = lv_label_create(ui_BundleViewerPanel);
    lv_label_set_text_fmt(title, "%s DataBundle", titleText);
    lv_obj_align(title, LV_ALIGN_TOP_LEFT, 18, 14);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_16, LV_PART_MAIN);
    lv_obj_set_style_text_color(title, lv_color_hex(0x111111), LV_PART_MAIN);

    ui_BundleViewerGraphTab = lv_btn_create(ui_BundleViewerPanel);
    lv_obj_set_size(ui_BundleViewerGraphTab, 76, 30);
    lv_obj_align(ui_BundleViewerGraphTab, LV_ALIGN_TOP_LEFT, 18, 48);
    lv_obj_set_style_radius(ui_BundleViewerGraphTab, 5, LV_PART_MAIN);
    lv_obj_add_event_cb(ui_BundleViewerGraphTab, [](lv_event_t *e)
                        {
        auto self = static_cast<DataBundleSelectionGui*>(lv_event_get_user_data(e));
        self->setBundleViewerMode(false); }, LV_EVENT_CLICKED, this);

    lv_obj_t *graphLabel = lv_label_create(ui_BundleViewerGraphTab);
    lv_label_set_text(graphLabel, "Graph");
    lv_obj_center(graphLabel);
    lv_obj_set_style_text_font(graphLabel, &lv_font_montserrat_12, LV_PART_MAIN);

    ui_BundleViewerCsvTab = lv_btn_create(ui_BundleViewerPanel);
    lv_obj_set_size(ui_BundleViewerCsvTab, 76, 30);
    lv_obj_align(ui_BundleViewerCsvTab, LV_ALIGN_TOP_LEFT, 100, 48);
    lv_obj_set_style_radius(ui_BundleViewerCsvTab, 5, LV_PART_MAIN);
    lv_obj_add_event_cb(ui_BundleViewerCsvTab, [](lv_event_t *e)
                        {
        auto self = static_cast<DataBundleSelectionGui*>(lv_event_get_user_data(e));
        self->setBundleViewerMode(true); }, LV_EVENT_CLICKED, this);

    lv_obj_t *csvTabLabel = lv_label_create(ui_BundleViewerCsvTab);
    lv_label_set_text(csvTabLabel, "CSV");
    lv_obj_center(csvTabLabel);
    lv_obj_set_style_text_font(csvTabLabel, &lv_font_montserrat_12, LV_PART_MAIN);

    ui_BundleViewerSettingsButton = lv_btn_create(ui_BundleViewerPanel);
    lv_obj_set_size(ui_BundleViewerSettingsButton, 34, 32);
    lv_obj_align(ui_BundleViewerSettingsButton, LV_ALIGN_TOP_RIGHT, -98, 10);
    lv_obj_set_style_radius(ui_BundleViewerSettingsButton, 16, LV_PART_MAIN);
    lv_obj_add_event_cb(ui_BundleViewerSettingsButton, [](lv_event_t *e)
                        {
        auto self = static_cast<DataBundleSelectionGui*>(lv_event_get_user_data(e));
        self->showBundleViewerSettings(); }, LV_EVENT_CLICKED, this);

    lv_obj_t *settingsImage = lv_img_create(ui_BundleViewerSettingsButton);
    lv_img_set_src(settingsImage, &ui_img_settings_png);
    lv_obj_center(settingsImage);
    lv_img_set_zoom(settingsImage, 92);

    lv_obj_t *closeButton = lv_btn_create(ui_BundleViewerPanel);
    lv_obj_set_size(closeButton, 76, 32);
    lv_obj_align(closeButton, LV_ALIGN_TOP_RIGHT, -14, 10);
    lv_obj_set_style_radius(closeButton, 5, LV_PART_MAIN);
    lv_obj_add_event_cb(closeButton, [](lv_event_t *e)
                        {
        auto self = static_cast<DataBundleSelectionGui*>(lv_event_get_user_data(e));
        self->closeBundleViewer(); }, LV_EVENT_CLICKED, this);

    lv_obj_t *closeLabel = lv_label_create(closeButton);
    lv_label_set_text(closeLabel, "Close");
    lv_obj_center(closeLabel);
    lv_obj_set_style_text_font(closeLabel, &lv_font_montserrat_14, LV_PART_MAIN);

    createBundleViewerContent();
    setBundleViewerMode(false);
    lv_obj_move_foreground(ui_BundleViewerGraphTab);
    lv_obj_move_foreground(ui_BundleViewerCsvTab);
    lv_obj_move_foreground(ui_BundleViewerSettingsButton);
    lv_obj_move_foreground(closeButton);

    lv_obj_move_foreground(ui_BundleViewerOverlay);
}

void DataBundleSelectionGui::parseBundleViewerCsv(const std::string &csvText)
{
    bundleViewerHeaders.clear();
    bundleViewerRows.clear();
    bundleViewerSignals.clear();
    bundleViewerSelectedSignals.clear();
    bundleViewerCsvMode = false;
    bundleViewerHistoryOffset = 0;
    bundleViewerDragAccumulatorPx = 0;
    bundleViewerVisibleSampleCount = BUNDLE_VIEW_DEFAULT_CHART_POINTS;
    bundleViewerXTickMax = 10;
    bundleViewerPinchLastDistancePx = 0;
    bundleViewerPinchAccumulatorPx = 0;
    bundleViewerPinchActive = false;
    bundleViewerCursorIndex = bundleViewerVisibleSampleCount / 2;
    bundleViewerCursorSignalSlot = 0;
    bundleViewerCursorVisible = false;

    size_t start = 0;
    bool headerRead = false;
    int signalIndex = -1;
    int valueIndex = -1;

    while (start <= csvText.size())
    {
        const size_t lineEnd = csvText.find('\n', start);
        const std::string line = lineEnd == std::string::npos
                                     ? csvText.substr(start)
                                     : csvText.substr(start, lineEnd - start);
        start = lineEnd == std::string::npos ? csvText.size() + 1 : lineEnd + 1;

        if (line.empty())
        {
            continue;
        }

        if (!headerRead)
        {
            bundleViewerHeaders = splitSemicolonLine(line);
            for (size_t i = 0; i < bundleViewerHeaders.size(); ++i)
            {
                if (bundleViewerHeaders[i] == "SignalName")
                {
                    signalIndex = static_cast<int>(i);
                }
                else if (bundleViewerHeaders[i] == "Value")
                {
                    valueIndex = static_cast<int>(i);
                }
            }
            headerRead = true;
            continue;
        }

        BundleCsvRow row;
        row.cells = splitSemicolonLine(line);
        while (row.cells.size() < bundleViewerHeaders.size())
        {
            row.cells.push_back("");
        }

        if (signalIndex >= 0 && static_cast<size_t>(signalIndex) < row.cells.size())
        {
            row.signalName = row.cells[signalIndex];
        }
        if (valueIndex >= 0 && static_cast<size_t>(valueIndex) < row.cells.size())
        {
            row.value = row.cells[valueIndex];
        }

        double numericValue = 0.0;
        row.numeric = parseNumericValue(row.value, numericValue);
        row.numericValue = numericValue;

        if (row.numeric &&
            !row.signalName.empty() &&
            std::find(bundleViewerSignals.begin(), bundleViewerSignals.end(), row.signalName) == bundleViewerSignals.end())
        {
            bundleViewerSignals.push_back(row.signalName);
        }

        bundleViewerRows.push_back(row);
    }

    ensureBundleViewerSelectedSignals();
}

void DataBundleSelectionGui::createBundleViewerContent()
{
    ui_BundleViewerScalingLabel = lv_label_create(ui_BundleViewerPanel);
    lv_label_set_text_fmt(ui_BundleViewerScalingLabel, "Samples %d", bundleViewerVisibleSampleCount);
    lv_obj_set_width(ui_BundleViewerScalingLabel, 90);
    lv_label_set_long_mode(ui_BundleViewerScalingLabel, LV_LABEL_LONG_DOT);
    lv_obj_align(ui_BundleViewerScalingLabel, LV_ALIGN_TOP_LEFT, 526, 56);
    lv_obj_set_style_text_font(ui_BundleViewerScalingLabel, &lv_font_montserrat_10, LV_PART_MAIN);
    lv_obj_set_style_text_color(ui_BundleViewerScalingLabel, lv_color_hex(0x5F6B7A), LV_PART_MAIN);

    ui_BundleViewerPrimaryScaleLabel = lv_label_create(ui_BundleViewerPanel);
    lv_label_set_text(ui_BundleViewerPrimaryScaleLabel, "Scale: x1");
    lv_obj_set_width(ui_BundleViewerPrimaryScaleLabel, 100);
    lv_label_set_long_mode(ui_BundleViewerPrimaryScaleLabel, LV_LABEL_LONG_DOT);
    lv_obj_align(ui_BundleViewerPrimaryScaleLabel, LV_ALIGN_TOP_LEFT, 626, 56);
    lv_obj_set_style_text_font(ui_BundleViewerPrimaryScaleLabel, &lv_font_montserrat_10, LV_PART_MAIN);
    lv_obj_set_style_text_color(ui_BundleViewerPrimaryScaleLabel, lv_color_hex(getBundleViewerColor(bundleViewerPrimaryColorIndex)), LV_PART_MAIN);

    ui_BundleViewerSecondaryScaleLabel = lv_label_create(ui_BundleViewerPanel);
    lv_label_set_text(ui_BundleViewerSecondaryScaleLabel, "Scale: x1");
    lv_obj_set_width(ui_BundleViewerSecondaryScaleLabel, 100);
    lv_label_set_long_mode(ui_BundleViewerSecondaryScaleLabel, LV_LABEL_LONG_DOT);
    lv_obj_align(ui_BundleViewerSecondaryScaleLabel, LV_ALIGN_TOP_LEFT, 626, 70);
    lv_obj_set_style_text_font(ui_BundleViewerSecondaryScaleLabel, &lv_font_montserrat_10, LV_PART_MAIN);
    lv_obj_set_style_text_color(ui_BundleViewerSecondaryScaleLabel, lv_color_hex(getBundleViewerColor(bundleViewerSecondaryColorIndex)), LV_PART_MAIN);
    lv_obj_add_flag(ui_BundleViewerSecondaryScaleLabel, LV_OBJ_FLAG_HIDDEN);

    ui_BundleViewerPrimaryLegend = lv_obj_create(ui_BundleViewerPanel);
    lv_obj_remove_style_all(ui_BundleViewerPrimaryLegend);
    lv_obj_set_size(ui_BundleViewerPrimaryLegend, 128, 26);
    lv_obj_align(ui_BundleViewerPrimaryLegend, LV_ALIGN_TOP_LEFT, 250, 50);
    lv_obj_set_style_radius(ui_BundleViewerPrimaryLegend, 5, LV_PART_MAIN);
    lv_obj_set_style_bg_color(ui_BundleViewerPrimaryLegend, lv_color_hex(getBundleViewerColor(bundleViewerPrimaryColorIndex)), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(ui_BundleViewerPrimaryLegend, 255, LV_PART_MAIN);
    lv_obj_add_flag(ui_BundleViewerPrimaryLegend, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(ui_BundleViewerPrimaryLegend, [](lv_event_t *e)
                        {
        auto self = static_cast<DataBundleSelectionGui*>(lv_event_get_user_data(e));
        self->cycleBundleViewerSignalSlot(0); }, LV_EVENT_CLICKED, this);

    ui_BundleViewerPrimaryLegendLabel = lv_label_create(ui_BundleViewerPrimaryLegend);
    lv_obj_set_width(ui_BundleViewerPrimaryLegendLabel, 116);
    lv_label_set_long_mode(ui_BundleViewerPrimaryLegendLabel, LV_LABEL_LONG_DOT);
    lv_obj_center(ui_BundleViewerPrimaryLegendLabel);
    lv_obj_set_style_text_font(ui_BundleViewerPrimaryLegendLabel, &lv_font_montserrat_12, LV_PART_MAIN);
    lv_obj_set_style_text_color(ui_BundleViewerPrimaryLegendLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN);

    ui_BundleViewerSecondaryLegend = lv_obj_create(ui_BundleViewerPanel);
    lv_obj_remove_style_all(ui_BundleViewerSecondaryLegend);
    lv_obj_set_size(ui_BundleViewerSecondaryLegend, 128, 26);
    lv_obj_align(ui_BundleViewerSecondaryLegend, LV_ALIGN_TOP_LEFT, 386, 50);
    lv_obj_set_style_radius(ui_BundleViewerSecondaryLegend, 5, LV_PART_MAIN);
    lv_obj_set_style_bg_color(ui_BundleViewerSecondaryLegend, lv_color_hex(getBundleViewerColor(bundleViewerSecondaryColorIndex)), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(ui_BundleViewerSecondaryLegend, 255, LV_PART_MAIN);
    lv_obj_add_flag(ui_BundleViewerSecondaryLegend, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(ui_BundleViewerSecondaryLegend, [](lv_event_t *e)
                        {
        auto self = static_cast<DataBundleSelectionGui*>(lv_event_get_user_data(e));
        self->cycleBundleViewerSignalSlot(1); }, LV_EVENT_CLICKED, this);

    ui_BundleViewerSecondaryLegendLabel = lv_label_create(ui_BundleViewerSecondaryLegend);
    lv_obj_set_width(ui_BundleViewerSecondaryLegendLabel, 116);
    lv_label_set_long_mode(ui_BundleViewerSecondaryLegendLabel, LV_LABEL_LONG_DOT);
    lv_obj_center(ui_BundleViewerSecondaryLegendLabel);
    lv_obj_set_style_text_font(ui_BundleViewerSecondaryLegendLabel, &lv_font_montserrat_12, LV_PART_MAIN);
    lv_obj_set_style_text_color(ui_BundleViewerSecondaryLegendLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN);

    ui_BundleViewerChart = lv_chart_create(ui_BundleViewerPanel);
    lv_obj_set_size(ui_BundleViewerChart, 610, 300);
    lv_obj_align(ui_BundleViewerChart, LV_ALIGN_TOP_LEFT, 72, 92);
    lv_obj_add_flag(ui_BundleViewerChart, LV_OBJ_FLAG_CLICKABLE);
    lv_chart_set_type(ui_BundleViewerChart, LV_CHART_TYPE_LINE);
    lv_chart_set_update_mode(ui_BundleViewerChart, LV_CHART_UPDATE_MODE_CIRCULAR);
    lv_chart_set_point_count(ui_BundleViewerChart, static_cast<uint16_t>(bundleViewerVisibleSampleCount));
    lv_chart_set_div_line_count(ui_BundleViewerChart, 5, 10);
    updateBundleViewerXAxisTicks();
    lv_chart_set_axis_tick(ui_BundleViewerChart, LV_CHART_AXIS_PRIMARY_Y, 8, 4, 5, 2, true, 42);
    lv_chart_set_axis_tick(ui_BundleViewerChart, LV_CHART_AXIS_SECONDARY_Y, 8, 4, 0, 2, false, 42);
    lv_chart_set_range(ui_BundleViewerChart, LV_CHART_AXIS_PRIMARY_Y, BUNDLE_VIEW_PLOT_MIN, BUNDLE_VIEW_PLOT_MAX);
    lv_chart_set_range(ui_BundleViewerChart, LV_CHART_AXIS_SECONDARY_Y, BUNDLE_VIEW_PLOT_MIN, BUNDLE_VIEW_PLOT_MAX);
    lv_obj_set_style_bg_opa(ui_BundleViewerChart, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(ui_BundleViewerChart, 2, LV_PART_MAIN);
    lv_obj_set_style_border_color(ui_BundleViewerChart, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_line_color(ui_BundleViewerChart, lv_color_hex(0x000000), LV_PART_TICKS);
    lv_obj_set_style_text_color(ui_BundleViewerChart, lv_color_hex(0x000000), LV_PART_TICKS);
    lv_obj_add_event_cb(ui_BundleViewerChart, [](lv_event_t *e)
                        {
        auto self = static_cast<DataBundleSelectionGui*>(lv_event_get_user_data(e));
        self->handleBundleViewerChartDrag(e); }, LV_EVENT_ALL, this);
    lv_obj_add_event_cb(ui_BundleViewerChart, handleBundleViewerChartDrawPart, LV_EVENT_DRAW_PART_BEGIN, this);

    ui_BundleViewerPrimarySeries = lv_chart_add_series(ui_BundleViewerChart, lv_color_hex(getBundleViewerColor(bundleViewerPrimaryColorIndex)), LV_CHART_AXIS_PRIMARY_Y);
    ui_BundleViewerSecondarySeries = lv_chart_add_series(ui_BundleViewerChart, lv_color_hex(getBundleViewerColor(bundleViewerSecondaryColorIndex)), LV_CHART_AXIS_SECONDARY_Y);

    ui_BundleViewerCursorLabel = lv_label_create(ui_BundleViewerPanel);
    lv_obj_set_width(ui_BundleViewerCursorLabel, 230);
    lv_label_set_long_mode(ui_BundleViewerCursorLabel, LV_LABEL_LONG_DOT);
    lv_obj_align(ui_BundleViewerCursorLabel, LV_ALIGN_TOP_LEFT, 526, 78);
    lv_obj_set_style_text_font(ui_BundleViewerCursorLabel, &lv_font_montserrat_10, LV_PART_MAIN);
    lv_obj_set_style_text_color(ui_BundleViewerCursorLabel, lv_color_hex(0xD32F2F), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(ui_BundleViewerCursorLabel, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(ui_BundleViewerCursorLabel, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(ui_BundleViewerCursorLabel, 0, LV_PART_MAIN);
    lv_obj_add_flag(ui_BundleViewerCursorLabel, LV_OBJ_FLAG_HIDDEN);

    ui_BundleViewerCursorValueLabel = lv_label_create(ui_BundleViewerPanel);
    lv_obj_set_width(ui_BundleViewerCursorValueLabel, 220);
    lv_label_set_long_mode(ui_BundleViewerCursorValueLabel, LV_LABEL_LONG_DOT);
    lv_obj_set_style_text_font(ui_BundleViewerCursorValueLabel, &lv_font_montserrat_12, LV_PART_MAIN);
    lv_obj_set_style_text_color(ui_BundleViewerCursorValueLabel, lv_color_hex(getBundleViewerColor(bundleViewerPrimaryColorIndex)), LV_PART_MAIN);
    lv_obj_set_style_bg_color(ui_BundleViewerCursorValueLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(ui_BundleViewerCursorValueLabel, 240, LV_PART_MAIN);
    lv_obj_set_style_border_width(ui_BundleViewerCursorValueLabel, 1, LV_PART_MAIN);
    lv_obj_set_style_border_color(ui_BundleViewerCursorValueLabel, lv_color_hex(getBundleViewerColor(bundleViewerPrimaryColorIndex)), LV_PART_MAIN);
    lv_obj_set_style_radius(ui_BundleViewerCursorValueLabel, 4, LV_PART_MAIN);
    lv_obj_set_style_pad_left(ui_BundleViewerCursorValueLabel, 6, LV_PART_MAIN);
    lv_obj_set_style_pad_right(ui_BundleViewerCursorValueLabel, 6, LV_PART_MAIN);
    lv_obj_set_style_pad_top(ui_BundleViewerCursorValueLabel, 3, LV_PART_MAIN);
    lv_obj_set_style_pad_bottom(ui_BundleViewerCursorValueLabel, 3, LV_PART_MAIN);
    lv_obj_add_flag(ui_BundleViewerCursorValueLabel, LV_OBJ_FLAG_HIDDEN);

    ui_BundleViewerCursorXLine = lv_line_create(ui_BundleViewerPanel);
    lv_obj_set_size(ui_BundleViewerCursorXLine, 780, 460);
    lv_obj_set_pos(ui_BundleViewerCursorXLine, 0, 0);
    lv_obj_set_style_line_color(ui_BundleViewerCursorXLine, lv_color_hex(0xD32F2F), LV_PART_MAIN);
    lv_obj_set_style_line_width(ui_BundleViewerCursorXLine, 2, LV_PART_MAIN);
    lv_obj_set_style_line_dash_width(ui_BundleViewerCursorXLine, 6, LV_PART_MAIN);
    lv_obj_set_style_line_dash_gap(ui_BundleViewerCursorXLine, 4, LV_PART_MAIN);
    lv_obj_add_flag(ui_BundleViewerCursorXLine, LV_OBJ_FLAG_HIDDEN);

    ui_BundleViewerCursorYLine = lv_line_create(ui_BundleViewerPanel);
    lv_obj_set_size(ui_BundleViewerCursorYLine, 780, 460);
    lv_obj_set_pos(ui_BundleViewerCursorYLine, 0, 0);
    lv_obj_set_style_line_color(ui_BundleViewerCursorYLine, lv_color_hex(0xD32F2F), LV_PART_MAIN);
    lv_obj_set_style_line_width(ui_BundleViewerCursorYLine, 2, LV_PART_MAIN);
    lv_obj_set_style_line_dash_width(ui_BundleViewerCursorYLine, 6, LV_PART_MAIN);
    lv_obj_set_style_line_dash_gap(ui_BundleViewerCursorYLine, 4, LV_PART_MAIN);
    lv_obj_add_flag(ui_BundleViewerCursorYLine, LV_OBJ_FLAG_HIDDEN);

    ui_BundleViewerTable = lv_table_create(ui_BundleViewerPanel);
    lv_obj_set_size(ui_BundleViewerTable, 740, 335);
    lv_obj_align(ui_BundleViewerTable, LV_ALIGN_BOTTOM_MID, 0, -18);
    lv_obj_set_scroll_dir(ui_BundleViewerTable, static_cast<lv_dir_t>(LV_DIR_VER | LV_DIR_HOR));
    lv_obj_set_style_text_font(ui_BundleViewerTable, &lv_font_montserrat_10, LV_PART_ITEMS);
    lv_obj_set_style_border_color(ui_BundleViewerTable, lv_color_hex(0xC7D2E0), LV_PART_MAIN);
    lv_obj_set_style_border_width(ui_BundleViewerTable, 1, LV_PART_MAIN);
}

void DataBundleSelectionGui::setBundleViewerMode(bool csvMode)
{
    bundleViewerCsvMode = csvMode;

    const lv_color_t activeColor = lv_color_hex(0x009BFF);
    const lv_color_t inactiveColor = lv_color_hex(0xA8B3BF);
    if (ui_BundleViewerGraphTab)
    {
        lv_obj_set_style_bg_color(ui_BundleViewerGraphTab, csvMode ? inactiveColor : activeColor, LV_PART_MAIN);
    }
    if (ui_BundleViewerCsvTab)
    {
        lv_obj_set_style_bg_color(ui_BundleViewerCsvTab, csvMode ? activeColor : inactiveColor, LV_PART_MAIN);
    }

    lv_obj_t *graphObjects[] = {
        ui_BundleViewerChart,
        ui_BundleViewerScalingLabel,
        ui_BundleViewerPrimaryScaleLabel,
        ui_BundleViewerSecondaryScaleLabel,
        ui_BundleViewerCursorLabel,
        ui_BundleViewerCursorValueLabel,
        ui_BundleViewerPrimaryLegend,
        ui_BundleViewerSecondaryLegend,
        ui_BundleViewerCursorXLine,
        ui_BundleViewerCursorYLine
    };
    for (lv_obj_t *object : graphObjects)
    {
        if (!object) {
            continue;
        }
        if (csvMode) {
            lv_obj_add_flag(object, LV_OBJ_FLAG_HIDDEN);
        } else {
            lv_obj_clear_flag(object, LV_OBJ_FLAG_HIDDEN);
        }
    }

    if (ui_BundleViewerTable)
    {
        if (csvMode) {
            lv_obj_clear_flag(ui_BundleViewerTable, LV_OBJ_FLAG_HIDDEN);
            hideBundleViewerCursor();
            updateBundleViewerTable();
        } else {
            lv_obj_add_flag(ui_BundleViewerTable, LV_OBJ_FLAG_HIDDEN);
            updateBundleViewerGraph();
        }
    }

    lv_obj_move_foreground(ui_BundleViewerGraphTab);
    lv_obj_move_foreground(ui_BundleViewerCsvTab);
    lv_obj_move_foreground(ui_BundleViewerSettingsButton);
}

void DataBundleSelectionGui::ensureBundleViewerSelectedSignals()
{
    std::vector<std::string> filtered;
    for (const auto &signal : bundleViewerSelectedSignals)
    {
        if (std::find(bundleViewerSignals.begin(), bundleViewerSignals.end(), signal) != bundleViewerSignals.end() &&
            std::find(filtered.begin(), filtered.end(), signal) == filtered.end())
        {
            filtered.push_back(signal);
            if (filtered.size() == 2)
            {
                break;
            }
        }
    }

    if (filtered.empty() && !bundleViewerSignals.empty())
    {
        filtered.push_back(bundleViewerSignals[0]);
        if (bundleViewerSignals.size() > 1)
        {
            filtered.push_back(bundleViewerSignals[1]);
        }
    }

    bundleViewerSelectedSignals = filtered;
}

std::vector<std::string> DataBundleSelectionGui::getBundleViewerActiveSignals()
{
    ensureBundleViewerSelectedSignals();
    return bundleViewerSelectedSignals;
}

void DataBundleSelectionGui::cycleBundleViewerSignalSlot(size_t slot)
{
    ensureBundleViewerSelectedSignals();
    if (bundleViewerSignals.empty() || slot > 1)
    {
        return;
    }

    if (slot == 0)
    {
        const std::string current = bundleViewerSelectedSignals.empty() ? "" : bundleViewerSelectedSignals[0];
        auto it = std::find(bundleViewerSignals.begin(), bundleViewerSignals.end(), current);
        size_t startIndex = it == bundleViewerSignals.end() ? 0 : (static_cast<size_t>(it - bundleViewerSignals.begin()) + 1) % bundleViewerSignals.size();
        for (size_t i = 0; i < bundleViewerSignals.size(); ++i)
        {
            const std::string &candidate = bundleViewerSignals[(startIndex + i) % bundleViewerSignals.size()];
            if (bundleViewerSelectedSignals.size() < 2 || candidate != bundleViewerSelectedSignals[1])
            {
                if (bundleViewerSelectedSignals.empty()) bundleViewerSelectedSignals.push_back(candidate);
                else bundleViewerSelectedSignals[0] = candidate;
                break;
            }
        }
    }
    else
    {
        const std::string primary = bundleViewerSelectedSignals.empty() ? "" : bundleViewerSelectedSignals[0];
        const std::string current = bundleViewerSelectedSignals.size() > 1 ? bundleViewerSelectedSignals[1] : "";
        std::vector<std::string> candidates;
        candidates.push_back("");
        for (const auto &signal : bundleViewerSignals)
        {
            if (signal != primary)
            {
                candidates.push_back(signal);
            }
        }

        auto it = std::find(candidates.begin(), candidates.end(), current);
        const size_t nextIndex = it == candidates.end() ? 0 : (static_cast<size_t>(it - candidates.begin()) + 1) % candidates.size();
        if (candidates[nextIndex].empty())
        {
            if (bundleViewerSelectedSignals.size() > 1)
            {
                bundleViewerSelectedSignals.erase(bundleViewerSelectedSignals.begin() + 1);
            }
        }
        else if (bundleViewerSelectedSignals.size() > 1)
        {
            bundleViewerSelectedSignals[1] = candidates[nextIndex];
        }
        else
        {
            bundleViewerSelectedSignals.push_back(candidates[nextIndex]);
        }
    }

    bundleViewerHistoryOffset = 0;
    bundleViewerCursorSignalSlot = 0;
    bundleViewerCursorVisible = false;
    updateBundleViewerGraph();
}

void DataBundleSelectionGui::updateBundleViewerXAxisTicks()
{
    if (!ui_BundleViewerChart)
    {
        return;
    }

    bundleViewerXTickMax = bundleViewerVisibleSampleCount <= 20 ? bundleViewerVisibleSampleCount - 1 : 10;
    if (bundleViewerXTickMax < 1)
    {
        bundleViewerXTickMax = 1;
    }
    lv_chart_set_div_line_count(ui_BundleViewerChart, 5, bundleViewerXTickMax + 1);
    lv_chart_set_axis_tick(ui_BundleViewerChart, LV_CHART_AXIS_PRIMARY_X, 10, 0, bundleViewerXTickMax + 1, 1, true, 40);
}

void DataBundleSelectionGui::adjustBundleViewerVisibleSamples(int deltaSamples)
{
    if (deltaSamples == 0 || !ui_BundleViewerChart)
    {
        return;
    }

    int nextCount = bundleViewerVisibleSampleCount + deltaSamples;
    if (nextCount < BUNDLE_VIEW_MIN_CHART_POINTS) nextCount = BUNDLE_VIEW_MIN_CHART_POINTS;
    if (nextCount > BUNDLE_VIEW_MAX_CHART_POINTS) nextCount = BUNDLE_VIEW_MAX_CHART_POINTS;
    nextCount = (nextCount / BUNDLE_VIEW_SAMPLE_STEP) * BUNDLE_VIEW_SAMPLE_STEP;
    if (nextCount < BUNDLE_VIEW_MIN_CHART_POINTS) nextCount = BUNDLE_VIEW_MIN_CHART_POINTS;
    if (nextCount == bundleViewerVisibleSampleCount)
    {
        return;
    }

    bundleViewerVisibleSampleCount = nextCount;
    if (bundleViewerCursorIndex >= bundleViewerVisibleSampleCount)
    {
        bundleViewerCursorIndex = bundleViewerVisibleSampleCount - 1;
    }
    lv_chart_set_point_count(ui_BundleViewerChart, static_cast<uint16_t>(bundleViewerVisibleSampleCount));
    updateBundleViewerXAxisTicks();
    bundleViewerDragAccumulatorPx = 0;
    updateBundleViewerGraph();
}

bool DataBundleSelectionGui::handleBundleViewerPinchGesture()
{
    if (&touch_point_count == nullptr || &touch_last_x == nullptr || &touch_last_y == nullptr ||
        &touch_second_x == nullptr || &touch_second_y == nullptr || touch_point_count < 2)
    {
        bundleViewerPinchActive = false;
        bundleViewerPinchLastDistancePx = 0;
        bundleViewerPinchAccumulatorPx = 0;
        return false;
    }

    const int dx = touch_last_x - touch_second_x;
    const int dy = touch_last_y - touch_second_y;
    const int distancePx = static_cast<int>(std::sqrt(static_cast<double>(dx * dx + dy * dy)));
    if (!bundleViewerPinchActive)
    {
        bundleViewerPinchActive = true;
        bundleViewerPinchLastDistancePx = distancePx;
        bundleViewerPinchAccumulatorPx = 0;
        bundleViewerDragAccumulatorPx = 0;
        return true;
    }

    bundleViewerPinchAccumulatorPx += bundleViewerPinchLastDistancePx - distancePx;
    bundleViewerPinchLastDistancePx = distancePx;

    const int pixelsPerStep = 18;
    int sampleSteps = 0;
    while (bundleViewerPinchAccumulatorPx >= pixelsPerStep)
    {
        ++sampleSteps;
        bundleViewerPinchAccumulatorPx -= pixelsPerStep;
    }
    while (bundleViewerPinchAccumulatorPx <= -pixelsPerStep)
    {
        --sampleSteps;
        bundleViewerPinchAccumulatorPx += pixelsPerStep;
    }

    if (sampleSteps != 0)
    {
        adjustBundleViewerVisibleSamples(sampleSteps * BUNDLE_VIEW_SAMPLE_STEP);
    }

    return true;
}

std::pair<double, double> DataBundleSelectionGui::computeBundleViewerRange(const std::vector<double> &values,
                                                                           int start,
                                                                           int pointCount)
{
    bool hasRange = false;
    double minValue = 0.0;
    double maxValue = 0.0;
    for (int i = 0; i < pointCount; ++i)
    {
        const int sourceIndex = start + i;
        if (sourceIndex < 0 || sourceIndex >= static_cast<int>(values.size()))
        {
            continue;
        }

        const double value = values[sourceIndex];
        if (!hasRange)
        {
            minValue = value;
            maxValue = value;
            hasRange = true;
        }
        else
        {
            if (value < minValue) minValue = value;
            if (value > maxValue) maxValue = value;
        }
    }

    if (!hasRange)
    {
        return std::pair<double, double>(-1.0, 1.0);
    }

    if (minValue == maxValue)
    {
        const double absValue = std::fabs(minValue);
        const double delta = absValue == 0.0 ? 1.0 : std::max(absValue * 0.1, 1e-9);
        minValue -= delta;
        maxValue += delta;
    }

    const double span = maxValue - minValue;
    const double magnitude = std::max(std::fabs(minValue), std::fabs(maxValue));
    const double minPad = magnitude == 0.0 ? 1e-9 : std::max(magnitude * 0.01, 1e-9);
    const double pad = std::max(std::fabs(span / 10.0), minPad);
    return std::pair<double, double>(minValue - pad, maxValue + pad);
}

lv_coord_t DataBundleSelectionGui::mapBundleViewerValueToPlot(double value, double minValue, double maxValue)
{
    const double range = maxValue - minValue;
    if (range == 0.0)
    {
        return (BUNDLE_VIEW_PLOT_MAX - BUNDLE_VIEW_PLOT_MIN) / 2;
    }

    double normalized = (value - minValue) / range;
    if (normalized < 0.0) normalized = 0.0;
    if (normalized > 1.0) normalized = 1.0;
    return static_cast<lv_coord_t>(std::lround(BUNDLE_VIEW_PLOT_MIN + normalized * (BUNDLE_VIEW_PLOT_MAX - BUNDLE_VIEW_PLOT_MIN)));
}

int DataBundleSelectionGui::axisScaleExponent(double minValue, double maxValue)
{
    const double maxAbs = std::max(std::fabs(minValue), std::fabs(maxValue));
    if (maxAbs == 0.0 || (maxAbs >= 0.001 && maxAbs <= 9999.0))
    {
        return 0;
    }

    int exponent = static_cast<int>(std::floor(std::log10(maxAbs) / 3.0)) * 3;
    if (exponent > 9) exponent = 9;
    if (exponent < -9) exponent = -9;
    return exponent;
}

double DataBundleSelectionGui::axisScaleDivisor(int exponent)
{
    return exponent == 0 ? 1.0 : std::pow(10.0, static_cast<double>(exponent));
}

void DataBundleSelectionGui::formatBundleViewerAxisLabel(char *buffer, size_t bufferSize, double value, double span, int exponent)
{
    if (!buffer || bufferSize == 0)
    {
        return;
    }

    const double displayValue = value / axisScaleDivisor(exponent);
    const double absSpan = std::fabs(span / axisScaleDivisor(exponent));
    if (absSpan < 1.0)
    {
        std::snprintf(buffer, bufferSize, "%.3f", displayValue);
    }
    else if (absSpan < 20.0)
    {
        std::snprintf(buffer, bufferSize, "%.2f", displayValue);
    }
    else if (absSpan < 200.0)
    {
        std::snprintf(buffer, bufferSize, "%.1f", displayValue);
    }
    else
    {
        std::snprintf(buffer, bufferSize, "%.0f", displayValue);
    }

    char *dot = std::strchr(buffer, '.');
    if (dot)
    {
        char *end = buffer + std::strlen(buffer) - 1;
        while (end > dot && *end == '0')
        {
            *end-- = '\0';
        }
        if (end == dot)
        {
            *end = '\0';
        }
    }
}

void DataBundleSelectionGui::applyBundleViewerTickLabelFont(lv_obj_draw_part_dsc_t *dsc)
{
    if (!dsc || !dsc->label_dsc || !dsc->text)
    {
        return;
    }

    const size_t length = std::strlen(dsc->text);
    dsc->label_dsc->font = length > 5 ? &lv_font_montserrat_10 : &lv_font_montserrat_12;
}

void DataBundleSelectionGui::updateBundleViewerScaleLabel()
{
    if (ui_BundleViewerScalingLabel)
    {
        lv_label_set_text_fmt(ui_BundleViewerScalingLabel, "Samples %d", bundleViewerVisibleSampleCount);
    }

    const int primaryExponent = axisScaleExponent(bundleViewerPrimaryRangeMin, bundleViewerPrimaryRangeMax);
    const int secondaryExponent = bundleViewerHasSecondaryRange
                                      ? axisScaleExponent(bundleViewerSecondaryRangeMin, bundleViewerSecondaryRangeMax)
                                      : 0;

    if (ui_BundleViewerPrimaryScaleLabel)
    {
        if (primaryExponent == 0)
        {
            lv_label_set_text(ui_BundleViewerPrimaryScaleLabel, "Scale: x1");
        }
        else
        {
            lv_label_set_text_fmt(ui_BundleViewerPrimaryScaleLabel, "Scale: x1e%d", primaryExponent);
        }
        lv_obj_set_style_text_color(ui_BundleViewerPrimaryScaleLabel, lv_color_hex(getBundleViewerColor(bundleViewerPrimaryColorIndex)), LV_PART_MAIN);
    }

    if (ui_BundleViewerSecondaryScaleLabel)
    {
        if (bundleViewerHasSecondaryRange)
        {
            if (secondaryExponent == 0)
            {
                lv_label_set_text(ui_BundleViewerSecondaryScaleLabel, "Scale: x1");
            }
            else
            {
                lv_label_set_text_fmt(ui_BundleViewerSecondaryScaleLabel, "Scale: x1e%d", secondaryExponent);
            }
            lv_obj_set_style_text_color(ui_BundleViewerSecondaryScaleLabel, lv_color_hex(getBundleViewerColor(bundleViewerSecondaryColorIndex)), LV_PART_MAIN);
            lv_obj_clear_flag(ui_BundleViewerSecondaryScaleLabel, LV_OBJ_FLAG_HIDDEN);
        }
        else
        {
            lv_label_set_text(ui_BundleViewerSecondaryScaleLabel, "");
            lv_obj_add_flag(ui_BundleViewerSecondaryScaleLabel, LV_OBJ_FLAG_HIDDEN);
        }
    }
}

void DataBundleSelectionGui::handleBundleViewerChartDrawPart(lv_event_t *e)
{
    auto *self = static_cast<DataBundleSelectionGui *>(lv_event_get_user_data(e));
    lv_obj_draw_part_dsc_t *dsc = lv_event_get_draw_part_dsc(e);
    if (!self || !dsc || dsc->part != LV_PART_TICKS || dsc->text == nullptr)
    {
        return;
    }

    if (dsc->id == LV_CHART_AXIS_PRIMARY_X)
    {
        const int sampleIndex = self->bundleViewerXTickMax <= 0
                                    ? 0
                                    : static_cast<int>(std::lround((static_cast<double>(dsc->value) / self->bundleViewerXTickMax) * (self->bundleViewerVisibleSampleCount - 1)));
        std::snprintf(dsc->text, dsc->text_length, "%d", sampleIndex);
        applyBundleViewerTickLabelFont(dsc);
        return;
    }

    if (dsc->id != LV_CHART_AXIS_PRIMARY_Y && dsc->id != LV_CHART_AXIS_SECONDARY_Y)
    {
        return;
    }

    const bool secondary = dsc->id == LV_CHART_AXIS_SECONDARY_Y;
    if (secondary && !self->bundleViewerHasSecondaryRange)
    {
        return;
    }

    const double minValue = secondary ? self->bundleViewerSecondaryRangeMin : self->bundleViewerPrimaryRangeMin;
    const double maxValue = secondary ? self->bundleViewerSecondaryRangeMax : self->bundleViewerPrimaryRangeMax;
    const double span = maxValue - minValue;
    const double normalized = static_cast<double>(dsc->value - BUNDLE_VIEW_PLOT_MIN) / static_cast<double>(BUNDLE_VIEW_PLOT_MAX - BUNDLE_VIEW_PLOT_MIN);
    const double rawValue = minValue + normalized * span;
    const int exponent = axisScaleExponent(minValue, maxValue);
    formatBundleViewerAxisLabel(dsc->text, dsc->text_length, rawValue, span, exponent);
    applyBundleViewerTickLabelFont(dsc);
}

void DataBundleSelectionGui::updateBundleViewerGraph()
{
    if (!ui_BundleViewerChart || !ui_BundleViewerPrimarySeries || !ui_BundleViewerSecondarySeries)
    {
        return;
    }

    std::vector<double> primaryValues;
    std::vector<double> secondaryValues;
    std::vector<std::string> primaryRawValues;
    std::vector<std::string> secondaryRawValues;

    const auto activeSignals = getBundleViewerActiveSignals();
    const std::string primarySignal = activeSignals.empty() ? "" : activeSignals[0];
    const std::string secondarySignal = activeSignals.size() > 1 ? activeSignals[1] : "";

    for (const auto &row : bundleViewerRows)
    {
        if (!row.numeric)
        {
            continue;
        }

        if (row.signalName == primarySignal)
        {
            primaryValues.push_back(row.numericValue);
            primaryRawValues.push_back(row.value);
        }
        else if (!secondarySignal.empty() && row.signalName == secondarySignal)
        {
            secondaryValues.push_back(row.numericValue);
            secondaryRawValues.push_back(row.value);
        }
    }

    lv_chart_set_series_color(ui_BundleViewerChart, ui_BundleViewerPrimarySeries, lv_color_hex(getBundleViewerColor(bundleViewerPrimaryColorIndex)));
    lv_chart_set_series_color(ui_BundleViewerChart, ui_BundleViewerSecondarySeries, lv_color_hex(getBundleViewerColor(bundleViewerSecondaryColorIndex)));
    lv_chart_set_point_count(ui_BundleViewerChart, static_cast<uint16_t>(bundleViewerVisibleSampleCount));
    lv_chart_set_x_start_point(ui_BundleViewerChart, ui_BundleViewerPrimarySeries, 0);
    lv_chart_set_x_start_point(ui_BundleViewerChart, ui_BundleViewerSecondarySeries, 0);
    updateBundleViewerXAxisTicks();
    lv_chart_set_all_value(ui_BundleViewerChart, ui_BundleViewerPrimarySeries, LV_CHART_POINT_NONE);
    lv_chart_set_all_value(ui_BundleViewerChart, ui_BundleViewerSecondarySeries, LV_CHART_POINT_NONE);

    const int maxCount = static_cast<int>(std::max(primaryValues.size(), secondaryValues.size()));
    const int maxOffset = maxCount > bundleViewerVisibleSampleCount ? maxCount - bundleViewerVisibleSampleCount : 0;
    if (bundleViewerHistoryOffset > maxOffset)
    {
        bundleViewerHistoryOffset = maxOffset;
    }

    const int start = maxCount > bundleViewerVisibleSampleCount
                          ? maxCount - bundleViewerVisibleSampleCount - bundleViewerHistoryOffset
                          : 0;

    const auto primaryRange = computeBundleViewerRange(primaryValues, start, bundleViewerVisibleSampleCount);
    const bool hasSecondary = !secondaryValues.empty();
    const auto secondaryRange = hasSecondary
                                    ? computeBundleViewerRange(secondaryValues, start, bundleViewerVisibleSampleCount)
                                    : primaryRange;
    bundleViewerPrimaryRangeMin = primaryRange.first;
    bundleViewerPrimaryRangeMax = primaryRange.second;
    bundleViewerSecondaryRangeMin = secondaryRange.first;
    bundleViewerSecondaryRangeMax = secondaryRange.second;
    bundleViewerHasSecondaryRange = hasSecondary;

    auto applySeries = [&](lv_chart_series_t *series, const std::vector<double> &values, double minValue, double maxValue)
    {
        for (int i = 0; i < bundleViewerVisibleSampleCount; ++i)
        {
            const int sourceIndex = start + i;
            if (sourceIndex < 0 || sourceIndex >= static_cast<int>(values.size()))
            {
                continue;
            }

            lv_chart_set_value_by_id(ui_BundleViewerChart, series, i, mapBundleViewerValueToPlot(values[sourceIndex], minValue, maxValue));
        }
    };

    applySeries(ui_BundleViewerPrimarySeries, primaryValues, bundleViewerPrimaryRangeMin, bundleViewerPrimaryRangeMax);
    applySeries(ui_BundleViewerSecondarySeries, secondaryValues, bundleViewerSecondaryRangeMin, bundleViewerSecondaryRangeMax);
    lv_chart_set_range(ui_BundleViewerChart, LV_CHART_AXIS_PRIMARY_Y, BUNDLE_VIEW_PLOT_MIN, BUNDLE_VIEW_PLOT_MAX);
    lv_chart_set_range(ui_BundleViewerChart, LV_CHART_AXIS_SECONDARY_Y, BUNDLE_VIEW_PLOT_MIN, BUNDLE_VIEW_PLOT_MAX);
    lv_chart_set_axis_tick(ui_BundleViewerChart, LV_CHART_AXIS_PRIMARY_Y, 8, 4, 5, 2, true, 42);
    lv_chart_set_axis_tick(ui_BundleViewerChart, LV_CHART_AXIS_SECONDARY_Y, 8, 4, hasSecondary ? 5 : 0, 2, hasSecondary, 42);

    if (bundleViewerCursorIndex >= bundleViewerVisibleSampleCount)
    {
        bundleViewerCursorIndex = bundleViewerVisibleSampleCount - 1;
    }

    updateBundleViewerScaleLabel();
    if (ui_BundleViewerPrimaryLegend)
    {
        lv_obj_set_style_bg_color(ui_BundleViewerPrimaryLegend, lv_color_hex(getBundleViewerColor(bundleViewerPrimaryColorIndex)), LV_PART_MAIN);
    }
    if (ui_BundleViewerPrimaryLegendLabel)
    {
        lv_label_set_text(ui_BundleViewerPrimaryLegendLabel, primarySignal.empty() ? "S1" : primarySignal.c_str());
    }
    if (ui_BundleViewerSecondaryLegend)
    {
        lv_obj_set_style_bg_color(ui_BundleViewerSecondaryLegend, lv_color_hex(hasSecondary ? getBundleViewerColor(bundleViewerSecondaryColorIndex) : 0xA8B3BF), LV_PART_MAIN);
    }
    if (ui_BundleViewerSecondaryLegendLabel)
    {
        lv_label_set_text(ui_BundleViewerSecondaryLegendLabel, secondarySignal.empty() ? "Add signal" : secondarySignal.c_str());
    }

    lv_chart_refresh(ui_BundleViewerChart);
    lv_obj_invalidate(ui_BundleViewerChart);

    if (bundleViewerCursorVisible)
    {
        showBundleViewerCursorAtIndex(bundleViewerCursorIndex);
    }
    else
    {
        hideBundleViewerCursor();
    }
}

void DataBundleSelectionGui::updateBundleViewerTable()
{
    if (!ui_BundleViewerTable)
    {
        return;
    }

    const uint16_t columnCount = static_cast<uint16_t>(bundleViewerHeaders.empty() ? 1 : bundleViewerHeaders.size());
    const uint16_t rowCount = static_cast<uint16_t>(bundleViewerRows.size() + 1);
    lv_table_set_col_cnt(ui_BundleViewerTable, columnCount);
    lv_table_set_row_cnt(ui_BundleViewerTable, rowCount);

    for (uint16_t col = 0; col < columnCount; ++col)
    {
        const std::string header = bundleViewerHeaders.empty() ? "Data" : bundleViewerHeaders[col];
        lv_table_set_cell_value(ui_BundleViewerTable, 0, col, header.c_str());
        lv_table_set_col_width(ui_BundleViewerTable, col, col < 2 ? 118 : 92);
    }

    for (uint16_t row = 0; row < bundleViewerRows.size(); ++row)
    {
        for (uint16_t col = 0; col < columnCount; ++col)
        {
            const char *cell = col < bundleViewerRows[row].cells.size() ? bundleViewerRows[row].cells[col].c_str() : "";
            lv_table_set_cell_value(ui_BundleViewerTable, static_cast<uint16_t>(row + 1), col, cell);
        }
    }
}

void DataBundleSelectionGui::panBundleViewerHistory(int steps)
{
    if (steps == 0)
    {
        return;
    }

    int maxCount = 0;
    for (const auto &signal : getBundleViewerActiveSignals())
    {
        int count = 0;
        for (const auto &row : bundleViewerRows)
        {
            if (row.numeric && row.signalName == signal)
            {
                ++count;
            }
        }
        if (count > maxCount)
        {
            maxCount = count;
        }
    }

    const int maxOffset = maxCount > bundleViewerVisibleSampleCount ? maxCount - bundleViewerVisibleSampleCount : 0;
    int nextOffset = bundleViewerHistoryOffset + steps;
    if (nextOffset < 0) nextOffset = 0;
    if (nextOffset > maxOffset) nextOffset = maxOffset;
    if (nextOffset == bundleViewerHistoryOffset)
    {
        return;
    }

    bundleViewerHistoryOffset = nextOffset;
    updateBundleViewerGraph();
}

void DataBundleSelectionGui::handleBundleViewerChartDrag(lv_event_t *e)
{
    const lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_PRESSED || code == LV_EVENT_RELEASED || code == LV_EVENT_PRESS_LOST)
    {
        bundleViewerDragAccumulatorPx = 0;
        bundleViewerPinchLastDistancePx = 0;
        bundleViewerPinchAccumulatorPx = 0;
        bundleViewerPinchActive = false;
        if (code == LV_EVENT_RELEASED || code == LV_EVENT_PRESS_LOST)
        {
            hideBundleViewerCursor();
        }
        return;
    }

    if (code == LV_EVENT_PRESSING && handleBundleViewerPinchGesture())
    {
        return;
    }

    if (code == LV_EVENT_LONG_PRESSED || (code == LV_EVENT_PRESSING && bundleViewerCursorVisible))
    {
        lv_indev_t *indev = lv_indev_get_act();
        if (!indev || !ui_BundleViewerChart)
        {
            return;
        }

        lv_point_t point;
        lv_indev_get_point(indev, &point);

        showBundleViewerCursorNearPoint(point);
        return;
    }

    if (code != LV_EVENT_PRESSING)
    {
        return;
    }

    lv_indev_t *indev = lv_indev_get_act();
    if (!indev)
    {
        return;
    }

    lv_point_t vect;
    lv_indev_get_vect(indev, &vect);
    bundleViewerDragAccumulatorPx += vect.x;

    const int pixelsPerStep = 20;
    int steps = 0;
    while (bundleViewerDragAccumulatorPx >= pixelsPerStep)
    {
        ++steps;
        bundleViewerDragAccumulatorPx -= pixelsPerStep;
    }
    while (bundleViewerDragAccumulatorPx <= -pixelsPerStep)
    {
        --steps;
        bundleViewerDragAccumulatorPx += pixelsPerStep;
    }

    panBundleViewerHistory(steps);
}

void DataBundleSelectionGui::moveBundleViewerCursor(int steps)
{
    int nextIndex = bundleViewerCursorIndex + steps;
    if (nextIndex < 0) nextIndex = 0;
    if (nextIndex >= bundleViewerVisibleSampleCount) nextIndex = bundleViewerVisibleSampleCount - 1;
    if (nextIndex == bundleViewerCursorIndex)
    {
        return;
    }

    bundleViewerCursorIndex = nextIndex;
    bundleViewerCursorVisible = true;
    updateBundleViewerGraph();
}

void DataBundleSelectionGui::showBundleViewerCursorAtIndex(int index)
{
    if (!ui_BundleViewerChart || !ui_BundleViewerCursorXLine || !ui_BundleViewerCursorYLine)
    {
        return;
    }

    if (index < 0) index = 0;
    if (index >= bundleViewerVisibleSampleCount) index = bundleViewerVisibleSampleCount - 1;
    bundleViewerCursorIndex = index;
    bundleViewerCursorVisible = true;

    const auto activeSignals = getBundleViewerActiveSignals();
    const std::string primarySignal = activeSignals.empty() ? "" : activeSignals[0];
    const std::string secondarySignal = activeSignals.size() > 1 ? activeSignals[1] : "";
    std::vector<double> values[2];
    std::vector<std::string> rawValues[2];
    std::string signals[2] = {primarySignal, secondarySignal};

    for (const auto &row : bundleViewerRows)
    {
        if (!row.numeric)
        {
            continue;
        }

        if (row.signalName == primarySignal)
        {
            values[0].push_back(row.numericValue);
            rawValues[0].push_back(row.value);
        }
        else if (!secondarySignal.empty() && row.signalName == secondarySignal)
        {
            values[1].push_back(row.numericValue);
            rawValues[1].push_back(row.value);
        }
    }

    const int maxCount = static_cast<int>(std::max(values[0].size(), values[1].size()));
    const int start = maxCount > bundleViewerVisibleSampleCount
                          ? maxCount - bundleViewerVisibleSampleCount - bundleViewerHistoryOffset
                          : 0;
    const int visibleCount = maxCount - start;
    if (visibleCount > 0 && bundleViewerCursorIndex >= visibleCount)
    {
        bundleViewerCursorIndex = visibleCount - 1;
    }
    const int sourceIndex = start + bundleViewerCursorIndex;

    lv_area_t chartArea;
    lv_area_t panelArea;
    lv_obj_get_content_coords(ui_BundleViewerChart, &chartArea);
    lv_obj_get_content_coords(ui_BundleViewerPanel, &panelArea);

    const lv_coord_t chartLeft = chartArea.x1 - panelArea.x1;
    const lv_coord_t chartRight = chartArea.x2 - panelArea.x1;
    const lv_coord_t chartTop = chartArea.y1 - panelArea.y1;
    const lv_coord_t chartBottom = chartArea.y2 - panelArea.y1;
    const lv_coord_t chartWidth = chartArea.x2 - chartArea.x1;
    const lv_coord_t chartHeight = chartArea.y2 - chartArea.y1;
    int slot = bundleViewerCursorSignalSlot;
    if (slot < 0 || slot > 1 || sourceIndex < 0 || sourceIndex >= static_cast<int>(values[slot].size()))
    {
        slot = sourceIndex >= 0 && sourceIndex < static_cast<int>(values[0].size()) ? 0 : 1;
    }
    if (slot < 0 || slot > 1 || sourceIndex < 0 || sourceIndex >= static_cast<int>(values[slot].size()))
    {
        hideBundleViewerCursor();
        return;
    }
    bundleViewerCursorSignalSlot = slot;

    const double minValue = slot == 0 ? bundleViewerPrimaryRangeMin : bundleViewerSecondaryRangeMin;
    const double maxValue = slot == 0 ? bundleViewerPrimaryRangeMax : bundleViewerSecondaryRangeMax;
    const lv_coord_t plotValue = mapBundleViewerValueToPlot(values[slot][sourceIndex], minValue, maxValue);
    lv_coord_t cursorX = chartLeft;
    if (bundleViewerVisibleSampleCount > 1 && chartWidth > 0)
    {
        cursorX = chartLeft + static_cast<lv_coord_t>(
                                  std::lround((static_cast<double>(bundleViewerCursorIndex) * chartWidth) /
                                              (bundleViewerVisibleSampleCount - 1)));
    }
    lv_coord_t cursorY = chartBottom;
    if (chartHeight > 0)
    {
        const double normalized = static_cast<double>(plotValue - BUNDLE_VIEW_PLOT_MIN) /
                                  static_cast<double>(BUNDLE_VIEW_PLOT_MAX - BUNDLE_VIEW_PLOT_MIN);
        cursorY = chartBottom - static_cast<lv_coord_t>(std::lround(normalized * chartHeight));
    }

    bundleViewerCursorXPoints[0] = {chartLeft, cursorY};
    bundleViewerCursorXPoints[1] = {chartRight, cursorY};
    bundleViewerCursorYPoints[0] = {cursorX, chartTop};
    bundleViewerCursorYPoints[1] = {cursorX, chartBottom};
    lv_line_set_points(ui_BundleViewerCursorXLine, bundleViewerCursorXPoints, 2);
    lv_line_set_points(ui_BundleViewerCursorYLine, bundleViewerCursorYPoints, 2);
    lv_obj_clear_flag(ui_BundleViewerCursorXLine, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_BundleViewerCursorYLine, LV_OBJ_FLAG_HIDDEN);
    lv_obj_move_foreground(ui_BundleViewerCursorXLine);
    lv_obj_move_foreground(ui_BundleViewerCursorYLine);

    if (ui_BundleViewerCursorLabel)
    {
        lv_label_set_text_fmt(ui_BundleViewerCursorLabel, "Cursor:%d/%d", maxCount == 0 ? 0 : sourceIndex + 1, maxCount);
        lv_obj_clear_flag(ui_BundleViewerCursorLabel, LV_OBJ_FLAG_HIDDEN);
        lv_obj_align(ui_BundleViewerCursorLabel, LV_ALIGN_TOP_LEFT, 526, 78);
        lv_obj_move_foreground(ui_BundleViewerCursorLabel);
        lv_obj_invalidate(ui_BundleViewerCursorLabel);
    }

    if (ui_BundleViewerCursorValueLabel)
    {
        const uint32_t signalColor = getBundleViewerColor(slot == 0 ? bundleViewerPrimaryColorIndex : bundleViewerSecondaryColorIndex);
        const char *signalName = signals[slot].empty() ? (slot == 0 ? "S1" : "S2") : signals[slot].c_str();
        const char *signalValue = sourceIndex >= 0 && sourceIndex < static_cast<int>(rawValues[slot].size())
                                      ? rawValues[slot][sourceIndex].c_str()
                                      : "-";
        lv_label_set_text_fmt(ui_BundleViewerCursorValueLabel, "%s:%s", signalName, signalValue);
        lv_obj_set_style_text_color(ui_BundleViewerCursorValueLabel, lv_color_hex(signalColor), LV_PART_MAIN);
        lv_obj_set_style_border_color(ui_BundleViewerCursorValueLabel, lv_color_hex(signalColor), LV_PART_MAIN);
        lv_obj_clear_flag(ui_BundleViewerCursorValueLabel, LV_OBJ_FLAG_HIDDEN);

        const lv_coord_t labelWidth = 220;
        const lv_coord_t labelHeight = 28;
        const lv_coord_t panelWidth = lv_obj_get_width(ui_BundleViewerPanel);
        const lv_coord_t panelHeight = lv_obj_get_height(ui_BundleViewerPanel);
        lv_coord_t labelX = cursorX + 8;
        if (labelX + labelWidth > panelWidth - 8)
        {
            labelX = cursorX - labelWidth - 8;
        }
        if (labelX < 8) labelX = 8;
        lv_coord_t labelY = cursorY - labelHeight / 2;
        if (labelY < 8) labelY = 8;
        if (labelY + labelHeight > panelHeight - 8) labelY = panelHeight - labelHeight - 8;
        lv_obj_set_pos(ui_BundleViewerCursorValueLabel, labelX, labelY);
        lv_obj_move_foreground(ui_BundleViewerCursorValueLabel);
        lv_obj_invalidate(ui_BundleViewerCursorValueLabel);
    }
}

void DataBundleSelectionGui::showBundleViewerCursorNearPoint(const lv_point_t &touchPoint)
{
    if (!ui_BundleViewerChart)
    {
        return;
    }

    const auto activeSignals = getBundleViewerActiveSignals();
    const std::string primarySignal = activeSignals.empty() ? "" : activeSignals[0];
    const std::string secondarySignal = activeSignals.size() > 1 ? activeSignals[1] : "";
    std::vector<double> values[2];

    for (const auto &row : bundleViewerRows)
    {
        if (!row.numeric)
        {
            continue;
        }
        if (row.signalName == primarySignal)
        {
            values[0].push_back(row.numericValue);
        }
        else if (!secondarySignal.empty() && row.signalName == secondarySignal)
        {
            values[1].push_back(row.numericValue);
        }
    }

    const int maxCount = static_cast<int>(std::max(values[0].size(), values[1].size()));
    if (maxCount <= 0)
    {
        hideBundleViewerCursor();
        return;
    }

    const int start = maxCount > bundleViewerVisibleSampleCount
                          ? maxCount - bundleViewerVisibleSampleCount - bundleViewerHistoryOffset
                          : 0;
    const int visibleCount = maxCount - start;

    lv_area_t chartArea;
    lv_obj_get_content_coords(ui_BundleViewerChart, &chartArea);
    const lv_coord_t chartWidth = chartArea.x2 - chartArea.x1;
    const lv_coord_t chartHeight = chartArea.y2 - chartArea.y1;
    if (chartWidth <= 0 || chartHeight <= 0)
    {
        return;
    }

    bool found = false;
    long bestDistance = 0;
    int bestIndex = 0;
    int bestSlot = 0;

    for (int slot = 0; slot < 2; ++slot)
    {
        if (values[slot].empty())
        {
            continue;
        }
        const double minValue = slot == 0 ? bundleViewerPrimaryRangeMin : bundleViewerSecondaryRangeMin;
        const double maxValue = slot == 0 ? bundleViewerPrimaryRangeMax : bundleViewerSecondaryRangeMax;
        for (int i = 0; i < visibleCount && i < bundleViewerVisibleSampleCount; ++i)
        {
            const int sourceIndex = start + i;
            if (sourceIndex < 0 || sourceIndex >= static_cast<int>(values[slot].size()))
            {
                continue;
            }

            lv_coord_t pointX = chartArea.x1;
            if (bundleViewerVisibleSampleCount > 1)
            {
                pointX = chartArea.x1 + static_cast<lv_coord_t>(
                                           std::lround((static_cast<double>(i) * chartWidth) /
                                                       (bundleViewerVisibleSampleCount - 1)));
            }
            const lv_coord_t plotValue = mapBundleViewerValueToPlot(values[slot][sourceIndex], minValue, maxValue);
            const double normalized = static_cast<double>(plotValue - BUNDLE_VIEW_PLOT_MIN) /
                                      static_cast<double>(BUNDLE_VIEW_PLOT_MAX - BUNDLE_VIEW_PLOT_MIN);
            const lv_coord_t pointY = chartArea.y2 - static_cast<lv_coord_t>(std::lround(normalized * chartHeight));
            const long dx = static_cast<long>(touchPoint.x - pointX);
            const long dy = static_cast<long>(touchPoint.y - pointY);
            const long distance = dx * dx + dy * dy;
            if (!found || distance < bestDistance)
            {
                found = true;
                bestDistance = distance;
                bestIndex = i;
                bestSlot = slot;
            }
        }
    }

    if (!found)
    {
        hideBundleViewerCursor();
        return;
    }

    bundleViewerCursorSignalSlot = bestSlot;
    showBundleViewerCursorAtIndex(bestIndex);
}

void DataBundleSelectionGui::hideBundleViewerCursor()
{
    bundleViewerCursorVisible = false;
    if (ui_BundleViewerCursorXLine)
    {
        lv_obj_add_flag(ui_BundleViewerCursorXLine, LV_OBJ_FLAG_HIDDEN);
    }
    if (ui_BundleViewerCursorYLine)
    {
        lv_obj_add_flag(ui_BundleViewerCursorYLine, LV_OBJ_FLAG_HIDDEN);
    }
    if (ui_BundleViewerCursorLabel)
    {
        lv_obj_add_flag(ui_BundleViewerCursorLabel, LV_OBJ_FLAG_HIDDEN);
    }
    if (ui_BundleViewerCursorValueLabel)
    {
        lv_obj_add_flag(ui_BundleViewerCursorValueLabel, LV_OBJ_FLAG_HIDDEN);
    }
}

void DataBundleSelectionGui::showBundleViewerSettings()
{
    if (!ui_BundleViewerPanel || ui_BundleViewerSettingsOverlay)
    {
        return;
    }

    ui_BundleViewerSettingsOverlay = lv_obj_create(lv_scr_act());
    lv_obj_set_size(ui_BundleViewerSettingsOverlay, lv_pct(100), lv_pct(100));
    lv_obj_clear_flag(ui_BundleViewerSettingsOverlay, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_flag(ui_BundleViewerSettingsOverlay, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_bg_opa(ui_BundleViewerSettingsOverlay, LV_OPA_0, LV_PART_MAIN);
    lv_obj_set_style_border_width(ui_BundleViewerSettingsOverlay, 0, LV_PART_MAIN);
    lv_obj_add_event_cb(ui_BundleViewerSettingsOverlay, [](lv_event_t *e)
                        {
        auto self = static_cast<DataBundleSelectionGui*>(lv_event_get_user_data(e));
        self->hideBundleViewerSettings(); }, LV_EVENT_CLICKED, this);

    ui_BundleViewerSettingsPanel = lv_obj_create(ui_BundleViewerSettingsOverlay);
    lv_obj_set_size(ui_BundleViewerSettingsPanel, 220, 132);
    lv_obj_align(ui_BundleViewerSettingsPanel, LV_ALIGN_TOP_RIGHT, -54, 60);
    lv_obj_clear_flag(ui_BundleViewerSettingsPanel, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_flag(ui_BundleViewerSettingsPanel, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_radius(ui_BundleViewerSettingsPanel, 8, LV_PART_MAIN);
    lv_obj_set_style_bg_color(ui_BundleViewerSettingsPanel, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(ui_BundleViewerSettingsPanel, 255, LV_PART_MAIN);
    lv_obj_set_style_border_width(ui_BundleViewerSettingsPanel, 2, LV_PART_MAIN);
    lv_obj_set_style_border_color(ui_BundleViewerSettingsPanel, lv_color_hex(0x009BFF), LV_PART_MAIN);

    lv_obj_t *title = lv_label_create(ui_BundleViewerSettingsPanel);
    lv_label_set_text(title, "Graph Settings");
    lv_obj_align(title, LV_ALIGN_TOP_LEFT, 10, 8);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_14, LV_PART_MAIN);

    lv_obj_t *primaryLabel = lv_label_create(ui_BundleViewerSettingsPanel);
    lv_label_set_text(primaryLabel, "Primary line");
    lv_obj_align(primaryLabel, LV_ALIGN_TOP_LEFT, 10, 42);
    lv_obj_set_style_text_font(primaryLabel, &lv_font_montserrat_12, LV_PART_MAIN);

    ui_BundleViewerPrimarySwatch = lv_btn_create(ui_BundleViewerSettingsPanel);
    lv_obj_set_size(ui_BundleViewerPrimarySwatch, 56, 22);
    lv_obj_align(ui_BundleViewerPrimarySwatch, LV_ALIGN_TOP_RIGHT, -12, 38);
    lv_obj_set_style_radius(ui_BundleViewerPrimarySwatch, 4, LV_PART_MAIN);
    lv_obj_set_style_bg_color(ui_BundleViewerPrimarySwatch, lv_color_hex(getBundleViewerColor(bundleViewerPrimaryColorIndex)), LV_PART_MAIN);
    lv_obj_add_event_cb(ui_BundleViewerPrimarySwatch, [](lv_event_t *e)
                        {
        auto self = static_cast<DataBundleSelectionGui*>(lv_event_get_user_data(e));
        self->cycleBundleViewerSeriesColor(true); }, LV_EVENT_CLICKED, this);

    lv_obj_t *secondaryLabel = lv_label_create(ui_BundleViewerSettingsPanel);
    lv_label_set_text(secondaryLabel, "Secondary line");
    lv_obj_align(secondaryLabel, LV_ALIGN_TOP_LEFT, 10, 76);
    lv_obj_set_style_text_font(secondaryLabel, &lv_font_montserrat_12, LV_PART_MAIN);

    ui_BundleViewerSecondarySwatch = lv_btn_create(ui_BundleViewerSettingsPanel);
    lv_obj_set_size(ui_BundleViewerSecondarySwatch, 56, 22);
    lv_obj_align(ui_BundleViewerSecondarySwatch, LV_ALIGN_TOP_RIGHT, -12, 72);
    lv_obj_set_style_radius(ui_BundleViewerSecondarySwatch, 4, LV_PART_MAIN);
    lv_obj_set_style_bg_color(ui_BundleViewerSecondarySwatch, lv_color_hex(getBundleViewerColor(bundleViewerSecondaryColorIndex)), LV_PART_MAIN);
    lv_obj_add_event_cb(ui_BundleViewerSecondarySwatch, [](lv_event_t *e)
                        {
        auto self = static_cast<DataBundleSelectionGui*>(lv_event_get_user_data(e));
        self->cycleBundleViewerSeriesColor(false); }, LV_EVENT_CLICKED, this);

    lv_obj_move_foreground(ui_BundleViewerSettingsOverlay);
}

void DataBundleSelectionGui::hideBundleViewerSettings()
{
    if (ui_BundleViewerSettingsOverlay)
    {
        lv_obj_del(ui_BundleViewerSettingsOverlay);
        ui_BundleViewerSettingsOverlay = nullptr;
        ui_BundleViewerSettingsPanel = nullptr;
        ui_BundleViewerPrimarySwatch = nullptr;
        ui_BundleViewerSecondarySwatch = nullptr;
    }
}

void DataBundleSelectionGui::cycleBundleViewerSeriesColor(bool primary)
{
    const uint8_t colorCount = sizeof(BUNDLE_VIEWER_COLORS) / sizeof(BUNDLE_VIEWER_COLORS[0]);
    if (primary)
    {
        bundleViewerPrimaryColorIndex = static_cast<uint8_t>((bundleViewerPrimaryColorIndex + 1) % colorCount);
        if (ui_BundleViewerPrimarySwatch)
        {
            lv_obj_set_style_bg_color(ui_BundleViewerPrimarySwatch, lv_color_hex(getBundleViewerColor(bundleViewerPrimaryColorIndex)), LV_PART_MAIN);
        }
    }
    else
    {
        bundleViewerSecondaryColorIndex = static_cast<uint8_t>((bundleViewerSecondaryColorIndex + 1) % colorCount);
        if (ui_BundleViewerSecondarySwatch)
        {
            lv_obj_set_style_bg_color(ui_BundleViewerSecondarySwatch, lv_color_hex(getBundleViewerColor(bundleViewerSecondaryColorIndex)), LV_PART_MAIN);
        }
    }

    updateBundleViewerGraph();
}

void DataBundleSelectionGui::closeBundleViewer()
{
    hideBundleViewerSettings();
    if (ui_BundleViewerOverlay)
    {
        lv_obj_del(ui_BundleViewerOverlay);
        ui_BundleViewerOverlay = nullptr;
        ui_BundleViewerPanel = nullptr;
        ui_BundleViewerGraphTab = nullptr;
        ui_BundleViewerCsvTab = nullptr;
        ui_BundleViewerSettingsButton = nullptr;
        ui_BundleViewerChart = nullptr;
        ui_BundleViewerTable = nullptr;
        ui_BundleViewerScalingLabel = nullptr;
        ui_BundleViewerPrimaryScaleLabel = nullptr;
        ui_BundleViewerSecondaryScaleLabel = nullptr;
        ui_BundleViewerCursorLabel = nullptr;
        ui_BundleViewerCursorValueLabel = nullptr;
        ui_BundleViewerPrimaryLegend = nullptr;
        ui_BundleViewerSecondaryLegend = nullptr;
        ui_BundleViewerPrimaryLegendLabel = nullptr;
        ui_BundleViewerSecondaryLegendLabel = nullptr;
        ui_BundleViewerCursorXLine = nullptr;
        ui_BundleViewerCursorYLine = nullptr;
        ui_BundleViewerPrimarySeries = nullptr;
        ui_BundleViewerSecondarySeries = nullptr;
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
        int index = (intptr_t)lv_obj_get_user_data(lv_event_get_current_target(e));
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
    dataBundleManager.deleteBundle(currentPage * 6 + index);
    updateBundles();
}

void DataBundleSelectionGui::handleDeleteAllButtonClick()
{
    static const char *btns[] = {"Yes", ""};
    showShadowOverlay();
    lv_obj_t *confirmDialog = lv_msgbox_create(lv_scr_act(), "Confirm Deletion", "Delete all data bundles?", btns, true);
    lv_obj_set_width(confirmDialog, 250);
    lv_obj_center(confirmDialog);
    lv_obj_move_foreground(confirmDialog);
    lv_obj_add_event_cb(confirmDialog, [](lv_event_t *e)
                        {
        auto self = static_cast<DataBundleSelectionGui*>(lv_event_get_user_data(e));
        lv_event_code_t code = lv_event_get_code(e);

        if (code == LV_EVENT_VALUE_CHANGED)
        {
            lv_obj_t *msgbox = lv_event_get_current_target(e);
            const char *btnText = lv_msgbox_get_active_btn_text(msgbox);
            if (btnText && strcmp(btnText, "Yes") == 0)
            {
                self->handleDeleteAllConfirmButtonClick();
            }
            self->hideShadowOverlay();
            lv_obj_del(msgbox);
        }
        else if (code == LV_EVENT_DELETE)
        {
            self->hideShadowOverlay();
        } }, LV_EVENT_ALL, this);
}

void DataBundleSelectionGui::handleDeleteAllConfirmButtonClick()
{
    dataBundleManager.deleteAllBundles();
    currentPage = 0;
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

    dataBundleManager.reloadBundleFileNames();
    dataBundleManager.listAllBundles();

    updateBundles();
}

void DataBundleSelectionGui::hideDataBundles()
{
    if (!initialized || !ui_DataBundlesWidget)
        return;

    closeBundleViewer();
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
    const unsigned char amount = dataBundleManager.getDataBundleAmount();
    if(amount > 6){
        unsigned char availablePages = ((amount-1)/6);
        currentPage = (currentPage+1)%(availablePages+1);
    }
    //logMessage("Current page is: %d", currentPage);
    updateBundles();
}

void DataBundleSelectionGui::goToPreviousPage(){
    const unsigned char amount = dataBundleManager.getDataBundleAmount();
    if(amount > 6){
        unsigned char availablePages = ((amount-1)/6);
        currentPage = (currentPage == 0) ? availablePages : currentPage - 1;
    }
    //logMessage("Current page is: %d", currentPage);
    updateBundles();
}

void DataBundleSelectionGui::updateWatcherCells(){
    const unsigned char amount = dataBundleManager.getDataBundleAmount();
    const unsigned char availablePages = amount == 0 ? 0 : static_cast<unsigned char>((amount - 1) / 6);
    for(unsigned char i=0;i<5;i++){
        if(i == currentPage){
            lv_obj_set_style_bg_color(ui_DataBundlePageWatcherCell[i], lv_color_hex(0x009BFF), LV_PART_MAIN);
            lv_obj_set_style_border_color(ui_DataBundlePageWatcherCell[i], lv_color_hex(0x009BFF), LV_PART_MAIN);
            continue;
        }
        else if(amount > 0 && i <= availablePages){
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
        const unsigned char amount = dataBundleManager.getDataBundleAmount();
        const unsigned char availablePages = amount == 0 ? 0 : static_cast<unsigned char>((amount - 1) / 6);
        if (currentPage > availablePages)
        {
            currentPage = availablePages;
        }

        std::array<DataBundleBuffer, 6> currentDataBundles = dataBundleManager.getBundlePage(currentPage);

        for (unsigned char i = 0; i < 6; i++)
        {
            if (currentDataBundles[i].metaBuffer.deviceName.empty())
            {
                if (ui_DataBundle[i])
                {
                    hideSpecificDataBundle(i);
                }
                continue;
            }

            const char* sName = currentDataBundles[i].metaBuffer.deviceName.c_str();
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
        Exception("DataBundleSelectionGui::updateBundles", e.what()).print();
    }
    catch (...)
    {
        Exception("DataBundleSelectionGui::updateBundles", "Unknown Error").print();
    }
}
