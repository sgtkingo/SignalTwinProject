/**
 * @file signals_chart_panel.cpp
 * @brief Chart panel helper for SignalsVisualizationGui.
 */

#include "signals_chart_panel.hpp"

void SignalsChartPanel::create(lv_obj_t *parent)
{
    chart = lv_chart_create(parent);
    lv_obj_set_width(chart, 410);
    lv_obj_set_height(chart, 280);
    lv_obj_set_x(chart, 150);
    lv_obj_set_y(chart, 20);
    lv_obj_set_align(chart, LV_ALIGN_CENTER);
    lv_obj_clear_flag(chart, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE |
                                LV_OBJ_FLAG_GESTURE_BUBBLE | LV_OBJ_FLAG_SNAPPABLE);
    lv_chart_set_type(chart, LV_CHART_TYPE_LINE);
    lv_chart_set_div_line_count(chart, HISTORY_CAP - 1, HISTORY_CAP);
    lv_chart_set_axis_tick(chart, LV_CHART_AXIS_PRIMARY_X, HISTORY_CAP / 2, 0, HISTORY_CAP, 1, true, 50);
    lv_chart_set_axis_tick(chart, LV_CHART_AXIS_PRIMARY_Y, HISTORY_CAP, 5, 5, 2, true, 50);

    primarySeries = lv_chart_add_series(chart, lv_color_hex(0x009BFF), LV_CHART_AXIS_PRIMARY_Y);
    secondarySeries = lv_chart_add_series(chart, lv_color_hex(0xFF6B35), LV_CHART_AXIS_PRIMARY_Y);

    lv_obj_set_style_bg_color(chart, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(chart, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(chart, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(chart, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(chart, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_color(chart, lv_color_hex(0x000000), LV_PART_TICKS | LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(chart, 255, LV_PART_TICKS | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(chart, lv_color_hex(0x000000), LV_PART_TICKS | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(chart, 255, LV_PART_TICKS | LV_STATE_DEFAULT);

    emptyLabel = lv_label_create(chart);
    lv_label_set_text(emptyLabel, "No numeric signal available");
    lv_obj_center(emptyLabel);
    lv_obj_set_style_text_color(emptyLabel, lv_color_hex(0x5F6B7A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_flag(emptyLabel, LV_OBJ_FLAG_HIDDEN);
}

void SignalsChartPanel::showEmptyState(const char *message)
{
    if (!isReady()) {
        return;
    }

    clearSeries();
    if (emptyLabel) {
        lv_label_set_text(emptyLabel, message ? message : "");
        lv_obj_clear_flag(emptyLabel, LV_OBJ_FLAG_HIDDEN);
    }
    refresh();
}

void SignalsChartPanel::hideEmptyState()
{
    if (emptyLabel) {
        lv_obj_add_flag(emptyLabel, LV_OBJ_FLAG_HIDDEN);
    }
}

void SignalsChartPanel::setRange(lv_coord_t minValue, lv_coord_t maxValue)
{
    if (!chart) {
        return;
    }

    lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, minValue, maxValue);
    lv_chart_set_range(chart, LV_CHART_AXIS_SECONDARY_Y, minValue, maxValue);
}

void SignalsChartPanel::clearSeries()
{
    if (!isReady()) {
        return;
    }

    lv_chart_set_all_value(chart, primarySeries, LV_CHART_POINT_NONE);
    lv_chart_set_all_value(chart, secondarySeries, LV_CHART_POINT_NONE);
}

void SignalsChartPanel::populatePrimarySeries(const lv_coord_t *history)
{
    if (!isReady() || !history) {
        return;
    }

    for (int i = 0; i < HISTORY_CAP; ++i) {
        lv_chart_set_next_value(chart, primarySeries, history[i]);
    }
}

void SignalsChartPanel::populateSecondarySeries(const lv_coord_t *history)
{
    if (!isReady() || !history) {
        return;
    }

    for (int i = 0; i < HISTORY_CAP; ++i) {
        lv_chart_set_next_value(chart, secondarySeries, history[i]);
    }
}

void SignalsChartPanel::refresh()
{
    if (chart) {
        lv_chart_refresh(chart);
    }
}

void SignalsChartPanel::resetToZero()
{
    if (!isReady()) {
        return;
    }

    lv_chart_set_all_value(chart, primarySeries, 0);
    lv_chart_set_all_value(chart, secondarySeries, 0);
    refresh();
}
