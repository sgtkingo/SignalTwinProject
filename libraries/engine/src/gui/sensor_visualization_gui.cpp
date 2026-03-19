/**
 * @file sensor_visualization_gui.cpp
 * @brief Implementation of the SensorVisualizationGui class
 *
 * This source file implements the SensorVisualizationGui functionality for
 * active sensor visualization, data display, and navigation.
 *
 * @copyright 2025 MTA
 * @author Ing. Jiri Konecny, Ondřej Wrubel
 */

#include "sensor_visualization_gui.hpp"
#include "../helpers.hpp"
#include "./images/ui_images.h"
#include <cstdint>
#include <utility>

namespace
{
constexpr uint32_t SIGNAL_CARD_COLORS[] = {
    0x009BFF,
    0xFF6B35,
    0x00B894,
    0x9B51E0,
    0xF2C94C,
    0xEB5757
};

std::vector<std::string> splitOptionsCsv(const std::string &options)
{
    std::vector<std::string> result;
    for (const auto &item : splitString(options, ',')) {
        if (!item.empty()) {
            result.push_back(item);
        }
    }
    return result;
}
}

SensorVisualizationGui::SensorVisualizationGui(SensorManager &sensorManager, DataBundleManager &dataBundleManager) 
                                              : sensorManager(sensorManager), dataBundleManager(dataBundleManager)
{
    // Initialize all GUI pointers to nullptr
    ui_SensorWidget = nullptr;
    ui_SensorLabel = nullptr;
    ui_ContainerForValue_1 = nullptr;
    ui_VisualColorForValue_1 = nullptr;
    ui_LabelValueValue_1 = nullptr;
    ui_LabelDescValue_1 = nullptr;
    ui_LabelTypeValue_1 = nullptr;
    ui_VisualColorForValue_2 = nullptr;
    ui_ContainerForValue_2 = nullptr;
    ui_LabelValueValue_2 = nullptr;
    ui_LabelDescValue_2 = nullptr;
    ui_LabelTypeValue_2 = nullptr;
    ui_SignalScrollContainer = nullptr;
    ui_ChartEmptyLabel = nullptr;
    ui_Chart = nullptr;
    ui_Chart_series_V1 = nullptr;
    ui_Chart_series_V2 = nullptr;
    ui_btnPrev = nullptr;
    ui_btnPrevLabel = nullptr;
    ui_btnNext = nullptr;
    ui_btnNextLabel = nullptr;
    ui_btnBackGroup = nullptr;
    ui_btnBack = nullptr;
    ui_btnBackLabel = nullptr;
    ui_btnBackCornerBottomLeft = nullptr;
    ui_btnBackCornerTopRight = nullptr;
    ui_RecordGroup = nullptr;
    ui_RecordCornerTopLeft = nullptr;
    ui_RecordCornerFillTopLeft = nullptr;
    ui_RecordCornerTopRight = nullptr;
    ui_RecordCornerFillTopRight = nullptr;
    ui_RecordCornerFillTopRight2 = nullptr;
    ui_RecordOutlay = nullptr;
    ui_btnPause = nullptr;
    ui_pauseImage = nullptr;
    ui_btnSync = nullptr;
    ui_syncImage = nullptr;
    ui_btnRecord = nullptr;
    ui_recordImage = nullptr;
    ui_btnClear = nullptr;
    ui_clearImage = nullptr;
    ui_btnSettings = nullptr;
    ui_settingsImage = nullptr;
    ui_SettingsOverlay = nullptr;
    ui_SettingsBridgeGroup = nullptr;
    ui_SettingsBridge = nullptr;
    ui_SettingsBridgeFill = nullptr;
    ui_SettingsGroup = nullptr;
    ui_SettingsOutlay = nullptr;
    ui_SettingsHeaderLine = nullptr;
    ui_SettingsHeaderLabel = nullptr;
    ui_SettingsDataBundleLabel = nullptr;
    ui_SettingsDataBundleCountLabel = nullptr;
    ui_SettingsDataBundleShowButton = nullptr;
    ui_SettingsDataBundleShowButtonLabel = nullptr;
    ui_SettingsDataBundleDeleteAllButton = nullptr;
    ui_SettingsDataBundleDeleteAllButtonLabel = nullptr;
    ui_SettingsCreditsLabel = nullptr;
    ui_SettingsCreditsButton = nullptr;
    ui_SettingsCreditsButtonLabel = nullptr;
    ui_LogoGroup = nullptr;
    ui_LogoCornerBottomLeft = nullptr;
    ui_LogoCornerFillBottomLeft = nullptr;
    ui_LogoCornerTopRight = nullptr;
    ui_LogoCornerFillBottomRight = nullptr;
    ui_LogoOutlay = nullptr;
    ui_LogoImage = nullptr;
    ui_ShadowOverlay = nullptr;
    ui_Alert = nullptr;
    ui_AlertLabel = nullptr;
}

void SensorVisualizationGui::init()
{
    if (initialized)
        return;

    try
    {
        // // logMessage("Initializing SensorVisualizationGui...\n");
        constructVisualization();
        initialized = true;
        // // logMessage("SensorVisualizationGui initialization completed!\n");
    }
    catch (const std::exception &e)
    {
        // // logMessage("SensorVisualizationGui initialization failed: %s\n", e.what());
        initialized = false;
    }
}

