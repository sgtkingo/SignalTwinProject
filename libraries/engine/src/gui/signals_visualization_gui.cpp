/**
 * @file signals_visualization_gui.cpp
 * @brief Implementation of the SignalsVisualizationGui class
 *
 * This source file implements the SignalsVisualizationGui functionality for
 * active device visualization, data display, and navigation.
 *
 * @copyright 2025 MTA
 * @author Ing. Jiri Konecny, Ondřej Wrubel
 */

#include "signals_visualization_gui.hpp"
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

SignalsVisualizationGui::SignalsVisualizationGui(GuiRouter &router, DeviceManager &deviceManager, DeviceVisualizationSession &visualizationSession, DataBundleManager &dataBundleManager)
                                              : router(router), deviceManager(deviceManager), visualizationSession(visualizationSession), dataBundleManager(dataBundleManager)
{
    // Initialize all GUI pointers to nullptr
    ui_SensorWidget = nullptr;
    ui_SensorLabel = nullptr;
    ui_LogoGroup = nullptr;
    ui_LogoCornerBottomLeft = nullptr;
    ui_LogoCornerFillBottomLeft = nullptr;
    ui_LogoCornerTopRight = nullptr;
    ui_LogoCornerFillBottomRight = nullptr;
    ui_LogoOutlay = nullptr;
    ui_LogoImage = nullptr;
}

void SignalsVisualizationGui::init()
{
    if (initialized)
        return;

    try
    {
        // // logMessage("Initializing SignalsVisualizationGui...\n");
        constructVisualization();
        initialized = true;
        // // logMessage("SignalsVisualizationGui initialization completed!\n");
    }
    catch (const std::exception &e)
    {
        // // logMessage("SignalsVisualizationGui initialization failed: %s\n", e.what());
        initialized = false;
    }
}

void SignalsVisualizationGui::createMainWidget()
{
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
}

