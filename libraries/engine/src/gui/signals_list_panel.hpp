/**
 * @file signals_list_panel.hpp
 * @brief Scrollable signal/config panel helper for SignalsVisualizationGui.
 */

#ifndef SIGNALS_LIST_PANEL_HPP
#define SIGNALS_LIST_PANEL_HPP

#include <cstddef>
#include <string>
#include <vector>

#include "lvgl.h"

class SignalsListPanel
{
public:
    struct SignalCard
    {
        lv_obj_t *container = nullptr;
        lv_obj_t *accent = nullptr;
        lv_obj_t *nameLabel = nullptr;
        lv_obj_t *valueLabel = nullptr;
        lv_obj_t *unitLabel = nullptr;
    };

    struct ConfigControl
    {
        lv_obj_t *container = nullptr;
        lv_obj_t *accent = nullptr;
        lv_obj_t *nameLabel = nullptr;
        lv_obj_t *valueLabel = nullptr;
        lv_obj_t *unitLabel = nullptr;
        lv_obj_t *editor = nullptr;
        bool usesDropdown = false;
        bool usesSlider = false;
        bool isValueControl = false;
        std::string key;
    };

private:
    lv_obj_t *container = nullptr;
    std::vector<SignalCard> signalCards;
    std::vector<ConfigControl> configControls;

    void *eventUserData = nullptr;
    lv_event_cb_t dropdownChangedCb = nullptr;
    lv_event_cb_t sliderChangedCb = nullptr;
    lv_event_cb_t textSubmittedCb = nullptr;

public:
    void create(lv_obj_t *parent,
                void *eventUserData,
                lv_event_cb_t dropdownChangedCb,
                lv_event_cb_t sliderChangedCb,
                lv_event_cb_t textSubmittedCb);

    lv_obj_t *getContainer() const { return container; }

    void ensureSignalCards(size_t count);
    void clearUnusedSignalCards(size_t usedCount);
    void setSignalCard(size_t index,
                       uint32_t accentColor,
                       const std::string &name,
                       const std::string &value,
                       const std::string &unit);
    void setAllSignalCardValues(const char *value);

    void ensureConfigControls(size_t count);
    void clearUnusedConfigControls(size_t usedCount);
    size_t getConfigControlCount() const { return configControls.size(); }
    ConfigControl *getConfigControl(size_t index);

    void setControlVisual(size_t index,
                          uint32_t accentColor,
                          const std::string &name,
                          const std::string &value,
                          const std::string &unit,
                          const std::string &key,
                          bool isValueControl);
    void ensureDropdownEditor(size_t index, const std::string &optionsText, uint16_t selectedIndex);
    void ensureSliderEditor(size_t index, int minValue, int maxValue, int currentValue);
    void ensureTextEditor(size_t index, const std::string &value);
};

#endif // SIGNALS_LIST_PANEL_HPP
