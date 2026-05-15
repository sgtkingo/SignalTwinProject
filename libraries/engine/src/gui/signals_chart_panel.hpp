/**
 * @file signals_chart_panel.hpp
 * @brief Chart panel helper for SignalsVisualizationGui.
 */

#ifndef SIGNALS_CHART_PANEL_HPP
#define SIGNALS_CHART_PANEL_HPP

#include <cstddef>
#include "lvgl.h"

#include "../devices/base_device.hpp"

class SignalsChartPanel
{
private:
    lv_obj_t *chart = nullptr;
    lv_obj_t *emptyLabel = nullptr;
    lv_obj_t *scalingLabel = nullptr;
    lv_obj_t *secondaryScalingLabel = nullptr;
    lv_obj_t *samplesLabel = nullptr;
    lv_chart_series_t *primarySeries = nullptr;
    lv_chart_series_t *secondarySeries = nullptr;

public:
    void create(lv_obj_t *parent);

    bool isReady() const { return chart && primarySeries && secondarySeries; }
    lv_obj_t *getChart() const { return chart; }

    void showEmptyState(const char *message);
    void hideEmptyState();
    void setScalingText(const char *primaryText, const char *secondaryText = nullptr);
    void setSamplesText(const char *text);
    void setVisibleSampleCount(int sampleCount);
    void setRange(lv_coord_t primaryMinValue,
                  lv_coord_t primaryMaxValue,
                  lv_coord_t secondaryMinValue,
                  lv_coord_t secondaryMaxValue,
                  bool hasSecondarySeries);
    void clearSeries();
    void populatePrimarySeries(const lv_coord_t *history, int sampleCount);
    void populateSecondarySeries(const lv_coord_t *history, int sampleCount);
    void hideSecondarySeries();
    void refresh();
    void resetToZero();
};

#endif // SIGNALS_CHART_PANEL_HPP