void SignalsVisualizationGui::createTitleLabel()
{
    ui_SensorLabel = lv_label_create(ui_SensorWidget);
    lv_obj_set_width(ui_SensorLabel, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_SensorLabel, LV_SIZE_CONTENT);
    lv_obj_set_x(ui_SensorLabel, 0);
    lv_obj_set_y(ui_SensorLabel, 10);
    lv_obj_set_align(ui_SensorLabel, LV_ALIGN_TOP_MID);
    lv_label_set_text(ui_SensorLabel, "DHT11");
    lv_obj_clear_flag(ui_SensorLabel, LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE |
                                          LV_OBJ_FLAG_SNAPPABLE | LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                                          LV_OBJ_FLAG_SCROLL_CHAIN);
    lv_obj_set_style_text_color(ui_SensorLabel, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_SensorLabel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_SensorLabel, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
}

void SignalsVisualizationGui::createSignalScrollPanel()
{
    signalListPanel.create(
        ui_SensorWidget,
        this,
        [](lv_event_t *e) {
            if (lv_event_get_code(e) != LV_EVENT_VALUE_CHANGED) {
                return;
            }

            auto *self = static_cast<SignalsVisualizationGui *>(lv_event_get_user_data(e));
            const int index = static_cast<int>(reinterpret_cast<intptr_t>(lv_obj_get_user_data(lv_event_get_target(e))));
            self->handleDropdownConfigChanged(static_cast<size_t>(index));
        },
        [](lv_event_t *e) {
            if (lv_event_get_code(e) != LV_EVENT_VALUE_CHANGED) {
                return;
            }

            auto *self = static_cast<SignalsVisualizationGui *>(lv_event_get_user_data(e));
            const int index = static_cast<int>(reinterpret_cast<intptr_t>(lv_obj_get_user_data(lv_event_get_target(e))));
            self->handleSliderConfigChanged(static_cast<size_t>(index));
        },
        [](lv_event_t *e) {
            if (lv_event_get_code(e) != LV_EVENT_READY && lv_event_get_code(e) != LV_EVENT_DEFOCUSED) {
                return;
            }

            auto *self = static_cast<SignalsVisualizationGui *>(lv_event_get_user_data(e));
            const int index = static_cast<int>(reinterpret_cast<intptr_t>(lv_obj_get_user_data(lv_event_get_target(e))));
            self->handleTextConfigSubmitted(static_cast<size_t>(index));
        });
}

void SignalsVisualizationGui::createChartPanel()
{
    chartPanel.create(ui_SensorWidget);
}

void SignalsVisualizationGui::constructVisualization()
{
    // // logMessage("\t>constructing sensor visualization...\n");

    createMainWidget();
    createTitleLabel();
    createSignalScrollPanel();
    createChartPanel();
    createToolbarPanel();
    addLogoPanelToWidget(ui_SensorWidget);

    // // logMessage("\t>sensor visualization constructed!\n");
}

void SignalsVisualizationGui::createToolbarPanel()
{
    toolbarPanel.create(
        ui_SensorWidget,
        this,
        [](lv_event_t *e) {
            auto *self = static_cast<SignalsVisualizationGui *>(lv_event_get_user_data(e));
            self->goToPreviousDevice();
        },
        [](lv_event_t *e) {
            auto *self = static_cast<SignalsVisualizationGui *>(lv_event_get_user_data(e));
            self->goToNextDevice();
        },
        [](lv_event_t *e) {
            auto *self = static_cast<SignalsVisualizationGui *>(lv_event_get_user_data(e));
            self->handleBackButtonClick();
        },
        [](lv_event_t *e) {
            auto *self = static_cast<SignalsVisualizationGui *>(lv_event_get_user_data(e));
            self->handlePauseButtonClick();
        },
        [](lv_event_t *e) {
            auto *self = static_cast<SignalsVisualizationGui *>(lv_event_get_user_data(e));
            self->handleSyncButtonClick();
        },
        [](lv_event_t *e) {
            auto *self = static_cast<SignalsVisualizationGui *>(lv_event_get_user_data(e));
            self->handleRecordButtonClick(nullptr);
        },
        [](lv_event_t *e) {
            auto *self = static_cast<SignalsVisualizationGui *>(lv_event_get_user_data(e));
            self->handleClearButtonClick();
        },
        [](lv_event_t *e) {
            auto *self = static_cast<SignalsVisualizationGui *>(lv_event_get_user_data(e));
            self->handleSettingsButtonClick(self->toolbarPanel.getRecordGroup(), self->toolbarPanel.getSettingsButton(), self->getParentWidget());
        });
}

void SignalsVisualizationGui::showShadowOverlay()
{
    feedbackPanel.showShadowOverlay();
}

void SignalsVisualizationGui::hideShadowOverlay()
{
    feedbackPanel.hideShadowOverlay();
}

void SignalsVisualizationGui::addLogoPanelToWidget(lv_obj_t *parentWidget)
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

void SignalsVisualizationGui::drawCurrentDevice()
{
    if (!currentDevice)
    {
        // // logMessage("No current device to draw\n");
        return;
    }

    // // logMessage("Drawing device: %s\n", currentDevice->UID.c_str());
    if (!currentDevice->getRedrawPending())
    {
        return;
    }

    if (paused)
    {
        return;
    }

    updateDeviceDataDisplay();
    updateChart();
}

uint32_t SignalsVisualizationGui::getSignalAccentColor(size_t index)
{
    return SIGNAL_CARD_COLORS[index % (sizeof(SIGNAL_CARD_COLORS) / sizeof(SIGNAL_CARD_COLORS[0]))];
}

bool SignalsVisualizationGui::isNumericType(DeviceDataType dtype)
{
    return dtype == DeviceDataType::INT || dtype == DeviceDataType::FLOAT || dtype == DeviceDataType::DOUBLE;
}

bool SignalsVisualizationGui::hasSelectableOptions(const DeviceParam &param)
{
    return !param.Restrictions.Options.empty();
}

bool SignalsVisualizationGui::supportsSliderInput(const DeviceParam &param)
{
    return param.DType == DeviceDataType::INT &&
           !param.Restrictions.Min.empty() &&
           !param.Restrictions.Max.empty();
}

std::string SignalsVisualizationGui::buildUnitText(const std::string &unit, const char *fallbackText)
{
    if (unit.empty()) {
        return fallbackText ? std::string(fallbackText) : std::string();
    }

    return "[" + unit + "]";
}

bool SignalsVisualizationGui::currentDeviceSupportsRecording() const
{
    return currentDevice &&
           currentDevice->getRole() != DeviceRole::ACTUATOR &&
           !getChartableValueKeys().empty();
}

void SignalsVisualizationGui::ensureSignalCards(size_t count)
{
    signalListPanel.ensureSignalCards(count);
}

void SignalsVisualizationGui::clearUnusedSignalCards(size_t usedCount)
{
    signalListPanel.clearUnusedSignalCards(usedCount);
}

void SignalsVisualizationGui::ensureConfigControls(size_t count)
{
    signalListPanel.ensureConfigControls(count);
}

void SignalsVisualizationGui::clearUnusedConfigControls(size_t usedCount)
{
    signalListPanel.clearUnusedConfigControls(usedCount);
}

void SignalsVisualizationGui::updateDeviceTitle()
{
    if (!currentDevice || !ui_SensorLabel) {
        return;
    }

    const std::string title = currentDevice->getName() + " [" + currentDevice->getRoleLabel() + "]";
    lv_label_set_text(ui_SensorLabel, title.c_str());
}

void SignalsVisualizationGui::updateSignalCards(const std::unordered_map<std::string, DeviceParam> &values,
                                                const std::vector<std::string> &valueKeys,
                                                bool useValueControls)
{
    if (useValueControls) {
        clearUnusedSignalCards(0);
        return;
    }

    ensureSignalCards(valueKeys.size());

    for (size_t i = 0; i < valueKeys.size(); ++i) {
        const auto &key = valueKeys[i];
        auto it = values.find(key);
        if (it == values.end()) {
            continue;
        }

        const std::string units = buildUnitText(currentDevice->getValueUnits(key), "Live value");
        signalListPanel.setSignalCard(i, getSignalAccentColor(i), key, it->second.Value, units);
    }

    clearUnusedSignalCards(valueKeys.size());
}

void SignalsVisualizationGui::ensureControlEditor(size_t controlIndex, const DeviceParam &param)
{
    auto *control = signalListPanel.getConfigControl(controlIndex);
    if (!control) {
        return;
    }

    if (hasSelectableOptions(param)) {
        std::string optionsText;
        int selectedIndex = 0;
        int currentIndex = 0;
        for (const auto &option : splitOptionsCsv(param.Restrictions.Options)) {
            if (!optionsText.empty()) {
                optionsText += "\n";
            }
            optionsText += option;
            if (option == param.Value) {
                selectedIndex = currentIndex;
            }
            ++currentIndex;
        }
        signalListPanel.ensureDropdownEditor(controlIndex, optionsText, static_cast<uint16_t>(selectedIndex));
    } else if (supportsSliderInput(param)) {
        signalListPanel.ensureSliderEditor(
            controlIndex,
            convertStringToType<int>(param.Restrictions.Min),
            convertStringToType<int>(param.Restrictions.Max),
            convertStringToType<int>(param.Value));
    } else {
        signalListPanel.ensureTextEditor(controlIndex, param.Value);
    }
}

void SignalsVisualizationGui::syncControlEditorValue(size_t controlIndex, const DeviceParam &param)
{
    ensureControlEditor(controlIndex, param);
}

void SignalsVisualizationGui::updateEditableControls(const std::unordered_map<std::string, DeviceParam> &values,
                                                     const std::vector<std::string> &valueKeys,
                                                     const std::unordered_map<std::string, DeviceParam> &configs,
                                                     const std::vector<std::string> &configKeys,
                                                     bool useValueControls)
{
    const auto &editableKeys = useValueControls ? valueKeys : configKeys;
    ensureConfigControls(editableKeys.size());

    for (size_t i = 0; i < editableKeys.size(); ++i) {
        const auto &key = editableKeys[i];
        const auto editableIt = useValueControls ? values.find(key) : configs.find(key);
        if (editableIt == (useValueControls ? values.end() : configs.end()) || i >= signalListPanel.getConfigControlCount()) {
            continue;
        }

        const DeviceParam &param = editableIt->second;
        const uint32_t accentColor = getSignalAccentColor(i + (useValueControls ? 0 : valueKeys.size()));
        signalListPanel.setControlVisual(
            i,
            accentColor,
            key,
            param.Value,
            buildUnitText(
                useValueControls ? currentDevice->getValueUnits(key) : currentDevice->getConfigUnits(key),
                useValueControls ? "Queued via CONTROL" : "Queued via CONFIG"),
            key,
            useValueControls);

        ensureControlEditor(i, param);
        syncControlEditorValue(i, param);
    }

    clearUnusedConfigControls(editableKeys.size());
}

bool SignalsVisualizationGui::buildNumericHistoryForKey(const std::string &key, lv_coord_t *history)
{
    if (!currentDevice || !history) {
        return false;
    }

    auto values = currentDevice->getValues();
    auto it = values.find(key);
    if (it == values.end()) {
        return false;
    }

    switch (it->second.DType)
    {
    case DeviceDataType::INT:
        buildDeviceHistory<int>(currentDevice, key, history);
        return true;
    case DeviceDataType::FLOAT:
        buildDeviceHistory<float>(currentDevice, key, history);
        return true;
    case DeviceDataType::DOUBLE:
        buildDeviceHistory<double>(currentDevice, key, history);
        return true;
    default:
        return false;
    }
}

std::vector<std::string> SignalsVisualizationGui::getChartableValueKeys() const
{
    std::vector<std::string> chartKeys;
    if (!currentDevice) {
        return chartKeys;
    }

    if (currentDevice->getRole() == DeviceRole::ACTUATOR) {
        return chartKeys;
    }

    const auto values = currentDevice->getValues();
    for (const auto &key : currentDevice->getValuesKeys()) {
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

void SignalsVisualizationGui::showEmptyChartState(const char *message)
{
    chartPanel.showEmptyState(message);
}

void SignalsVisualizationGui::hideEmptyChartState()
{
    chartPanel.hideEmptyState();
}

std::pair<lv_coord_t, lv_coord_t> SignalsVisualizationGui::computeChartRange(const lv_coord_t *history)
{
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
}

void SignalsVisualizationGui::populateChartSeries(lv_chart_series_t *series, const lv_coord_t *history)
{
    if (!series || !history || !chartPanel.getChart()) {
        return;
    }

    for (int i = 0; i < HISTORY_CAP; ++i) {
        lv_chart_set_next_value(chartPanel.getChart(), series, history[i]);
    }
}

bool SignalsVisualizationGui::beginDeviceNavigation(bool requireIdleRecording, bool &wasRunning)
{
    if (requireIdleRecording && recording) {
        return false;
    }

    wasRunning = deviceManager.isRunning();
    deviceManager.setRunning(false);
    return true;
}

void SignalsVisualizationGui::finishDeviceNavigation(bool wasRunning, BaseDevice *nextDevice)
{
    currentDevice = nextDevice;
    if (currentDevice) {
        currentDevice->setRedrawPending(true);
    }

    delay_ms(10);
    deviceManager.setRunning(wasRunning);
}

void SignalsVisualizationGui::updateDeviceDataDisplay()
{
    if (!currentDevice || !signalListPanel.getContainer())
        return;

    const auto values = currentDevice->getValues();
    const auto valueKeys = currentDevice->getValuesKeys();
    const auto configs = currentDevice->getConfigs();
    const auto configKeys = currentDevice->getConfigsKeys();
    const bool useValueControls = currentDevice->getRole() == DeviceRole::ACTUATOR;

    updateDeviceTitle();
    updateSignalCards(values, valueKeys, useValueControls);
    updateEditableControls(values, valueKeys, configs, configKeys, useValueControls);
    updateActionButtonsState();
}

void SignalsVisualizationGui::updateChart()
{
    if (!currentDevice || !chartPanel.isReady())
        return;

    if (deviceManager.isRedrawPending() == false)
        return;

    const auto chartKeys = getChartableValueKeys();
    if (chartKeys.empty())
    {
        showEmptyChartState(currentDevice->getRole() == DeviceRole::ACTUATOR
                                ? "Control-only device"
                                : "No numeric signal available");
        return;
    }

    try
    {
        hideEmptyChartState();

        lv_coord_t historyPrimary[HISTORY_CAP];
        if (!buildNumericHistoryForKey(chartKeys[0], historyPrimary)) {
            return;
        }

        auto primaryRange = computeChartRange(historyPrimary);
        lv_coord_t globalMin = primaryRange.first;
        lv_coord_t globalMax = primaryRange.second;

        bool haveSecond = false;
        lv_coord_t historySecondary[HISTORY_CAP];
        if (chartKeys.size() > 1) {
            haveSecond = buildNumericHistoryForKey(chartKeys[1], historySecondary);
            if (haveSecond) {
                auto secondaryRange = computeChartRange(historySecondary);
                if (secondaryRange.first < globalMin) {
                    globalMin = secondaryRange.first;
                }
                if (secondaryRange.second > globalMax) {
                    globalMax = secondaryRange.second;
                }
            }
        }

        chartPanel.setRange(globalMin, globalMax);
        chartPanel.clearSeries();
        chartPanel.populatePrimarySeries(historyPrimary);

        if (haveSecond) {
            chartPanel.populateSecondarySeries(historySecondary);
        }

        chartPanel.refresh();
    }
    catch (const std::exception &e)
    {
        // logMessage("Error updating chart: %s\n", e.what());
    }
}

void SignalsVisualizationGui::updateActionButtonsState()
{
    toolbarPanel.setRecordingState(currentDeviceSupportsRecording(), recording);
}

bool SignalsVisualizationGui::applyEditableValue(bool isValueControl, const std::string &key, const std::string &value)
{
    if (!currentDevice || key.empty()) {
        return false;
    }

    try {
        if (isValueControl) {
            currentDevice->setValue(key, value);
        } else {
            currentDevice->setConfig(key, value);
        }
        return true;
    } catch (...) {
        showAlert(isValueControl ? "Failed to queue control value" : "Failed to queue config value");
        return false;
    }
}

void SignalsVisualizationGui::handleDropdownConfigChanged(size_t controlIndex)
{
    auto *control = signalListPanel.getConfigControl(controlIndex);
    if (!control || !control->editor || control->key.empty()) {
        return;
    }

    char selected[64] = {0};
    lv_dropdown_get_selected_str(control->editor, selected, sizeof(selected));
    if (applyEditableValue(control->isValueControl, control->key, selected) && control->valueLabel) {
        lv_label_set_text(control->valueLabel, selected);
    }
}

void SignalsVisualizationGui::handleSliderConfigChanged(size_t controlIndex)
{
    auto *control = signalListPanel.getConfigControl(controlIndex);
    if (!control || !control->editor || control->key.empty()) {
        return;
    }

    const std::string value = std::to_string(lv_slider_get_value(control->editor));
    if (applyEditableValue(control->isValueControl, control->key, value) && control->valueLabel) {
        lv_label_set_text(control->valueLabel, value.c_str());
    }
}

void SignalsVisualizationGui::handleTextConfigSubmitted(size_t controlIndex)
{
    auto *control = signalListPanel.getConfigControl(controlIndex);
    if (!control || !control->editor || control->key.empty()) {
        return;
    }

    const char *value = lv_textarea_get_text(control->editor);
    if (applyEditableValue(control->isValueControl, control->key, value) && control->valueLabel) {
        lv_label_set_text(control->valueLabel, value);
    }
}

void SignalsVisualizationGui::handleBackButtonClick(){
    if(recording){
        handleStillRecording();
        return;
    }
    router.showSelection();
}

void SignalsVisualizationGui::handlePauseButtonClick()
{
    paused = !paused;
    deviceManager.setRunning(!paused);
    toolbarPanel.setPaused(paused);
}

void SignalsVisualizationGui::handleSyncButtonClick()
{
    if (!currentDevice)
        return;

    if (!paused)
        return;

    bool success = syncCurrentDevice();
    //logMessage("Sync button clicked. Sync %s\n", success ? "succeeded" : "failed");
}

void SignalsVisualizationGui::handleRecordButtonClick(const char *message)
{
    if (!currentDevice)
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
    }
    else
    {
        dataBundleManager.startRecording(currentDevice->Type);
    }

    recording = !recording;

    if(!recording){
        showAlert(message ? message : "Record was saved (view settings)");
    }

    updateActionButtonsState();
}

void SignalsVisualizationGui::handleClearButtonClick()
{
    if (!recording && !currentDeviceSupportsRecording())
    {
        showAlert("Nothing to clear on control-only devices");
        return;
    }

    static const char *btns[] = {"Yes", ""};
    const char *message = recording
                              ? "Are you sure you want to scrape this recording?"
                              : "Are you sure you want to clear the sensor history?";

    feedbackPanel.showConfirmationDialog(
        "Confirm Clear",
        message,
        btns,
        this,
        [](lv_event_t *e) {
            auto *self = static_cast<SignalsVisualizationGui *>(lv_event_get_user_data(e));
            const lv_event_code_t code = lv_event_get_code(e);

            if (self->feedbackPanel.isConfirmationAccepted(e, "Yes")) {
                if (self->recording) {
                    self->dataBundleManager.scrapRecording();
                    self->handleRecordButtonClick("Recording discarded as requested");
                } else {
                    self->handleClearConfirmButtonClick();
                }
            } else if (code != LV_EVENT_DELETE) {
                return;
            }

            self->feedbackPanel.closeConfirmationDialog(e);
        });
}

void SignalsVisualizationGui::handleClearConfirmButtonClick()
{
    if (currentDevice)
    {
        // Clear device internal history
        currentDevice->clearHistory();

        // Clear per-key buffers and set them to zero
        for (auto &v : currentDevice->getValuesKeys())
        {
            clearDeviceHistoryBuffer(v);
        }

        chartPanel.resetToZero();
        signalListPanel.setAllSignalCardValues("0");
    }
}

void SignalsVisualizationGui::handleSettingsButtonClick(lv_obj_t *recordGroup, lv_obj_t *btnSettings,lv_obj_t *parentWidget)
{
    settingsPanel.show(
        parentWidget,
        recordGroup,
        btnSettings,
        this,
        dataBundleManager.getDataBundleAmount(),
        dataBundleManager.isDataBundleFull(),
        [](lv_event_t *e) {
            auto *self = static_cast<SignalsVisualizationGui *>(lv_event_get_user_data(e));
            self->hideSettingsPanel();
        },
        [](lv_event_t *e) {
            auto *self = static_cast<SignalsVisualizationGui *>(lv_event_get_user_data(e));
            self->handleDataBundleShowButtonClick();
        },
        [](lv_event_t *e) {
            auto *self = static_cast<SignalsVisualizationGui *>(lv_event_get_user_data(e));
            self->handleDataBundleDeleteAllButtonClick();
        },
        [](lv_event_t *e) {
            auto *self = static_cast<SignalsVisualizationGui *>(lv_event_get_user_data(e));
            self->handleCreditsButtonClick();
        });
}

void SignalsVisualizationGui::handleDataBundleShowButtonClick(){
    if(recording){
        handleStillRecording();
        return;
    }
    hideSettingsPanel();
    
    router.openDatabankFromVisualization();
}

void SignalsVisualizationGui::handleDataBundleDeleteAllButtonClick(){
    static const char *btns[] = {"Yes", ""};
    feedbackPanel.showConfirmationDialog(
        "Confirm Clear (Bundles)",
        "Are you sure you want DELETE ALL BUNDLES?",
        btns,
        this,
        [](lv_event_t *e) {
            auto *self = static_cast<SignalsVisualizationGui *>(lv_event_get_user_data(e));
            const lv_event_code_t code = lv_event_get_code(e);

            if (self->feedbackPanel.isConfirmationAccepted(e, "Yes")) {
                self->feedbackPanel.closeConfirmationDialog(e);

                if (self->recording) {
                    self->handleStillRecording();
                    return;
                }

                self->hideSettingsPanel();
                self->dataBundleManager.deleteAllDataBundles();
                return;
            }

            if (code == LV_EVENT_DELETE) {
                self->feedbackPanel.closeConfirmationDialog(e);
            }
        });
}

void SignalsVisualizationGui::handleCreditsButtonClick(){
    if(recording){
        handleStillRecording();
        return;
    }
    hideSettingsPanel();
    router.showCreditsScreen();
}

void SignalsVisualizationGui::handleStillRecording(){
    if(!recording) return;

    static const char *btns[] = {"Save", "Discard", ""};
    feedbackPanel.showConfirmationDialog(
        "Confirm Clear",
        "You are currently recording. Do you want to stop recording?",
        btns,
        this,
        [](lv_event_t *e) {
            auto *self = static_cast<SignalsVisualizationGui *>(lv_event_get_user_data(e));
            const lv_event_code_t code = lv_event_get_code(e);

            if (self->feedbackPanel.isConfirmationAccepted(e, "Save")) {
                self->handleRecordButtonClick(nullptr);
                self->dataBundleManager.saveRecording();
            } else if (self->feedbackPanel.isConfirmationAccepted(e, "Discard")) {
                self->handleRecordButtonClick("Recording discarded as requested");
                self->dataBundleManager.scrapRecording();
            } else if (code != LV_EVENT_DELETE) {
                return;
            }

            self->feedbackPanel.closeConfirmationDialog(e);
        });
}

void SignalsVisualizationGui::hideSettingsPanel()
{
    settingsPanel.hide();
}

void SignalsVisualizationGui::goToPreviousDevice()
{
    bool wasRunning = false;
    if (!beginDeviceNavigation(true, wasRunning))
        return;

    finishDeviceNavigation(wasRunning, visualizationSession.previousDevice());
}

void SignalsVisualizationGui::goToNextDevice()
{
    bool wasRunning = false;
    if (!beginDeviceNavigation(true, wasRunning))
        return;

    finishDeviceNavigation(wasRunning, visualizationSession.nextDevice());
}

void SignalsVisualizationGui::goToFirstDevice()
{
    bool wasRunning = false;
    if (!beginDeviceNavigation(false, wasRunning))
        return;

    visualizationSession.resetCurrentIndex();
    finishDeviceNavigation(wasRunning, visualizationSession.getCurrentDevice());
}

bool SignalsVisualizationGui::syncCurrentDevice()
{
    if (!currentDevice)
    {
        return false;
    }

    if (!deviceManager.ensureProtocolInitialized()) {
        showAlert("Protocol init failed");
        return false;
    }

    const bool success = syncDevice(currentDevice);
    if (!success) {
        showAlert(currentDevice->getError().empty() ? "Sync failed" : currentDevice->getError().c_str());
        return false;
    }

    currentDevice->setRedrawPending(true);
    updateDeviceDataDisplay();
    updateChart();
    return true;
}

void SignalsVisualizationGui::showVisualization()
{
    if (!initialized || !ui_SensorWidget)
        return;

    lv_obj_clear_flag(ui_SensorWidget, LV_OBJ_FLAG_HIDDEN);

    // Refresh the display with current device data
    goToFirstDevice();
    drawCurrentDevice();
    // logMessage("Showing device visualization\n");
}

void SignalsVisualizationGui::hideVisualization()
{
    if (!initialized || !ui_SensorWidget)
        return;

    lv_obj_add_flag(ui_SensorWidget, LV_OBJ_FLAG_HIDDEN);
    // logMessage("Hiding device visualization\n");
}

void SignalsVisualizationGui::showAlert(const char *message){
    if(message == nullptr || !initialized || !ui_SensorWidget) 
        return;

    feedbackPanel.showAlert(
        ui_SensorWidget,
        this,
        message,
        [](lv_event_t *e) {
            auto *self = static_cast<SignalsVisualizationGui *>(lv_event_get_user_data(e));
            self->hideAlert();
        });
}

void SignalsVisualizationGui::hideAlert(){
    feedbackPanel.hideAlert();
}