void SensorVisualizationGui::constructVisualization()
{
    // // logMessage("\t>constructing sensor visualization...\n");

    // Main sensor widget container
    ui_SensorWidget = lv_obj_create(lv_scr_act());
    lv_obj_remove_style_all(ui_SensorWidget);
    lv_obj_set_width(ui_SensorWidget, 760);
    lv_obj_set_height(ui_SensorWidget, 440);
    lv_obj_set_align(ui_SensorWidget, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_SensorWidget, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE |
                                           LV_OBJ_FLAG_GESTURE_BUBBLE | LV_OBJ_FLAG_SNAPPABLE | LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC |
                                           LV_OBJ_FLAG_SCROLL_MOMENTUM | LV_OBJ_FLAG_SCROLL_CHAIN);
    lv_obj_set_style_radius(ui_SensorWidget, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_SensorWidget, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_SensorWidget, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_SensorWidget, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_SensorWidget, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_SensorWidget, 2, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Sensor title label
    ui_SensorLabel = lv_label_create(ui_SensorWidget);
    lv_obj_set_width(ui_SensorLabel, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_SensorLabel, LV_SIZE_CONTENT); /// 1
    lv_obj_set_x(ui_SensorLabel, 0);
    lv_obj_set_y(ui_SensorLabel, 10);
    lv_obj_set_align(ui_SensorLabel, LV_ALIGN_TOP_MID);
    lv_label_set_text(ui_SensorLabel, "DHT11");
    lv_obj_clear_flag(ui_SensorLabel, LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE |
                                          LV_OBJ_FLAG_SNAPPABLE | LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                                          LV_OBJ_FLAG_SCROLL_CHAIN); /// Flags
    lv_obj_set_style_text_color(ui_SensorLabel, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_SensorLabel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_SensorLabel, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Value container 1
    ui_ContainerForValue_1 = lv_obj_create(ui_SensorWidget);
    lv_obj_remove_style_all(ui_ContainerForValue_1);
    lv_obj_set_width(ui_ContainerForValue_1, 230);
    lv_obj_set_height(ui_ContainerForValue_1, 118);
    lv_obj_set_x(ui_ContainerForValue_1, -230);
    lv_obj_set_y(ui_ContainerForValue_1, -85);
    lv_obj_set_align(ui_ContainerForValue_1, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_ContainerForValue_1,
                      LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE |
                          LV_OBJ_FLAG_SNAPPABLE | LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                          LV_OBJ_FLAG_SCROLL_CHAIN);
    lv_obj_set_style_radius(ui_ContainerForValue_1, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_ContainerForValue_1, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_ContainerForValue_1, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_ContainerForValue_1, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_ContainerForValue_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_ContainerForValue_1, 2, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Color indicator for value 1
    ui_VisualColorForValue_1 = lv_obj_create(ui_ContainerForValue_1);
    lv_obj_remove_style_all(ui_VisualColorForValue_1);
    lv_obj_set_width(ui_VisualColorForValue_1, 20);
    lv_obj_set_height(ui_VisualColorForValue_1, 20);
    lv_obj_set_x(ui_VisualColorForValue_1, -10);
    lv_obj_set_y(ui_VisualColorForValue_1, 10);
    lv_obj_set_align(ui_VisualColorForValue_1, LV_ALIGN_TOP_RIGHT);
    lv_obj_clear_flag(ui_VisualColorForValue_1, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_radius(ui_VisualColorForValue_1, 1000, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_VisualColorForValue_1, lv_color_hex(0x009BFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_VisualColorForValue_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Value label for value 1
    ui_LabelValueValue_1 = lv_label_create(ui_ContainerForValue_1);
    lv_obj_set_width(ui_LabelValueValue_1, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_LabelValueValue_1, LV_SIZE_CONTENT);
    lv_obj_set_x(ui_LabelValueValue_1, 0);
    lv_obj_set_y(ui_LabelValueValue_1, -5);
    lv_obj_set_align(ui_LabelValueValue_1, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelValueValue_1, "0");
    lv_obj_clear_flag(ui_LabelValueValue_1,
                      LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE | LV_OBJ_FLAG_SNAPPABLE |
                          LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                          LV_OBJ_FLAG_SCROLL_CHAIN);
    lv_obj_set_style_text_color(ui_LabelValueValue_1, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelValueValue_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelValueValue_1, &lv_font_montserrat_40, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Description label for value 1
    ui_LabelDescValue_1 = lv_label_create(ui_ContainerForValue_1);
    lv_obj_set_width(ui_LabelDescValue_1, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_LabelDescValue_1, LV_SIZE_CONTENT);
    lv_obj_set_x(ui_LabelDescValue_1, 0);
    lv_obj_set_y(ui_LabelDescValue_1, 30);
    lv_obj_set_align(ui_LabelDescValue_1, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelDescValue_1, "[°C]");
    lv_obj_clear_flag(ui_LabelDescValue_1,
                      LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE | LV_OBJ_FLAG_SNAPPABLE |
                          LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                          LV_OBJ_FLAG_SCROLL_CHAIN);
    lv_obj_set_style_text_color(ui_LabelDescValue_1, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelDescValue_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelDescValue_1, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Type label for value 1
    ui_LabelTypeValue_1 = lv_label_create(ui_ContainerForValue_1);
    lv_obj_set_width(ui_LabelTypeValue_1, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_LabelTypeValue_1, LV_SIZE_CONTENT);
    lv_obj_set_x(ui_LabelTypeValue_1, 15);
    lv_obj_set_y(ui_LabelTypeValue_1, 10);
    lv_label_set_text(ui_LabelTypeValue_1, "Value");
    lv_obj_set_style_text_color(ui_LabelTypeValue_1, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelTypeValue_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Value container 2 (for sensors with multiple values like DHT11)
    ui_ContainerForValue_2 = lv_obj_create(ui_SensorWidget);
    lv_obj_remove_style_all(ui_ContainerForValue_2);
    lv_obj_set_width(ui_ContainerForValue_2, 230);
    lv_obj_set_height(ui_ContainerForValue_2, 118);
    lv_obj_set_x(ui_ContainerForValue_2, -230);
    lv_obj_set_y(ui_ContainerForValue_2, 55);
    lv_obj_set_align(ui_ContainerForValue_2, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_ContainerForValue_2,
                      LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE |
                          LV_OBJ_FLAG_SNAPPABLE | LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                          LV_OBJ_FLAG_SCROLL_CHAIN);
    lv_obj_set_style_radius(ui_ContainerForValue_2, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_ContainerForValue_2, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_ContainerForValue_2, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_ContainerForValue_2, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_ContainerForValue_2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_ContainerForValue_2, 2, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Color indicator for value 2
    ui_VisualColorForValue_2 = lv_obj_create(ui_ContainerForValue_2);
    lv_obj_remove_style_all(ui_VisualColorForValue_2);
    lv_obj_set_width(ui_VisualColorForValue_2, 20);
    lv_obj_set_height(ui_VisualColorForValue_2, 20);
    lv_obj_set_x(ui_VisualColorForValue_2, -10);
    lv_obj_set_y(ui_VisualColorForValue_2, 10);
    lv_obj_set_align(ui_VisualColorForValue_2, LV_ALIGN_TOP_RIGHT);
    lv_obj_clear_flag(ui_VisualColorForValue_2, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_radius(ui_VisualColorForValue_2, 1000, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_VisualColorForValue_2, lv_color_hex(0xFF6B35), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_VisualColorForValue_2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Value label for value 2
    ui_LabelValueValue_2 = lv_label_create(ui_ContainerForValue_2);
    lv_obj_set_width(ui_LabelValueValue_2, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_LabelValueValue_2, LV_SIZE_CONTENT);
    lv_obj_set_x(ui_LabelValueValue_2, 0);
    lv_obj_set_y(ui_LabelValueValue_2, -5);
    lv_obj_set_align(ui_LabelValueValue_2, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelValueValue_2, "0");
    lv_obj_clear_flag(ui_LabelValueValue_2,
                      LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE | LV_OBJ_FLAG_SNAPPABLE |
                          LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                          LV_OBJ_FLAG_SCROLL_CHAIN);
    lv_obj_set_style_text_color(ui_LabelValueValue_2, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelValueValue_2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelValueValue_2, &lv_font_montserrat_40, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Description label for value 2
    ui_LabelDescValue_2 = lv_label_create(ui_ContainerForValue_2);
    lv_obj_set_width(ui_LabelDescValue_2, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_LabelDescValue_2, LV_SIZE_CONTENT);
    lv_obj_set_x(ui_LabelDescValue_2, 0);
    lv_obj_set_y(ui_LabelDescValue_2, 30);
    lv_obj_set_align(ui_LabelDescValue_2, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelDescValue_2, "[]");
    lv_obj_clear_flag(ui_LabelDescValue_2,
                      LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE | LV_OBJ_FLAG_SNAPPABLE |
                          LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                          LV_OBJ_FLAG_SCROLL_CHAIN);
    lv_obj_set_style_text_color(ui_LabelDescValue_2, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelDescValue_2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelDescValue_2, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Type label for value 2
    ui_LabelTypeValue_2 = lv_label_create(ui_ContainerForValue_2);
    lv_obj_set_width(ui_LabelTypeValue_2, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_LabelTypeValue_2, LV_SIZE_CONTENT);
    lv_obj_set_x(ui_LabelTypeValue_2, 15);
    lv_obj_set_y(ui_LabelTypeValue_2, 10);
    lv_label_set_text(ui_LabelTypeValue_2, "Value2");
    lv_obj_set_style_text_color(ui_LabelTypeValue_2, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelTypeValue_2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Initially hide second value container
    lv_obj_add_flag(ui_ContainerForValue_1, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_ContainerForValue_2, LV_OBJ_FLAG_HIDDEN);

    ui_SignalScrollContainer = lv_obj_create(ui_SensorWidget);
    lv_obj_set_size(ui_SignalScrollContainer, 230, 260);
    lv_obj_set_x(ui_SignalScrollContainer, -230);
    lv_obj_set_y(ui_SignalScrollContainer, -15);
    lv_obj_set_align(ui_SignalScrollContainer, LV_ALIGN_CENTER);
    lv_obj_set_scroll_dir(ui_SignalScrollContainer, LV_DIR_VER);
    lv_obj_set_scrollbar_mode(ui_SignalScrollContainer, LV_SCROLLBAR_MODE_AUTO);
    lv_obj_set_style_pad_all(ui_SignalScrollContainer, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_gap(ui_SignalScrollContainer, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_SignalScrollContainer, lv_color_hex(0xF8FAFC), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_SignalScrollContainer, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_SignalScrollContainer, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_SignalScrollContainer, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui_SignalScrollContainer, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_flex_flow(ui_SignalScrollContainer, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(ui_SignalScrollContainer, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);

    // Create chart for sensor history
    ui_Chart = lv_chart_create(ui_SensorWidget);
    lv_obj_set_width(ui_Chart, 410);
    lv_obj_set_height(ui_Chart, 280);
    lv_obj_set_x(ui_Chart, 150);
    lv_obj_set_y(ui_Chart, 20);
    lv_obj_set_align(ui_Chart, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Chart, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE |
                                    LV_OBJ_FLAG_GESTURE_BUBBLE | LV_OBJ_FLAG_SNAPPABLE);
    lv_chart_set_type(ui_Chart, LV_CHART_TYPE_LINE);
    lv_chart_set_div_line_count(ui_Chart, HISTORY_CAP - 1, HISTORY_CAP);
    lv_chart_set_axis_tick(ui_Chart, LV_CHART_AXIS_PRIMARY_X, HISTORY_CAP / 2, 0, HISTORY_CAP, 1, true, 50);
    lv_chart_set_axis_tick(ui_Chart, LV_CHART_AXIS_PRIMARY_Y, HISTORY_CAP, 5, 5, 2, true, 50);

    // Chart series for value 1
    ui_Chart_series_V1 = lv_chart_add_series(ui_Chart, lv_color_hex(0x009BFF), LV_CHART_AXIS_PRIMARY_Y);

    // Chart series for value 2 (different color)
    ui_Chart_series_V2 = lv_chart_add_series(ui_Chart, lv_color_hex(0xFF6B35), LV_CHART_AXIS_PRIMARY_Y);

    // Chart styling
    lv_obj_set_style_bg_color(ui_Chart, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Chart, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_Chart, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Chart, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_Chart, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_color(ui_Chart, lv_color_hex(0x000000), LV_PART_TICKS | LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(ui_Chart, 255, LV_PART_TICKS | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_Chart, lv_color_hex(0x000000), LV_PART_TICKS | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Chart, 255, LV_PART_TICKS | LV_STATE_DEFAULT);

    ui_ChartEmptyLabel = lv_label_create(ui_Chart);
    lv_label_set_text(ui_ChartEmptyLabel, "No numeric signal available");
    lv_obj_center(ui_ChartEmptyLabel);
    lv_obj_set_style_text_color(ui_ChartEmptyLabel, lv_color_hex(0x5F6B7A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_flag(ui_ChartEmptyLabel, LV_OBJ_FLAG_HIDDEN);

    // Add navigation and control buttons
    addNavButtonsToWidget(ui_SensorWidget);
    addControlButtonsToWidget(ui_SensorWidget);
    addRecordPanelToWidget(ui_SensorWidget);
    addLogoPanelToWidget(ui_SensorWidget);

    // // logMessage("\t>sensor visualization constructed!\n");
}

void SensorVisualizationGui::addNavButtonsToWidget(lv_obj_t *parentWidget)
{
    if (!parentWidget)
        return;

    // Previous button
    ui_btnPrev = lv_btn_create(parentWidget);
    lv_obj_set_width(ui_btnPrev, 80);
    lv_obj_set_height(ui_btnPrev, 40);
    lv_obj_set_x(ui_btnPrev, 35);
    lv_obj_set_y(ui_btnPrev, -40);
    lv_obj_set_align(ui_btnPrev, LV_ALIGN_BOTTOM_LEFT);
    lv_obj_add_event_cb(ui_btnPrev, [](lv_event_t *e)
                        {
        auto self = static_cast<SensorVisualizationGui*>(lv_event_get_user_data(e));
        self->goToPreviousSensor(); }, LV_EVENT_CLICKED, this);

    ui_btnPrevLabel = lv_label_create(ui_btnPrev);
    lv_label_set_text(ui_btnPrevLabel, "Prev");
    lv_obj_center(ui_btnPrevLabel);
    lv_obj_set_style_text_font(ui_btnPrevLabel, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Next button
    ui_btnNext = lv_btn_create(parentWidget);
    lv_obj_set_width(ui_btnNext, 80);
    lv_obj_set_height(ui_btnNext, 40);
    lv_obj_set_x(ui_btnNext, 183);
    lv_obj_set_y(ui_btnNext, -40);
    lv_obj_set_align(ui_btnNext, LV_ALIGN_BOTTOM_LEFT);
    lv_obj_add_event_cb(ui_btnNext, [](lv_event_t *e)
                        {
        auto self = static_cast<SensorVisualizationGui*>(lv_event_get_user_data(e));
        self->goToNextSensor(); }, LV_EVENT_CLICKED, this);

    ui_btnNextLabel = lv_label_create(ui_btnNext);
    lv_label_set_text(ui_btnNextLabel, "Next");
    lv_obj_center(ui_btnNextLabel);
    lv_obj_set_style_text_font(ui_btnNextLabel, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);

    // // logMessage("Navigation buttons added to widget\n");
}

void SensorVisualizationGui::addControlButtonsToWidget(lv_obj_t *parentWidget)
{
    if (!parentWidget)
        return;

    ui_btnBackGroup = lv_obj_create(parentWidget);
    lv_obj_remove_style_all(ui_btnBackGroup);
    lv_obj_set_width(ui_btnBackGroup, 100);
    lv_obj_set_height(ui_btnBackGroup, 40);
    lv_obj_clear_flag(ui_btnBackGroup, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE); /// Flags

    ui_btnBackCornerBottomLeft = lv_obj_create(ui_btnBackGroup);
    lv_obj_remove_style_all(ui_btnBackCornerBottomLeft);
    lv_obj_set_width(ui_btnBackCornerBottomLeft, 20);
    lv_obj_set_height(ui_btnBackCornerBottomLeft, 20);
    lv_obj_set_align(ui_btnBackCornerBottomLeft, LV_ALIGN_BOTTOM_LEFT);
    lv_obj_clear_flag(ui_btnBackCornerBottomLeft, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_set_style_bg_color(ui_btnBackCornerBottomLeft, lv_color_hex(0x009BFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_btnBackCornerBottomLeft, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui_btnBackCornerBottomLeft, false, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_btnBackCornerTopRight = lv_obj_create(ui_btnBackGroup);
    lv_obj_remove_style_all(ui_btnBackCornerTopRight);
    lv_obj_set_width(ui_btnBackCornerTopRight, 20);
    lv_obj_set_height(ui_btnBackCornerTopRight, 20);
    lv_obj_set_align(ui_btnBackCornerTopRight, LV_ALIGN_TOP_RIGHT);
    lv_obj_clear_flag(ui_btnBackCornerTopRight, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_set_style_bg_color(ui_btnBackCornerTopRight, lv_color_hex(0x009BFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_btnBackCornerTopRight, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui_btnBackCornerTopRight, false, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Back button for returning to menu
    ui_btnBack = lv_btn_create(ui_btnBackGroup);
    lv_obj_set_width(ui_btnBack, 100);
    lv_obj_set_height(ui_btnBack, 40);
    lv_obj_set_align(ui_btnBack, LV_ALIGN_CENTER);
    lv_obj_add_event_cb(ui_btnBack, [](lv_event_t *e)
                        {
        auto self = static_cast<SensorVisualizationGui*>(lv_event_get_user_data(e));
        // // logMessage("Back button pressed - returning to menu\n");
        self->handleBackButtonClick(); }, LV_EVENT_CLICKED, this);

    ui_btnBackLabel = lv_label_create(ui_btnBack);
    lv_label_set_text(ui_btnBackLabel, "Back");
    lv_obj_center(ui_btnBackLabel);
    lv_obj_set_style_text_font(ui_btnBackLabel, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);

    // // logMessage("Control buttons added to widget\n");
}

void SensorVisualizationGui::addRecordPanelToWidget(lv_obj_t *parentWidget)
{
    if (!parentWidget)
        return;

    ui_RecordGroup = lv_obj_create(parentWidget);
    lv_obj_remove_style_all(ui_RecordGroup);
    lv_obj_set_width(ui_RecordGroup, 195);
    lv_obj_set_height(ui_RecordGroup, 45);
    lv_obj_set_x(ui_RecordGroup, -40);
    lv_obj_set_y(ui_RecordGroup, 0);
    lv_obj_set_align(ui_RecordGroup, LV_ALIGN_TOP_RIGHT);
    lv_obj_clear_flag(ui_RecordGroup, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE); /// Flags

    ui_RecordCornerTopLeft = lv_obj_create(ui_RecordGroup);
    lv_obj_remove_style_all(ui_RecordCornerTopLeft);
    lv_obj_set_width(ui_RecordCornerTopLeft, 40);
    lv_obj_set_height(ui_RecordCornerTopLeft, 20);
    lv_obj_clear_flag(ui_RecordCornerTopLeft, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_set_style_bg_color(ui_RecordCornerTopLeft, lv_color_hex(0x055DA9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_RecordCornerTopLeft, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui_RecordCornerTopLeft, false, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_RecordCornerFillTopLeft = lv_obj_create(ui_RecordGroup);
    lv_obj_remove_style_all(ui_RecordCornerFillTopLeft);
    lv_obj_set_width(ui_RecordCornerFillTopLeft, 30);
    lv_obj_set_height(ui_RecordCornerFillTopLeft, 40);
    lv_obj_set_x(ui_RecordCornerFillTopLeft, -20);
    lv_obj_set_y(ui_RecordCornerFillTopLeft, 0);
    lv_obj_clear_flag(ui_RecordCornerFillTopLeft, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_set_style_radius(ui_RecordCornerFillTopLeft, 1000, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_RecordCornerFillTopLeft, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_RecordCornerFillTopLeft, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui_RecordCornerFillTopLeft, false, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_RecordCornerTopRight = lv_obj_create(ui_RecordGroup);
    lv_obj_remove_style_all(ui_RecordCornerTopRight);
    lv_obj_set_width(ui_RecordCornerTopRight, 40);
    lv_obj_set_height(ui_RecordCornerTopRight, 20);
    lv_obj_set_align(ui_RecordCornerTopRight, LV_ALIGN_TOP_RIGHT);
    lv_obj_clear_flag(ui_RecordCornerTopRight, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_set_style_bg_color(ui_RecordCornerTopRight, lv_color_hex(0x055DA9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_RecordCornerTopRight, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui_RecordCornerTopRight, false, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_RecordCornerFillTopRight = lv_obj_create(ui_RecordGroup);
    lv_obj_remove_style_all(ui_RecordCornerFillTopRight);
    lv_obj_set_width(ui_RecordCornerFillTopRight, 26);
    lv_obj_set_height(ui_RecordCornerFillTopRight, 26);
    lv_obj_set_x(ui_RecordCornerFillTopRight, 16);
    lv_obj_set_y(ui_RecordCornerFillTopRight, 0);
    lv_obj_set_align(ui_RecordCornerFillTopRight, LV_ALIGN_TOP_RIGHT);
    lv_obj_clear_flag(ui_RecordCornerFillTopRight, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_RecordCornerFillTopRight, 1000, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_RecordCornerFillTopRight, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_RecordCornerFillTopRight, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui_RecordCornerFillTopRight, false, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_RecordCornerFillTopRight2 = lv_obj_create(ui_RecordGroup);
    lv_obj_remove_style_all(ui_RecordCornerFillTopRight2);
    lv_obj_set_width(ui_RecordCornerFillTopRight2, 10);
    lv_obj_set_height(ui_RecordCornerFillTopRight2, 10);
    lv_obj_set_x(ui_RecordCornerFillTopRight2, 0);
    lv_obj_set_y(ui_RecordCornerFillTopRight2, 13);
    lv_obj_set_align(ui_RecordCornerFillTopRight2, LV_ALIGN_TOP_RIGHT);
    lv_obj_clear_flag(ui_RecordCornerFillTopRight2, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_RecordCornerFillTopRight2, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_RecordCornerFillTopRight2, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_RecordCornerFillTopRight2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui_RecordCornerFillTopRight2, false, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_RecordOutlay = lv_obj_create(ui_RecordGroup);
    lv_obj_remove_style_all(ui_RecordOutlay);
    lv_obj_set_width(ui_RecordOutlay, 175);
    lv_obj_set_height(ui_RecordOutlay, 45);
    lv_obj_set_align(ui_RecordOutlay, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_RecordOutlay, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_set_style_radius(ui_RecordOutlay, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_RecordOutlay, lv_color_hex(0x055DA9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_RecordOutlay, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui_RecordOutlay, false, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_btnPause = lv_btn_create(ui_RecordGroup);
    lv_obj_set_width(ui_btnPause, 37);
    lv_obj_set_height(ui_btnPause, 35);
    lv_obj_set_x(ui_btnPause, 15);
    lv_obj_set_y(ui_btnPause, -1);
    lv_obj_set_align(ui_btnPause, LV_ALIGN_LEFT_MID);
    lv_obj_add_flag(ui_btnPause, LV_OBJ_FLAG_EVENT_BUBBLE); /// Flags
    lv_obj_clear_flag(ui_btnPause, LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE |
                                       LV_OBJ_FLAG_SNAPPABLE | LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                                       LV_OBJ_FLAG_SCROLL_CHAIN); /// Flags
    lv_obj_set_style_radius(ui_btnPause, 7, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(ui_btnPause, [](lv_event_t *e)
                        {
        auto self = static_cast<SensorVisualizationGui*>(lv_event_get_user_data(e));
        self->handlePauseButtonClick(); }, LV_EVENT_CLICKED, this);

    ui_pauseImage = lv_img_create(ui_btnPause);
    lv_img_set_src(ui_pauseImage, &ui_img_playpauseicon_png);
    lv_obj_set_width(ui_pauseImage, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_pauseImage, LV_SIZE_CONTENT); /// 1
    lv_obj_set_x(ui_pauseImage, -1);
    lv_obj_set_y(ui_pauseImage, 0);
    lv_obj_set_align(ui_pauseImage, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_pauseImage, LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE |
                                         LV_OBJ_FLAG_SNAPPABLE | LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_img_set_zoom(ui_pauseImage, 119);

    ui_btnSync = lv_btn_create(ui_RecordGroup);
    lv_obj_set_width(ui_btnSync, 37);
    lv_obj_set_height(ui_btnSync, 35);
    lv_obj_set_x(ui_btnSync, 57);
    lv_obj_set_y(ui_btnSync, -1);
    lv_obj_set_align(ui_btnSync, LV_ALIGN_LEFT_MID);
    lv_obj_add_flag(ui_btnSync, LV_OBJ_FLAG_EVENT_BUBBLE); /// Flags
    lv_obj_clear_flag(ui_btnSync, LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE |
                                      LV_OBJ_FLAG_SNAPPABLE | LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                                      LV_OBJ_FLAG_SCROLL_CHAIN); /// Flags
    lv_obj_set_style_radius(ui_btnSync, 7, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Sync is disabled on start
    lv_obj_set_style_bg_color(ui_btnSync, lv_color_hex(0x949494), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_clear_flag(ui_btnSync, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(ui_btnSync, [](lv_event_t *e)
                        {
        auto self = static_cast<SensorVisualizationGui*>(lv_event_get_user_data(e));
        self->handleSyncButtonClick(); }, LV_EVENT_CLICKED, this);

    ui_syncImage = lv_img_create(ui_btnSync);
    lv_img_set_src(ui_syncImage, &ui_img_clockicon_png);
    lv_obj_set_width(ui_syncImage, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_syncImage, LV_SIZE_CONTENT); /// 1
    lv_obj_set_x(ui_syncImage, -1);
    lv_obj_set_y(ui_syncImage, 0);
    lv_obj_set_align(ui_syncImage, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_syncImage, LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE |
                                        LV_OBJ_FLAG_SNAPPABLE | LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_img_set_zoom(ui_syncImage, 119);

    ui_btnRecord = lv_btn_create(ui_RecordGroup);
    lv_obj_set_width(ui_btnRecord, 37);
    lv_obj_set_height(ui_btnRecord, 35);
    lv_obj_set_x(ui_btnRecord, 99);
    lv_obj_set_y(ui_btnRecord, -1);
    lv_obj_set_align(ui_btnRecord, LV_ALIGN_LEFT_MID);
    lv_obj_add_flag(ui_btnRecord, LV_OBJ_FLAG_EVENT_BUBBLE); /// Flags
    lv_obj_clear_flag(ui_btnRecord, LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE |
                                        LV_OBJ_FLAG_SNAPPABLE | LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                                        LV_OBJ_FLAG_SCROLL_CHAIN); /// Flags
    lv_obj_set_style_radius(ui_btnRecord, 7, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(ui_btnRecord, [](lv_event_t *e)
                        {
        auto self = static_cast<SensorVisualizationGui*>(lv_event_get_user_data(e));
        self->handleRecordButtonClick(nullptr); }, LV_EVENT_CLICKED, this);

    ui_recordImage = lv_img_create(ui_btnRecord);
    lv_img_set_src(ui_recordImage, &ui_img_recordicon_png);
    lv_obj_set_width(ui_recordImage, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_recordImage, LV_SIZE_CONTENT); /// 1
    lv_obj_set_x(ui_recordImage, -1);
    lv_obj_set_y(ui_recordImage, 0);
    lv_obj_set_align(ui_recordImage, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_recordImage, LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE |
                                          LV_OBJ_FLAG_SNAPPABLE | LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_img_set_zoom(ui_recordImage, 119);

    ui_btnClear = lv_btn_create(ui_RecordGroup);
    lv_obj_set_width(ui_btnClear, 37);
    lv_obj_set_height(ui_btnClear, 35);
    lv_obj_set_x(ui_btnClear, 141);
    lv_obj_set_y(ui_btnClear, -1);
    lv_obj_set_align(ui_btnClear, LV_ALIGN_LEFT_MID);
    lv_obj_add_flag(ui_btnClear, LV_OBJ_FLAG_EVENT_BUBBLE); /// Flags
    lv_obj_clear_flag(ui_btnClear, LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE |
                                       LV_OBJ_FLAG_SNAPPABLE | LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                                       LV_OBJ_FLAG_SCROLL_CHAIN); /// Flags
    lv_obj_set_style_radius(ui_btnClear, 7, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(ui_btnClear, [](lv_event_t *e)
                        {
        auto self = static_cast<SensorVisualizationGui*>(lv_event_get_user_data(e));
        self->handleClearButtonClick(); }, LV_EVENT_CLICKED, this);

    ui_clearImage = lv_img_create(ui_btnClear);
    lv_img_set_src(ui_clearImage, &ui_img_trashicon_png);
    lv_obj_set_width(ui_clearImage, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_clearImage, LV_SIZE_CONTENT); /// 1
    lv_obj_set_x(ui_clearImage, -1);
    lv_obj_set_y(ui_clearImage, 0);
    lv_obj_set_align(ui_clearImage, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_clearImage, LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE |
                                         LV_OBJ_FLAG_SNAPPABLE | LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_img_set_zoom(ui_clearImage, 119);

    ui_btnSettings = lv_btn_create(parentWidget);
    lv_obj_set_width(ui_btnSettings, 37);
    lv_obj_set_height(ui_btnSettings, 36);
    lv_obj_set_x(ui_btnSettings, -7);
    lv_obj_set_y(ui_btnSettings, 4);
    lv_obj_set_align(ui_btnSettings, LV_ALIGN_TOP_RIGHT);
    lv_obj_add_flag(ui_btnSettings, LV_OBJ_FLAG_EVENT_BUBBLE); /// Flags
    lv_obj_clear_flag(ui_btnSettings, LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE |
                                          LV_OBJ_FLAG_SNAPPABLE | LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                                          LV_OBJ_FLAG_SCROLL_CHAIN); /// Flags
    lv_obj_set_style_radius(ui_btnSettings, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(ui_btnSettings, [](lv_event_t *e)
                        {
        auto self = static_cast<SensorVisualizationGui*>(lv_event_get_user_data(e));
        self->handleSettingsButtonClick(self->ui_RecordGroup,self->ui_btnSettings,self->getParentWidget()); }, LV_EVENT_CLICKED, this);

    ui_settingsImage = lv_img_create(ui_btnSettings);
    lv_img_set_src(ui_settingsImage, &ui_img_settings_png);
    lv_obj_set_width(ui_settingsImage, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_settingsImage, LV_SIZE_CONTENT); /// 1
    lv_obj_set_x(ui_settingsImage, -1);
    lv_obj_set_y(ui_settingsImage, 0);
    lv_obj_set_align(ui_settingsImage, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_settingsImage, LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE |
                                            LV_OBJ_FLAG_SNAPPABLE | LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_img_set_zoom(ui_settingsImage, 119);
}

void SensorVisualizationGui::showShadowOverlay()
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

void SensorVisualizationGui::hideShadowOverlay()
{
    if (ui_ShadowOverlay)
    {
        lv_obj_del(ui_ShadowOverlay);
        ui_ShadowOverlay = nullptr;
    }
}

void SensorVisualizationGui::addLogoPanelToWidget(lv_obj_t *parentWidget)
{
    ui_LogoGroup = lv_obj_create(parentWidget);
    lv_obj_remove_style_all(ui_LogoGroup);
    lv_obj_set_width(ui_LogoGroup, 100);
    lv_obj_set_height(ui_LogoGroup, 20);
    lv_obj_set_x(ui_LogoGroup, 9);
    lv_obj_set_y(ui_LogoGroup, 0);
    lv_obj_set_align(ui_LogoGroup, LV_ALIGN_BOTTOM_RIGHT);
    lv_obj_clear_flag(ui_LogoGroup, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE); /// Flags

    ui_LogoCornerBottomLeft = lv_obj_create(ui_LogoGroup);
    lv_obj_remove_style_all(ui_LogoCornerBottomLeft);
    lv_obj_set_width(ui_LogoCornerBottomLeft, 10);
    lv_obj_set_height(ui_LogoCornerBottomLeft, 10);
    lv_obj_set_x(ui_LogoCornerBottomLeft, 10);
    lv_obj_set_y(ui_LogoCornerBottomLeft, 0);
    lv_obj_set_align(ui_LogoCornerBottomLeft, LV_ALIGN_BOTTOM_LEFT);
    lv_obj_clear_flag(ui_LogoCornerBottomLeft, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_set_style_bg_color(ui_LogoCornerBottomLeft, lv_color_hex(0x055DA9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_LogoCornerBottomLeft, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui_LogoCornerBottomLeft, false, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LogoCornerTopRight = lv_obj_create(ui_LogoGroup);
    lv_obj_remove_style_all(ui_LogoCornerTopRight);
    lv_obj_set_width(ui_LogoCornerTopRight, 10);
    lv_obj_set_height(ui_LogoCornerTopRight, 10);
    lv_obj_set_x(ui_LogoCornerTopRight, -9);
    lv_obj_set_y(ui_LogoCornerTopRight, 0);
    lv_obj_set_align(ui_LogoCornerTopRight, LV_ALIGN_TOP_RIGHT);
    lv_obj_clear_flag(ui_LogoCornerTopRight, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_set_style_bg_color(ui_LogoCornerTopRight, lv_color_hex(0x055DA9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_LogoCornerTopRight, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui_LogoCornerTopRight, false, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LogoOutlay = lv_obj_create(ui_LogoGroup);
    lv_obj_remove_style_all(ui_LogoOutlay);
    lv_obj_set_width(ui_LogoOutlay, 80);
    lv_obj_set_height(ui_LogoOutlay, 20);
    lv_obj_set_align(ui_LogoOutlay, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_LogoOutlay, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_set_style_radius(ui_LogoOutlay, 1000, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_LogoOutlay, lv_color_hex(0x055DA9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_LogoOutlay, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui_LogoOutlay, false, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LogoImage = lv_img_create(ui_LogoGroup);
    lv_img_set_src(ui_LogoImage, &ui_img_mtalogo_png);
    lv_obj_set_width(ui_LogoImage, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_LogoImage, LV_SIZE_CONTENT); /// 1
    lv_obj_set_align(ui_LogoImage, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_LogoImage, LV_OBJ_FLAG_ADV_HITTEST);  /// Flags
    lv_obj_clear_flag(ui_LogoImage, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_img_set_zoom(ui_LogoImage, 70);
}

void SensorVisualizationGui::drawCurrentSensor()
{
    if (!currentSensor)
    {
        // // logMessage("No current sensor to draw\n");
        return;
    }

    // // logMessage("Drawing sensor: %s\n", currentSensor->UID.c_str());
    if (!currentSensor->getRedrawPending())
    {
        return;
    }

    if (paused)
    {
        return;
    }

    updateSensorDataDisplay();
    updateChart();
}

uint32_t SensorVisualizationGui::getSignalAccentColor(size_t index)
{
    return SIGNAL_CARD_COLORS[index % (sizeof(SIGNAL_CARD_COLORS) / sizeof(SIGNAL_CARD_COLORS[0]))];
}

bool SensorVisualizationGui::isNumericType(SensorDataType dtype)
{
    return dtype == SensorDataType::INT || dtype == SensorDataType::FLOAT || dtype == SensorDataType::DOUBLE;
}

bool SensorVisualizationGui::hasSelectableOptions(const SensorParam &param)
{
    return !param.Restrictions.Options.empty();
}

bool SensorVisualizationGui::supportsSliderInput(const SensorParam &param)
{
    return param.DType == SensorDataType::INT &&
           !param.Restrictions.Min.empty() &&
           !param.Restrictions.Max.empty();
}

std::string SensorVisualizationGui::buildUnitText(const std::string &unit, const char *fallbackText)
{
    if (unit.empty()) {
        return fallbackText ? std::string(fallbackText) : std::string();
    }

    return "[" + unit + "]";
}

bool SensorVisualizationGui::currentDeviceSupportsRecording() const
{
    return currentSensor && !getChartableValueKeys().empty();
}

void SensorVisualizationGui::ensureSignalCards(size_t count)
{
    if (!ui_SignalScrollContainer) {
        return;
    }

    while (signalCards.size() < count) {
        SignalCard card;
        card.container = lv_obj_create(ui_SignalScrollContainer);
        lv_obj_set_width(card.container, lv_pct(100));
        lv_obj_set_height(card.container, 74);
        lv_obj_clear_flag(card.container, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_set_style_pad_left(card.container, 16, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_right(card.container, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_top(card.container, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_bottom(card.container, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_radius(card.container, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(card.container, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_color(card.container, lv_color_hex(0xD0D7DE), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(card.container, 1, LV_PART_MAIN | LV_STATE_DEFAULT);

        card.accent = lv_obj_create(card.container);
        lv_obj_remove_style_all(card.accent);
        lv_obj_set_size(card.accent, 6, 54);
        lv_obj_align(card.accent, LV_ALIGN_LEFT_MID, -8, 0);
        lv_obj_set_style_radius(card.accent, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(card.accent, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

        card.nameLabel = lv_label_create(card.container);
        lv_obj_align(card.nameLabel, LV_ALIGN_TOP_LEFT, 6, 0);
        lv_obj_set_style_text_font(card.nameLabel, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(card.nameLabel, lv_color_hex(0x3B4252), LV_PART_MAIN | LV_STATE_DEFAULT);

        card.valueLabel = lv_label_create(card.container);
        lv_obj_align(card.valueLabel, LV_ALIGN_CENTER, 0, 6);
        lv_obj_set_style_text_font(card.valueLabel, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(card.valueLabel, lv_color_hex(0x111111), LV_PART_MAIN | LV_STATE_DEFAULT);

        card.unitLabel = lv_label_create(card.container);
        lv_obj_align(card.unitLabel, LV_ALIGN_BOTTOM_LEFT, 6, 0);
        lv_obj_set_width(card.unitLabel, 190);
        lv_obj_set_style_text_font(card.unitLabel, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(card.unitLabel, lv_color_hex(0x5F6B7A), LV_PART_MAIN | LV_STATE_DEFAULT);

        signalCards.push_back(card);
    }
}

void SensorVisualizationGui::clearUnusedSignalCards(size_t usedCount)
{
    for (size_t i = 0; i < signalCards.size(); ++i) {
        if (!signalCards[i].container) {
            continue;
        }

        if (i < usedCount) {
            lv_obj_clear_flag(signalCards[i].container, LV_OBJ_FLAG_HIDDEN);
        } else {
            lv_obj_add_flag(signalCards[i].container, LV_OBJ_FLAG_HIDDEN);
        }
    }
}

void SensorVisualizationGui::ensureConfigControls(size_t count)
{
    if (!ui_SignalScrollContainer) {
        return;
    }

    while (configControls.size() < count) {
        const size_t nextIndex = configControls.size();

        ConfigControl control;
        control.container = lv_obj_create(ui_SignalScrollContainer);
        lv_obj_set_width(control.container, lv_pct(100));
        lv_obj_set_height(control.container, 108);
        lv_obj_clear_flag(control.container, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_set_style_pad_left(control.container, 16, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_right(control.container, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_top(control.container, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_bottom(control.container, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_radius(control.container, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(control.container, lv_color_hex(0xF3F9FF), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_color(control.container, lv_color_hex(0x8FBDE8), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(control.container, 1, LV_PART_MAIN | LV_STATE_DEFAULT);

        control.accent = lv_obj_create(control.container);
        lv_obj_remove_style_all(control.accent);
        lv_obj_set_size(control.accent, 6, 88);
        lv_obj_align(control.accent, LV_ALIGN_LEFT_MID, -8, 0);
        lv_obj_set_style_radius(control.accent, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(control.accent, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

        control.nameLabel = lv_label_create(control.container);
        lv_obj_align(control.nameLabel, LV_ALIGN_TOP_LEFT, 6, 0);
        lv_obj_set_width(control.nameLabel, 150);
        lv_obj_set_style_text_font(control.nameLabel, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(control.nameLabel, lv_color_hex(0x24415E), LV_PART_MAIN | LV_STATE_DEFAULT);

        control.valueLabel = lv_label_create(control.container);
        lv_obj_align(control.valueLabel, LV_ALIGN_TOP_RIGHT, -4, 0);
        lv_obj_set_width(control.valueLabel, 70);
        lv_obj_set_style_text_align(control.valueLabel, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_font(control.valueLabel, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(control.valueLabel, lv_color_hex(0x0B7285), LV_PART_MAIN | LV_STATE_DEFAULT);

        control.unitLabel = lv_label_create(control.container);
        lv_obj_align(control.unitLabel, LV_ALIGN_BOTTOM_LEFT, 6, 0);
        lv_obj_set_width(control.unitLabel, 190);
        lv_obj_set_style_text_font(control.unitLabel, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(control.unitLabel, lv_color_hex(0x5F6B7A), LV_PART_MAIN | LV_STATE_DEFAULT);

        control.editor = lv_textarea_create(control.container);
        lv_obj_set_size(control.editor, 180, 34);
        lv_obj_align(control.editor, LV_ALIGN_BOTTOM_RIGHT, -4, -2);
        lv_textarea_set_one_line(control.editor, true);
        lv_textarea_set_max_length(control.editor, 24);
        lv_obj_set_user_data(control.editor, reinterpret_cast<void *>(static_cast<intptr_t>(nextIndex)));
        lv_obj_add_event_cb(control.editor, [](lv_event_t *e) {
            if (lv_event_get_code(e) != LV_EVENT_READY && lv_event_get_code(e) != LV_EVENT_DEFOCUSED) {
                return;
            }

            auto *self = static_cast<SensorVisualizationGui *>(lv_event_get_user_data(e));
            const int index = static_cast<int>(reinterpret_cast<intptr_t>(lv_obj_get_user_data(lv_event_get_target(e))));
            self->handleTextConfigSubmitted(static_cast<size_t>(index));
        }, LV_EVENT_ALL, this);

        configControls.push_back(control);
    }
}

void SensorVisualizationGui::clearUnusedConfigControls(size_t usedCount)
{
    for (size_t i = 0; i < configControls.size(); ++i) {
        if (!configControls[i].container) {
            continue;
        }

        if (i < usedCount) {
            lv_obj_clear_flag(configControls[i].container, LV_OBJ_FLAG_HIDDEN);
        } else {
            lv_obj_add_flag(configControls[i].container, LV_OBJ_FLAG_HIDDEN);
        }
    }
}

bool SensorVisualizationGui::buildNumericHistoryForKey(const std::string &key, lv_coord_t *history)
{
    if (!currentSensor || !history) {
        return false;
    }

    auto values = currentSensor->getValues();
    auto it = values.find(key);
    if (it == values.end()) {
        return false;
    }

    switch (it->second.DType)
    {
    case SensorDataType::INT:
        buildSensorHistory<int>(currentSensor, key, history);
        return true;
    case SensorDataType::FLOAT:
        buildSensorHistory<float>(currentSensor, key, history);
        return true;
    case SensorDataType::DOUBLE:
        buildSensorHistory<double>(currentSensor, key, history);
        return true;
    default:
        return false;
    }
}

std::vector<std::string> SensorVisualizationGui::getChartableValueKeys() const
{
    std::vector<std::string> chartKeys;
    if (!currentSensor) {
        return chartKeys;
    }

    const auto values = currentSensor->getValues();
    for (const auto &key : currentSensor->getValuesKeys()) {
        auto it = values.find(key);
        if (it == values.end()) {
            continue;
        }

        if (isNumericType(it->second.DType)) {
            chartKeys.push_back(key);
        }

        if (chartKeys.size() == 2) {
            break;
        }
    }

    return chartKeys;
}

void SensorVisualizationGui::updateSensorDataDisplay()
{
    if (!currentSensor || !ui_SignalScrollContainer)
        return;

    if (ui_SensorLabel)
    {
        const std::string title = currentSensor->getName() + " [" + currentSensor->getRoleLabel() + "]";
        lv_label_set_text(ui_SensorLabel, title.c_str());
    }

    const auto values = currentSensor->getValues();
    const auto valueKeys = currentSensor->getValuesKeys();
    const auto configs = currentSensor->getConfigs();
    const auto configKeys = currentSensor->getConfigsKeys();

    ensureSignalCards(valueKeys.size());
    ensureConfigControls(configKeys.size());

    for (size_t i = 0; i < valueKeys.size(); ++i) {
        const auto &key = valueKeys[i];
        auto it = values.find(key);
        if (it == values.end() || i >= signalCards.size()) {
            continue;
        }

        const uint32_t accentColor = getSignalAccentColor(i);
        lv_obj_set_style_bg_color(signalCards[i].accent, lv_color_hex(accentColor), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_label_set_text(signalCards[i].nameLabel, key.c_str());
        lv_label_set_text(signalCards[i].valueLabel, it->second.Value.c_str());
        const std::string units = buildUnitText(currentSensor->getValueUnits(key), "Live value");
        lv_label_set_text(signalCards[i].unitLabel, units.c_str());
    }

    clearUnusedSignalCards(valueKeys.size());

    for (size_t i = 0; i < configKeys.size(); ++i) {
        const auto &key = configKeys[i];
        auto it = configs.find(key);
        if (it == configs.end() || i >= configControls.size()) {
            continue;
        }

        ConfigControl &control = configControls[i];
        control.key = key;
        control.usesDropdown = false;
        control.usesSlider = false;

        const uint32_t accentColor = getSignalAccentColor(i + valueKeys.size());
        lv_obj_set_style_bg_color(control.accent, lv_color_hex(accentColor), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_label_set_text(control.nameLabel, key.c_str());
        lv_label_set_text(control.valueLabel, it->second.Value.c_str());
        lv_label_set_text(control.unitLabel, buildUnitText(currentSensor->getConfigUnits(key), "Queued via CONFIG").c_str());

        if (hasSelectableOptions(it->second)) {
            if (control.editor) {
                lv_obj_del(control.editor);
            }

            control.editor = lv_dropdown_create(control.container);
            control.usesDropdown = true;
            lv_obj_set_size(control.editor, 180, 34);
            lv_obj_align(control.editor, LV_ALIGN_BOTTOM_RIGHT, -4, -2);
            lv_obj_set_user_data(control.editor, reinterpret_cast<void *>(static_cast<intptr_t>(i)));
            lv_obj_add_event_cb(control.editor, [](lv_event_t *e) {
                if (lv_event_get_code(e) != LV_EVENT_VALUE_CHANGED) {
                    return;
                }

                auto *self = static_cast<SensorVisualizationGui *>(lv_event_get_user_data(e));
                const int index = static_cast<int>(reinterpret_cast<intptr_t>(lv_obj_get_user_data(lv_event_get_target(e))));
                self->handleDropdownConfigChanged(static_cast<size_t>(index));
            }, LV_EVENT_ALL, this);
        } else if (supportsSliderInput(it->second)) {
            if (control.editor) {
                lv_obj_del(control.editor);
            }

            control.editor = lv_slider_create(control.container);
            control.usesSlider = true;
            lv_obj_set_size(control.editor, 180, 16);
            lv_obj_align(control.editor, LV_ALIGN_BOTTOM_RIGHT, -4, -12);
            lv_obj_set_user_data(control.editor, reinterpret_cast<void *>(static_cast<intptr_t>(i)));
            lv_obj_add_event_cb(control.editor, [](lv_event_t *e) {
                if (lv_event_get_code(e) != LV_EVENT_VALUE_CHANGED) {
                    return;
                }

                auto *self = static_cast<SensorVisualizationGui *>(lv_event_get_user_data(e));
                const int index = static_cast<int>(reinterpret_cast<intptr_t>(lv_obj_get_user_data(lv_event_get_target(e))));
                self->handleSliderConfigChanged(static_cast<size_t>(index));
            }, LV_EVENT_ALL, this);
        } else if (!control.editor || control.usesDropdown || control.usesSlider) {
            if (control.editor) {
                lv_obj_del(control.editor);
            }

            control.editor = lv_textarea_create(control.container);
            lv_obj_set_size(control.editor, 180, 34);
            lv_obj_align(control.editor, LV_ALIGN_BOTTOM_RIGHT, -4, -2);
            lv_textarea_set_one_line(control.editor, true);
            lv_textarea_set_max_length(control.editor, 24);
            lv_obj_set_user_data(control.editor, reinterpret_cast<void *>(static_cast<intptr_t>(i)));
            lv_obj_add_event_cb(control.editor, [](lv_event_t *e) {
                if (lv_event_get_code(e) != LV_EVENT_READY && lv_event_get_code(e) != LV_EVENT_DEFOCUSED) {
                    return;
                }

                auto *self = static_cast<SensorVisualizationGui *>(lv_event_get_user_data(e));
                const int index = static_cast<int>(reinterpret_cast<intptr_t>(lv_obj_get_user_data(lv_event_get_target(e))));
                self->handleTextConfigSubmitted(static_cast<size_t>(index));
            }, LV_EVENT_ALL, this);
        }

        if (control.usesDropdown) {
            std::string optionsText;
            int selectedIndex = 0;
            int currentIndex = 0;
            for (const auto &option : splitOptionsCsv(it->second.Restrictions.Options)) {
                if (!optionsText.empty()) {
                    optionsText += "\n";
                }
                optionsText += option;
                if (option == it->second.Value) {
                    selectedIndex = currentIndex;
                }
                ++currentIndex;
            }
            lv_dropdown_set_options(control.editor, optionsText.c_str());
            lv_dropdown_set_selected(control.editor, selectedIndex);
        } else if (control.usesSlider) {
            const int minValue = convertStringToType<int>(it->second.Restrictions.Min);
            const int maxValue = convertStringToType<int>(it->second.Restrictions.Max);
            lv_slider_set_range(control.editor, minValue, maxValue);
            lv_slider_set_value(control.editor, convertStringToType<int>(it->second.Value), LV_ANIM_OFF);
        } else {
            lv_textarea_set_text(control.editor, it->second.Value.c_str());
        }
    }

    clearUnusedConfigControls(configKeys.size());
    updateActionButtonsState();
}

void SensorVisualizationGui::updateChart()
{
    if (!currentSensor || !ui_Chart || !ui_Chart_series_V1 || !ui_Chart_series_V2)
        return;

    if (sensorManager.isRedrawPending() == false)
        return;

    const auto chartKeys = getChartableValueKeys();
    if (chartKeys.empty())
    {
        lv_chart_set_all_value(ui_Chart, ui_Chart_series_V1, LV_CHART_POINT_NONE);
        lv_chart_set_all_value(ui_Chart, ui_Chart_series_V2, LV_CHART_POINT_NONE);
        if (ui_ChartEmptyLabel) {
            lv_label_set_text(ui_ChartEmptyLabel,
                              currentSensor->getRole() == DeviceRole::ACTUATOR
                                  ? "Control-only device"
                                  : "No numeric signal available");
            lv_obj_clear_flag(ui_ChartEmptyLabel, LV_OBJ_FLAG_HIDDEN);
        }
        lv_chart_refresh(ui_Chart);
        return;
    }

    try
    {
        if (ui_ChartEmptyLabel) {
            lv_obj_add_flag(ui_ChartEmptyLabel, LV_OBJ_FLAG_HIDDEN);
        }

        lv_coord_t historyPrimary[HISTORY_CAP];
        if (!buildNumericHistoryForKey(chartKeys[0], historyPrimary)) {
            return;
        }

        auto computeRange = [](const lv_coord_t *history) {
            lv_coord_t minValue = history[0];
            lv_coord_t maxValue = history[0];
            for (int i = 1; i < HISTORY_CAP; ++i) {
                if (history[i] < minValue) {
                    minValue = history[i];
                }
                if (history[i] > maxValue) {
                    maxValue = history[i];
                }
            }
            if (minValue == maxValue) {
                minValue -= 1;
                maxValue += 1;
            }
            const lv_coord_t span = maxValue - minValue;
            const lv_coord_t pad = (span / 10) > 1 ? (span / 10) : 1;
            return std::pair<lv_coord_t, lv_coord_t>(minValue - pad, maxValue + pad);
        };

        auto primaryRange = computeRange(historyPrimary);
        lv_coord_t globalMin = primaryRange.first;
        lv_coord_t globalMax = primaryRange.second;

        bool haveSecond = false;
        lv_coord_t historySecondary[HISTORY_CAP];
        if (chartKeys.size() > 1) {
            haveSecond = buildNumericHistoryForKey(chartKeys[1], historySecondary);
            if (haveSecond) {
                auto secondaryRange = computeRange(historySecondary);
                if (secondaryRange.first < globalMin) {
                    globalMin = secondaryRange.first;
                }
                if (secondaryRange.second > globalMax) {
                    globalMax = secondaryRange.second;
                }
            }
        }

        lv_chart_set_range(ui_Chart, LV_CHART_AXIS_PRIMARY_Y, globalMin, globalMax);
        lv_chart_set_range(ui_Chart, LV_CHART_AXIS_SECONDARY_Y, globalMin, globalMax);
        lv_chart_set_all_value(ui_Chart, ui_Chart_series_V1, LV_CHART_POINT_NONE);
        lv_chart_set_all_value(ui_Chart, ui_Chart_series_V2, LV_CHART_POINT_NONE);

        for (int i = 0; i < HISTORY_CAP; i++)
        {
            lv_chart_set_next_value(ui_Chart, ui_Chart_series_V1, historyPrimary[i]);
        }

        if (haveSecond)
        {
            for (int i = 0; i < HISTORY_CAP; i++)
            {
                lv_chart_set_next_value(ui_Chart, ui_Chart_series_V2, historySecondary[i]);
            }
        }

        lv_chart_refresh(ui_Chart);
    }
    catch (const std::exception &e)
    {
        // logMessage("Error updating chart: %s\n", e.what());
    }
}

void SensorVisualizationGui::updateActionButtonsState()
{
    const bool canRecord = currentDeviceSupportsRecording();
    const lv_color_t enabledColor = lv_color_hex(0x009BFF);
    const lv_color_t disabledColor = lv_color_hex(0x949494);

    if (ui_btnRecord) {
        lv_obj_set_style_bg_color(ui_btnRecord, canRecord && recording ? lv_color_hex(0xE55858) : (canRecord ? enabledColor : disabledColor), LV_PART_MAIN | LV_STATE_DEFAULT);
        if (canRecord) {
            lv_obj_add_flag(ui_btnRecord, LV_OBJ_FLAG_CLICKABLE);
        } else {
            lv_obj_clear_flag(ui_btnRecord, LV_OBJ_FLAG_CLICKABLE);
        }
    }

    if (ui_btnClear) {
        lv_obj_set_style_bg_color(ui_btnClear, canRecord ? enabledColor : disabledColor, LV_PART_MAIN | LV_STATE_DEFAULT);
        if (canRecord) {
            lv_obj_add_flag(ui_btnClear, LV_OBJ_FLAG_CLICKABLE);
        } else {
            lv_obj_clear_flag(ui_btnClear, LV_OBJ_FLAG_CLICKABLE);
        }
    }
}

bool SensorVisualizationGui::applyConfigValue(const std::string &key, const std::string &value)
{
    if (!currentSensor || key.empty()) {
        return false;
    }

    try {
        currentSensor->setConfig(key, value);
        return true;
    } catch (...) {
        showAlert("Failed to queue config value");
        return false;
    }
}

void SensorVisualizationGui::handleDropdownConfigChanged(size_t controlIndex)
{
    if (controlIndex >= configControls.size()) {
        return;
    }

    ConfigControl &control = configControls[controlIndex];
    if (!control.editor || control.key.empty()) {
        return;
    }

    char selected[64] = {0};
    lv_dropdown_get_selected_str(control.editor, selected, sizeof(selected));
    if (applyConfigValue(control.key, selected) && control.valueLabel) {
        lv_label_set_text(control.valueLabel, selected);
    }
}

void SensorVisualizationGui::handleSliderConfigChanged(size_t controlIndex)
{
    if (controlIndex >= configControls.size()) {
        return;
    }

    ConfigControl &control = configControls[controlIndex];
    if (!control.editor || control.key.empty()) {
        return;
    }

    const std::string value = std::to_string(lv_slider_get_value(control.editor));
    if (applyConfigValue(control.key, value) && control.valueLabel) {
        lv_label_set_text(control.valueLabel, value.c_str());
    }
}

void SensorVisualizationGui::handleTextConfigSubmitted(size_t controlIndex)
{
    if (controlIndex >= configControls.size()) {
        return;
    }

    ConfigControl &control = configControls[controlIndex];
    if (!control.editor || control.key.empty()) {
        return;
    }

    const char *value = lv_textarea_get_text(control.editor);
    if (applyConfigValue(control.key, value) && control.valueLabel) {
        lv_label_set_text(control.valueLabel, value);
    }
}

void SensorVisualizationGui::handleBackButtonClick(){
    if(recording){
        handleStillRecording();
        return;
    }
    switchToWiki();
}

void SensorVisualizationGui::handlePauseButtonClick()
{
    paused = !paused;
    sensorManager.setRunning(!paused);
    if (paused)
    {
        lv_obj_set_style_bg_color(ui_btnPause, lv_color_hex(0xE55858), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(ui_btnSync, lv_color_hex(0x009BFF), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_add_flag(ui_btnSync, LV_OBJ_FLAG_CLICKABLE);
    }
    else
    {
        lv_obj_set_style_bg_color(ui_btnPause, lv_color_hex(0x009BFF), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(ui_btnSync, lv_color_hex(0x949494), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_clear_flag(ui_btnSync, LV_OBJ_FLAG_CLICKABLE);
    }
}

void SensorVisualizationGui::handleSyncButtonClick()
{
    if (!currentSensor)
        return;

    if (!paused)
        return;

    bool success = syncCurrentSensor();
    //logMessage("Sync button clicked. Sync %s\n", success ? "succeeded" : "failed");
}

void SensorVisualizationGui::handleRecordButtonClick(const char *message)
{
    if (!currentSensor)
        return;

    if (!currentDeviceSupportsRecording())
    {
        showAlert("Recording is available only for numeric sensor signals");
        return;
    }

    //logMessage("Record button clicked. Current recording state: %s\n", recording ? "ON" : "OFF");

    if (recording)
    {
        dataBundleManager.saveRecording();
        lv_obj_set_style_bg_color(ui_btnRecord, lv_color_hex(0x009BFF), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(ui_btnPrev, lv_color_hex(0x009BFF), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(ui_btnNext, lv_color_hex(0x009BFF), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_add_flag(ui_btnPrev, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_flag(ui_btnNext, LV_OBJ_FLAG_CLICKABLE);
    }
    else
    {
        dataBundleManager.startRecording(currentSensor->Type);
        lv_obj_set_style_bg_color(ui_btnRecord, lv_color_hex(0xE55858), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(ui_btnPrev, lv_color_hex(0x949494), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(ui_btnNext, lv_color_hex(0x949494), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_clear_flag(ui_btnPrev, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(ui_btnNext, LV_OBJ_FLAG_CLICKABLE);
    }

    recording = !recording;

    if(!recording){
        showAlert(message ? message : "Record was saved (view settings)");
    }

    updateActionButtonsState();
}

void SensorVisualizationGui::handleClearButtonClick()
{
    if (!recording && !currentDeviceSupportsRecording())
    {
        showAlert("Nothing to clear on control-only devices");
        return;
    }

    static const char *btns[] = {"Yes", ""};
    showShadowOverlay();
    // Clear button has different behavior based on recording state
    char const* message;
    if(recording){
        message = "Are you sure you want to scrape this recording?";
    }
    else{
        message = "Are you sure you want to clear the sensor history?";
    }
    
    // Show confirmation dialog before clearing history
    lv_obj_t *confirmDialog = lv_msgbox_create(lv_scr_act(), "Confirm Clear", message, btns, true);
    lv_obj_set_width(confirmDialog, 250);
    lv_obj_center(confirmDialog);
    lv_obj_move_foreground(confirmDialog);
    lv_obj_add_event_cb(confirmDialog, [](lv_event_t *e)
                        {
        auto self = static_cast<SensorVisualizationGui*>(lv_event_get_user_data(e));
        lv_event_code_t code = lv_event_get_code(e);

        if (code == LV_EVENT_VALUE_CHANGED)
        {
            lv_obj_t *msgbox = lv_event_get_current_target(e);
            const char *btnText = lv_msgbox_get_active_btn_text(msgbox);
            if (btnText && strcmp(btnText, "Yes") == 0)
            {
                if(self->recording){
                    self->dataBundleManager.scrapRecording();

                    self->handleRecordButtonClick("Recording discarded as requested");
                }
                else{
                    // Clear sensor history
                    self->handleClearConfirmButtonClick();
                }
            }
            self->hideShadowOverlay();
            lv_obj_del(msgbox);
        }
        else if (code == LV_EVENT_DELETE)
        {
            self->hideShadowOverlay();
        } }, LV_EVENT_ALL, this);
}

void SensorVisualizationGui::handleClearConfirmButtonClick()
{
    if (currentSensor)
    {
        // Clear sensor internal history
        currentSensor->clearHistory();

        // Clear per-key buffers and set them to zero
        for (auto &v : currentSensor->getValuesKeys())
        {
            clearSensorHistoryBuffer(v);
        }

        if (ui_Chart && ui_Chart_series_V1)
            lv_chart_set_all_value(ui_Chart, ui_Chart_series_V1, 0);
        if (ui_Chart && ui_Chart_series_V2)
            lv_chart_set_all_value(ui_Chart, ui_Chart_series_V2, 0);

        lv_chart_refresh(ui_Chart);

        for (auto &card : signalCards) {
            if (card.valueLabel) {
                lv_label_set_text(card.valueLabel, "0");
            }
        }
    }
}

void SensorVisualizationGui::handleSettingsButtonClick(lv_obj_t *recordGroup, lv_obj_t *btnSettings,lv_obj_t *parentWidget)
{
    if (ui_SettingsOverlay != nullptr)
        return;

    ui_SettingsOverlay = lv_obj_create(lv_scr_act());
    lv_obj_set_size(ui_SettingsOverlay, LV_PCT(100), LV_PCT(100));
    lv_obj_clear_flag(ui_SettingsOverlay, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_opa(ui_SettingsOverlay, LV_OPA_0, 0);
    lv_obj_set_style_border_opa(ui_SettingsOverlay, 0, 0);
    lv_obj_add_flag(ui_SettingsOverlay, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(ui_SettingsOverlay, [](lv_event_t *e)
                        {
        auto * self = static_cast<SensorVisualizationGui*>(lv_event_get_user_data(e));
        self->hideSettingsPanel(); }, LV_EVENT_CLICKED, this);

    ui_SettingsBridgeGroup = lv_obj_create(parentWidget);
    lv_obj_remove_style_all(ui_SettingsBridgeGroup);
    lv_obj_set_width(ui_SettingsBridgeGroup, 250);
    lv_obj_set_height(ui_SettingsBridgeGroup, 225);
    lv_obj_set_x(ui_SettingsBridgeGroup, -7);
    lv_obj_set_y(ui_SettingsBridgeGroup, 25);
    lv_obj_set_align(ui_SettingsBridgeGroup, LV_ALIGN_TOP_RIGHT);
    lv_obj_clear_flag(ui_SettingsBridgeGroup, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_CLICKABLE);

    ui_SettingsBridge = lv_obj_create(ui_SettingsBridgeGroup);
    lv_obj_remove_style_all(ui_SettingsBridge);
    lv_obj_set_width(ui_SettingsBridge, 50);
    lv_obj_set_height(ui_SettingsBridge, 50);
    lv_obj_set_align(ui_SettingsBridge, LV_ALIGN_TOP_RIGHT);
    lv_obj_clear_flag(ui_SettingsBridge, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(ui_SettingsBridge, lv_color_hex(0x009BFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_SettingsBridge, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_SettingsBridgeFill = lv_obj_create(ui_SettingsBridgeGroup);
    lv_obj_remove_style_all(ui_SettingsBridgeFill);
    lv_obj_set_width(ui_SettingsBridgeFill, 50);
    lv_obj_set_height(ui_SettingsBridgeFill, 50);
    lv_obj_set_x(ui_SettingsBridgeFill, -37);
    lv_obj_set_y(ui_SettingsBridgeFill, -25);
    lv_obj_set_align(ui_SettingsBridgeFill, LV_ALIGN_TOP_RIGHT);
    lv_obj_clear_flag(ui_SettingsBridgeFill, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_radius(ui_SettingsBridgeFill, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_SettingsBridgeFill, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_SettingsBridgeFill, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    // move record group and btn settings to front
    lv_obj_move_foreground(recordGroup);
    lv_obj_move_foreground(btnSettings);

    ui_SettingsGroup = lv_obj_create(ui_SettingsOverlay);
    lv_obj_remove_style_all(ui_SettingsGroup);
    lv_obj_set_width(ui_SettingsGroup, 250);
    lv_obj_set_height(ui_SettingsGroup, 225);
    lv_obj_set_x(ui_SettingsGroup, -7);
    lv_obj_set_y(ui_SettingsGroup, 25);
    lv_obj_set_align(ui_SettingsGroup, LV_ALIGN_TOP_RIGHT);
    //Settings group itself is clickable to prevent click events from propagating to the overlay
    lv_obj_add_flag(ui_SettingsGroup, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(ui_SettingsGroup, LV_OBJ_FLAG_SCROLLABLE);

    ui_SettingsOutlay = lv_obj_create(ui_SettingsGroup);
    lv_obj_remove_style_all(ui_SettingsOutlay);
    lv_obj_set_width(ui_SettingsOutlay, 250);
    lv_obj_set_height(ui_SettingsOutlay, 200);
    lv_obj_set_align(ui_SettingsOutlay, LV_ALIGN_BOTTOM_MID);
    lv_obj_clear_flag(ui_SettingsOutlay, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_radius(ui_SettingsOutlay, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_SettingsOutlay, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_SettingsOutlay, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_SettingsOutlay, lv_color_hex(0x009BFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_SettingsOutlay, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_SettingsOutlay, 3, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_SettingsHeaderLine = lv_obj_create(ui_SettingsGroup);
    lv_obj_remove_style_all(ui_SettingsHeaderLine);
    lv_obj_set_width(ui_SettingsHeaderLine, 230);
    lv_obj_set_height(ui_SettingsHeaderLine, 1);
    lv_obj_set_x(ui_SettingsHeaderLine, 0);
    lv_obj_set_y(ui_SettingsHeaderLine, 58);
    lv_obj_set_align(ui_SettingsHeaderLine, LV_ALIGN_TOP_MID);
    lv_obj_clear_flag(ui_SettingsHeaderLine, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(ui_SettingsHeaderLine, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_SettingsHeaderLine, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_SettingsHeaderLabel = lv_label_create(ui_SettingsGroup);
    lv_obj_set_width(ui_SettingsHeaderLabel, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_SettingsHeaderLabel, LV_SIZE_CONTENT);
    lv_obj_set_x(ui_SettingsHeaderLabel, 10);
    lv_obj_set_y(ui_SettingsHeaderLabel, 35);
    lv_label_set_text(ui_SettingsHeaderLabel, "Settings");
    lv_obj_set_style_text_color(ui_SettingsHeaderLabel, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_SettingsHeaderLabel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_SettingsDataBundleLabel = lv_label_create(ui_SettingsGroup);
    lv_obj_set_width(ui_SettingsDataBundleLabel, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_SettingsDataBundleLabel, LV_SIZE_CONTENT);
    lv_obj_set_x(ui_SettingsDataBundleLabel, 10);
    lv_obj_set_y(ui_SettingsDataBundleLabel, -40);
    lv_obj_set_align(ui_SettingsDataBundleLabel, LV_ALIGN_LEFT_MID);
    lv_label_set_text(ui_SettingsDataBundleLabel, "Data bundle:");
    lv_obj_set_style_text_color(ui_SettingsDataBundleLabel, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_SettingsDataBundleCountLabel = lv_label_create(ui_SettingsGroup);
    lv_obj_set_width(ui_SettingsDataBundleCountLabel, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_SettingsDataBundleCountLabel, LV_SIZE_CONTENT);
    lv_obj_set_x(ui_SettingsDataBundleCountLabel, -20);  
    lv_obj_set_y(ui_SettingsDataBundleCountLabel, -40);
    lv_obj_set_align(ui_SettingsDataBundleCountLabel, LV_ALIGN_RIGHT_MID);
    std::string bundleAmount = "[" + std::to_string(dataBundleManager.getDataBundleAmount()) + "/30]";
    lv_label_set_text(ui_SettingsDataBundleCountLabel, bundleAmount.c_str());
    if(dataBundleManager.isDataBundleFull()){
        lv_obj_set_style_text_color(ui_SettingsDataBundleCountLabel, lv_color_hex(0xE55858), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    else{
        lv_obj_set_style_text_color(ui_SettingsDataBundleCountLabel, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    }

    ui_SettingsDataBundleShowButton = lv_btn_create(ui_SettingsGroup);
    lv_obj_set_width(ui_SettingsDataBundleShowButton, 200);
    lv_obj_set_height(ui_SettingsDataBundleShowButton, 20);
    lv_obj_set_x(ui_SettingsDataBundleShowButton, 17);
    lv_obj_set_y(ui_SettingsDataBundleShowButton, -17);
    lv_obj_set_align(ui_SettingsDataBundleShowButton, LV_ALIGN_LEFT_MID);
    lv_obj_add_flag(ui_SettingsDataBundleShowButton, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    lv_obj_clear_flag(ui_SettingsDataBundleShowButton, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_radius(ui_SettingsDataBundleShowButton, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(ui_SettingsDataBundleShowButton, [](lv_event_t *e)
                        {
        auto * self = static_cast<SensorVisualizationGui*>(lv_event_get_user_data(e));
        self->handleDataBundleShowButtonClick(); }, LV_EVENT_CLICKED, this);

    ui_SettingsDataBundleShowButtonLabel = lv_label_create(ui_SettingsDataBundleShowButton);
    lv_obj_set_width(ui_SettingsDataBundleShowButtonLabel, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_SettingsDataBundleShowButtonLabel, LV_SIZE_CONTENT);
    lv_obj_set_align(ui_SettingsDataBundleShowButtonLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_SettingsDataBundleShowButtonLabel, "Show Data Bundles");

    ui_SettingsDataBundleDeleteAllButton = lv_btn_create(ui_SettingsGroup);
    lv_obj_set_width(ui_SettingsDataBundleDeleteAllButton, 200);
    lv_obj_set_height(ui_SettingsDataBundleDeleteAllButton, 20);
    lv_obj_set_x(ui_SettingsDataBundleDeleteAllButton, 17);
    lv_obj_set_y(ui_SettingsDataBundleDeleteAllButton, 10);
    lv_obj_set_align(ui_SettingsDataBundleDeleteAllButton, LV_ALIGN_LEFT_MID);
    lv_obj_add_flag(ui_SettingsDataBundleDeleteAllButton, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    lv_obj_clear_flag(ui_SettingsDataBundleDeleteAllButton, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_radius(ui_SettingsDataBundleDeleteAllButton, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_SettingsDataBundleDeleteAllButton, lv_color_hex(0xE55858), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_SettingsDataBundleDeleteAllButton, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(ui_SettingsDataBundleDeleteAllButton, [](lv_event_t *e)
                        {
        auto * self = static_cast<SensorVisualizationGui*>(lv_event_get_user_data(e));
        self->handleDataBundleDeleteAllButtonClick(); }, LV_EVENT_CLICKED, this);

    ui_SettingsDataBundleDeleteAllButtonLabel = lv_label_create(ui_SettingsDataBundleDeleteAllButton);
    lv_obj_set_width(ui_SettingsDataBundleDeleteAllButtonLabel, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_SettingsDataBundleDeleteAllButtonLabel, LV_SIZE_CONTENT);
    lv_obj_set_align(ui_SettingsDataBundleDeleteAllButtonLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_SettingsDataBundleDeleteAllButtonLabel, "Delete All Data Bundles!!");

    ui_SettingsCreditsLabel = lv_label_create(ui_SettingsGroup);
    lv_obj_set_width(ui_SettingsCreditsLabel, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_SettingsCreditsLabel, LV_SIZE_CONTENT);
    lv_obj_set_x(ui_SettingsCreditsLabel, 10);
    lv_obj_set_y(ui_SettingsCreditsLabel, 37);
    lv_obj_set_align(ui_SettingsCreditsLabel, LV_ALIGN_LEFT_MID);
    lv_label_set_text(ui_SettingsCreditsLabel, "About Icons:");

    ui_SettingsCreditsButton = lv_btn_create(ui_SettingsGroup);
    lv_obj_set_width(ui_SettingsCreditsButton, 200);
    lv_obj_set_height(ui_SettingsCreditsButton, 20);
    lv_obj_set_x(ui_SettingsCreditsButton, 17);
    lv_obj_set_y(ui_SettingsCreditsButton, 60);
    lv_obj_set_align(ui_SettingsCreditsButton, LV_ALIGN_LEFT_MID);
    lv_obj_set_style_radius(ui_SettingsCreditsButton, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(ui_SettingsCreditsButton, [](lv_event_t *e)
                        {
        auto * self = static_cast<SensorVisualizationGui*>(lv_event_get_user_data(e));
        self->handleCreditsButtonClick(); }, LV_EVENT_CLICKED, this);

    ui_SettingsCreditsButtonLabel = lv_label_create(ui_SettingsCreditsButton);
    lv_obj_set_width(ui_SettingsCreditsButtonLabel, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_SettingsCreditsButtonLabel, LV_SIZE_CONTENT);
    lv_obj_set_align(ui_SettingsCreditsButtonLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_SettingsCreditsButtonLabel, "View About Icons");
}

void SensorVisualizationGui::handleDataBundleShowButtonClick(){
    if(recording){
        handleStillRecording();
        return;
    }
    hideSettingsPanel();
    
    switchToDataBundleSelection();
}

void SensorVisualizationGui::handleDataBundleDeleteAllButtonClick(){
    static const char *btns[] = {"Yes", ""};
    showShadowOverlay();
    // Clear button has different behavior based on recording state
    char const* message = "Are you sure you want DELETE ALL BUNDLES?";

    // Show confirmation dialog before clearing history
    lv_obj_t *confirmDialog = lv_msgbox_create(lv_scr_act(), "Confirm Clear (Bundles)", message, btns, true);
    lv_obj_set_width(confirmDialog, 250);
    lv_obj_center(confirmDialog);
    lv_obj_move_foreground(confirmDialog);
    lv_obj_add_event_cb(confirmDialog, [](lv_event_t *e)
                        {
        auto self = static_cast<SensorVisualizationGui*>(lv_event_get_user_data(e));
        lv_event_code_t code = lv_event_get_code(e);

        if (code == LV_EVENT_VALUE_CHANGED)
        {
            lv_obj_t *msgbox = lv_event_get_current_target(e);
            const char *btnText = lv_msgbox_get_active_btn_text(msgbox);
            if (btnText && strcmp(btnText, "Yes") == 0)
            {
                if(self->recording){
                    self->handleStillRecording();
                    return;
                }
                self->hideSettingsPanel();

                self->dataBundleManager.deleteAllDataBundles();
            }
            self->hideShadowOverlay();
            lv_obj_del(msgbox);
        }
        else if (code == LV_EVENT_DELETE)
        {
            self->hideShadowOverlay();
        } }, LV_EVENT_ALL, this);
}

void SensorVisualizationGui::handleCreditsButtonClick(){
    if(recording){
        handleStillRecording();
        return;
    }
    hideSettingsPanel();
    switchToCreditsScreen();
}

void SensorVisualizationGui::handleStillRecording(){
    if(!recording) return;

        static const char *btns[] = {"Save", "Discard", ""};
    showShadowOverlay();
    // Clear button has different behavior based on recording state
    char const* message = "You are currently recording. Do you want to stop recording?";
    
    // Show confirmation dialog before clearing history
    lv_obj_t *confirmDialog = lv_msgbox_create(lv_scr_act(), "Confirm Clear", message, btns, true);
    lv_obj_set_width(confirmDialog, 250);
    lv_obj_center(confirmDialog);
    lv_obj_move_foreground(confirmDialog);
    lv_obj_add_event_cb(confirmDialog, [](lv_event_t *e)
                        {
        auto self = static_cast<SensorVisualizationGui*>(lv_event_get_user_data(e));
        lv_event_code_t code = lv_event_get_code(e);

        if (code == LV_EVENT_VALUE_CHANGED)
        {
            lv_obj_t *msgbox = lv_event_get_current_target(e);
            const char *btnText = lv_msgbox_get_active_btn_text(msgbox);
            if (btnText && strcmp(btnText, "Save") == 0)
            {
                self->handleRecordButtonClick(nullptr);

                self->dataBundleManager.saveRecording();
            }
            else if (btnText && strcmp(btnText, "Discard") == 0)
            {
                self->handleRecordButtonClick("Recording discarded as requested");
                
                self->dataBundleManager.scrapRecording();
            }
            self->hideShadowOverlay();
            lv_obj_del(msgbox);
        }
        else if (code == LV_EVENT_DELETE)
        {
            self->hideShadowOverlay();
        } }, LV_EVENT_ALL, this);
}

void SensorVisualizationGui::hideSettingsPanel()
{
    if (ui_SettingsOverlay != nullptr)
    {
        lv_obj_del(ui_SettingsOverlay);
        ui_SettingsOverlay = nullptr;
    }

    if(ui_SettingsBridgeGroup != nullptr)
    {
        lv_obj_del(ui_SettingsBridgeGroup);
        ui_SettingsBridgeGroup = nullptr;
    }
}

void SensorVisualizationGui::goToPreviousSensor()
{
    if (recording)
        return;

    const bool wasRunning = sensorManager.isRunning();
    sensorManager.setRunning(false); // Pause any ongoing sensor updates
    currentSensor = sensorManager.previousSensor();
    if (currentSensor) {
        currentSensor->setRedrawPending(true);
    }
    delay_ms(10);                   // Small delay to ensure UI responsiveness
    sensorManager.setRunning(wasRunning); // Resume previous state
}

void SensorVisualizationGui::goToNextSensor()
{
    if (recording)
        return;

    const bool wasRunning = sensorManager.isRunning();
    sensorManager.setRunning(false); // Pause any ongoing sensor updates
    currentSensor = sensorManager.nextSensor();
    if (currentSensor) {
        currentSensor->setRedrawPending(true);
    }
    delay_ms(10);                   // Small delay to ensure UI responsiveness
    sensorManager.setRunning(wasRunning); // Resume previous state
}

void SensorVisualizationGui::goToFirstSensor()
{
    const bool wasRunning = sensorManager.isRunning();
    sensorManager.setRunning(false); // Pause any ongoing sensor updates
    sensorManager.resetCurrentIndex();
    currentSensor = sensorManager.getCurrentSensor();
    if (currentSensor) {
        currentSensor->setRedrawPending(true);
    }
    delay_ms(10);                   // Small delay to ensure UI responsiveness
    sensorManager.setRunning(wasRunning); // Resume previous state
}

bool SensorVisualizationGui::syncCurrentSensor()
{
    if (!currentSensor)
    {
        return false;
    }

    if (!sensorManager.ensureProtocolInitialized()) {
        showAlert("Protocol init failed");
        return false;
    }

    const bool success = syncSensor(currentSensor);
    if (!success) {
        showAlert(currentSensor->getError().empty() ? "Sync failed" : currentSensor->getError().c_str());
        return false;
    }

    currentSensor->setRedrawPending(true);
    updateSensorDataDisplay();
    updateChart();
    return true;
}

void SensorVisualizationGui::showVisualization()
{
    if (!initialized || !ui_SensorWidget)
        return;

    lv_obj_clear_flag(ui_SensorWidget, LV_OBJ_FLAG_HIDDEN);

    // Refresh the display with current sensor data
    goToFirstSensor();
    drawCurrentSensor();
    // logMessage("Showing sensor visualization\n");
}

void SensorVisualizationGui::hideVisualization()
{
    if (!initialized || !ui_SensorWidget)
        return;

    lv_obj_add_flag(ui_SensorWidget, LV_OBJ_FLAG_HIDDEN);
    // logMessage("Hiding sensor visualization\n");
}

void SensorVisualizationGui::showAlert(const char *message){
    if(message == nullptr || !initialized || !ui_SensorWidget) 
        return;
    
    ui_Alert = lv_obj_create(ui_SensorWidget);
    lv_obj_remove_style_all(ui_Alert);
    lv_obj_set_width(ui_Alert, 400);
    lv_obj_set_height(ui_Alert, 40);
    lv_obj_set_x(ui_Alert, 0);
    lv_obj_set_y(ui_Alert, 10);
    lv_obj_set_align(ui_Alert, LV_ALIGN_TOP_MID);
    lv_obj_clear_flag(ui_Alert, LV_OBJ_FLAG_SCROLLABLE);    
    lv_obj_set_style_radius(ui_Alert, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Alert, lv_color_hex(0x4C9ED3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Alert, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(ui_Alert, [](lv_event_t *e){
        auto * self = static_cast<SensorVisualizationGui*>(lv_event_get_user_data(e));
        self->hideAlert(); }, LV_EVENT_CLICKED, this);

    ui_AlertLabel = lv_label_create(ui_Alert);
    lv_obj_set_width(ui_AlertLabel, LV_SIZE_CONTENT); 
    lv_obj_set_height(ui_AlertLabel, LV_SIZE_CONTENT);   
    lv_obj_set_align(ui_AlertLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_AlertLabel, message);
    lv_obj_set_style_text_font(ui_AlertLabel, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_AlertLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    
    lv_obj_del_delayed(ui_Alert, 3000); // Auto hide after 3 seconds
}

void SensorVisualizationGui::hideAlert(){
    if(ui_Alert == nullptr) 
        return;

    lv_obj_del(ui_Alert);
    ui_Alert = nullptr;
}
