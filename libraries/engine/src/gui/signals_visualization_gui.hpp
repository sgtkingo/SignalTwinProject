/**
 * @file signals_visualization_gui.hpp
 * @brief Declaration of the SignalsVisualizationGui widget
 *
 * This header defines the SignalsVisualizationGui class which handles
 * active device visualization, data display, synchronization, and navigation.
 *
 * @copyright 2025 MTA
 * @author Ing. Jiri Konecny, Ondřej Wrubel
 */

#ifndef SIGNALS_VISUALIZATION_GUI_HPP
#define SIGNALS_VISUALIZATION_GUI_HPP

#include "lvgl.h"
#include <array>
#include <map>
#include <string>
#include <vector>

#include "gui_callbacks.hpp"
#include "../managers/device_manager.hpp"
#include "../managers/data_bundle_manager.hpp"
#include "../exceptions/data_exceptions.hpp"

/**
 * @class SignalsVisualizationGui
 * @brief Handles active device visualization, data display, and navigation.
 *
 * This class is responsible for:
 * - Displaying current device data and values
 * - Handling device navigation (prev/next)
 * - Managing device synchronization operations
 * - Handling device-specific events and interactions
 */
class SignalsVisualizationGui
{
private:
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

    DeviceManager &deviceManager;        ///< Reference to the device manager instance
    DeviceManager &sensorManager = deviceManager; ///< Transitional alias while internal implementation is still being renamed
    DataBundleManager &dataBundleManager;///< Reference to the databundle manager instance
    BaseDevice *currentDevice = nullptr; ///< Currently visualized device
    BaseDevice *&currentSensor = currentDevice; ///< Transitional alias while internal implementation is still being renamed

    /// Static buffers for chart data
    std::map<std::string, std::array<lv_coord_t, HISTORY_CAP>> bufMap;
    std::map<std::string, bool> initedMap;

    bool initialized = false; ///< Initialization state flag
    bool paused = false;      ///< Pause state flag
    bool recording = false;   ///< Recording state flag

    void createMainWidget();
    void createTitleLabel();
    void createSignalScrollPanel();
    void createChartPanel();

    // --- DEVICE VISUALIZATION MEMBERS ---
    lv_obj_t *ui_SensorWidget; ///< Widget for device visualisation
    lv_obj_t *ui_SensorLabel;  ///< Label for device name

    lv_obj_t *ui_SignalScrollContainer; ///< Scrollable container for dynamic signal cards
    lv_obj_t *ui_ChartEmptyLabel;       ///< Label shown when no numeric signal is available
    std::vector<SignalCard> signalCards;///< Dynamic cards for all device signals
    std::vector<ConfigControl> configControls; ///< Dynamic controls for actuator configs

    // CHART
    lv_obj_t *ui_Chart;                    ///< Chart widget for device data
    lv_chart_series_t *ui_Chart_series_V1; ///< Chart series for value 1
    lv_chart_series_t *ui_Chart_series_V2; ///< Chart series for value 2

    // --- NAVIGATION AND CONTROL MEMBERS ---
    lv_obj_t *ui_btnPrev;                                ///< Previous device button
    lv_obj_t *ui_btnPrevLabel;                           ///< Label for previous button
    lv_obj_t *ui_btnNext;                                ///< Next device button
    lv_obj_t *ui_btnNextLabel;                           ///< Label for next button
    lv_obj_t *ui_btnBackGroup;                           ///< Group container for back button
    lv_obj_t *ui_btnBack;                                ///< Back to menu button
    lv_obj_t *ui_btnBackLabel;                           ///< Label for back button
    lv_obj_t *ui_btnBackCornerBottomLeft;                ///< Decorative corner for back button
    lv_obj_t *ui_btnBackCornerTopRight;                  ///< Decorative corner for back button
    lv_obj_t *ui_RecordGroup;                            ///< Group container for record panel
    lv_obj_t *ui_RecordCornerTopLeft;                    ///< Decorative corner for record panel
    lv_obj_t *ui_RecordCornerFillTopLeft;                ///< Decorative fill for record panel
    lv_obj_t *ui_RecordCornerTopRight;                   ///< Decorative corner for record panel
    lv_obj_t *ui_RecordCornerFillTopRight;               ///< Decorative fill for record panel
    lv_obj_t *ui_RecordCornerFillTopRight2;              ///< Additional decorative fill for record panel
    lv_obj_t *ui_RecordOutlay;                           ///< Decorative outlay for record panel
    lv_obj_t *ui_btnPause;                               ///< Pause recording button
    lv_obj_t *ui_pauseImage;                             ///< Image for pause button
    lv_obj_t *ui_btnSync;                                ///< Sync button
    lv_obj_t *ui_syncImage;                              ///< Image for sync button
    lv_obj_t *ui_btnRecord;                              ///< Record button
    lv_obj_t *ui_recordImage;                            ///< Image for record button
    lv_obj_t *ui_btnClear;                               ///< Clear data button
    lv_obj_t *ui_clearImage;                             ///< Image for clear button
    lv_obj_t *ui_btnSettings;                            ///< Settings button
    lv_obj_t *ui_settingsImage;                          ///< Image for settings button
    lv_obj_t *ui_SettingsOverlay;                        ///< Click blocking overlay
    lv_obj_t *ui_SettingsBridgeGroup;                    ///< Bridge group container that will remain under record and settings button
    lv_obj_t *ui_SettingsBridge;                         ///< Decorative bridge element
    lv_obj_t *ui_SettingsBridgeFill;                     ///< Decorative bridge fill
    lv_obj_t *ui_SettingsGroup;                          ///< Main container for the settings panel
    lv_obj_t *ui_SettingsOutlay;                         ///< Main background panel
    lv_obj_t *ui_SettingsHeaderLine;                     ///< Header separator line
    lv_obj_t *ui_SettingsHeaderLabel;                    ///< Header text
    lv_obj_t *ui_SettingsDataBundleLabel;                ///< Data bundle label
    lv_obj_t *ui_SettingsDataBundleCountLabel;           ///< Data bundle count label
    lv_obj_t *ui_SettingsDataBundleShowButton;           ///< Show bundle button
    lv_obj_t *ui_SettingsDataBundleShowButtonLabel;      ///< Show bundle button text
    lv_obj_t *ui_SettingsDataBundleDeleteAllButton;      ///< Delete button
    lv_obj_t *ui_SettingsDataBundleDeleteAllButtonLabel; ///< Delete button text
    lv_obj_t *ui_SettingsCreditsLabel;                   ///< Credits label
    lv_obj_t *ui_SettingsCreditsButton;                  ///< Credits button
    lv_obj_t *ui_SettingsCreditsButtonLabel;             ///< Credits button label
    lv_obj_t *ui_LogoGroup;                              ///< Logo panel widget
    lv_obj_t *ui_LogoCornerBottomLeft;                   ///< Decorative corner for logo panel
    lv_obj_t *ui_LogoCornerFillBottomLeft;               ///< Decorative fill for logo
    lv_obj_t *ui_LogoCornerTopRight;                     ///< Decorative corner for logo panel
    lv_obj_t *ui_LogoCornerFillBottomRight;              ///< Decorative fill for logo
    lv_obj_t *ui_LogoOutlay;                             ///< Decorative outlay for logo panel
    lv_obj_t *ui_LogoImage;                              ///< Logo image widget
    lv_obj_t *ui_ShadowOverlay;                          ///< Shadow overlay for dialog
    lv_obj_t *ui_Alert;                                  ///< Alert dialog container
    lv_obj_t *ui_AlertLabel;                             ///< Alert dialog label

    /**
     * @brief Add navigation buttons to a widget
     * @param parentWidget The parent widget to add buttons to
     */
    void addNavButtonsToWidget(lv_obj_t *parentWidget);

    /**
     * @brief Add control buttons (sync, back) to a widget
     * @param parentWidget The parent widget to add the buttons to
     */
    void addControlButtonsToWidget(lv_obj_t *parentWidget);

    /**
     * @brief Add record panel to a widget
     */
    void addRecordPanelToWidget(lv_obj_t *parentWidget);

    /**
     * @brief Add logo panel to a widget
     * @param parentWidget The parent widget to add the logo panel to
     */
    void addLogoPanelToWidget(lv_obj_t *parentWidget);

    /**
     * @brief Build device history data for chart display
     * @param sensor Pointer to the device
     * @param key The key of the device parameter
     * @param history The history array to store the history
     */
    template <typename T>
    void buildSensorHistory(BaseDevice *sensor, const std::string &key, lv_coord_t *history)
    {
        if (!history || !sensor)
            return;

        auto it = sensor->getValues().find(key);
        if (it == sensor->getValues().end())
            return;

        // Static storage between calls
        auto &buf = bufMap[key];
        bool &inited = initedMap[key];

        // Get current value as string and convert
        lv_coord_t curr;
        try
        {
            std::string s = sensor->getValue<std::string>(key);
            curr = convertStringToType<T>(s);

            if(recording&&sensor->getRedrawPending()){
                dataBundleManager.saveNewDataPoint(key, s);
            }   
        }
        catch (const std::exception &e)
        {
            throw InvalidDataTypeException("SignalsVisualizationGui::buildSensorHistory", e.what());
        }

        if (!inited)
        {
            // First call: fill entire buffer with current value
            for (int i = 0; i < HISTORY_CAP; ++i)
            {
                buf[i] = curr;
            }
            inited = true;
        }
        else
        {
            // Shift left by one position
            for (int i = 0; i < HISTORY_CAP - 1; ++i)
            {
                buf[i] = buf[i + 1];
            }
            // Add current value at the end
            buf[HISTORY_CAP - 1] = curr;
        }

        // Copy entire buffer to output array
        for (int i = 0; i < HISTORY_CAP; ++i)
        {
            try
            {
                history[i] = buf[i];
            }
            catch (const std::exception &e)
            {
                throw InvalidDataTypeException("SignalsVisualizationGui::buildSensorHistory", e.what());
            }
        }
    }

    void clearSensorHistoryBuffer(const std::string &key)
    {
        std::array<lv_coord_t, HISTORY_CAP> zeroBuf;
        zeroBuf.fill(0);

        bufMap[key] = zeroBuf;
        initedMap[key] = true;
    }

    /**
     * @brief Update device data display
     */
    void updateDeviceDataDisplay();

    /**
     * @brief Update chart with current device data
     */
    void updateChart();

    void updateDeviceTitle();
    void ensureSignalCards(size_t count);
    void clearUnusedSignalCards(size_t usedCount);
    void updateSignalCards(const std::unordered_map<std::string, DeviceParam> &values,
                           const std::vector<std::string> &valueKeys,
                           bool useValueControls);
    void ensureConfigControls(size_t count);
    void clearUnusedConfigControls(size_t usedCount);
    void updateEditableControls(const std::unordered_map<std::string, DeviceParam> &values,
                                const std::vector<std::string> &valueKeys,
                                const std::unordered_map<std::string, DeviceParam> &configs,
                                const std::vector<std::string> &configKeys,
                                bool useValueControls);
    void ensureControlEditor(ConfigControl &control, const DeviceParam &param, size_t controlIndex);
    void syncControlEditorValue(ConfigControl &control, const DeviceParam &param);
    bool buildNumericHistoryForKey(const std::string &key, lv_coord_t *history);
    std::vector<std::string> getChartableValueKeys() const;
    void showEmptyChartState(const char *message);
    void hideEmptyChartState();
    static std::pair<lv_coord_t, lv_coord_t> computeChartRange(const lv_coord_t *history);
    void populateChartSeries(lv_chart_series_t *series, const lv_coord_t *history);
    bool beginDeviceNavigation(bool requireIdleRecording, bool &wasRunning);
    void finishDeviceNavigation(bool wasRunning, BaseDevice *nextDevice);
    bool currentDeviceSupportsRecording() const;
    void updateActionButtonsState();
    bool applyEditableValue(bool isValueControl, const std::string &key, const std::string &value);
    void handleDropdownConfigChanged(size_t controlIndex);
    void handleSliderConfigChanged(size_t controlIndex);
    void handleTextConfigSubmitted(size_t controlIndex);
    static uint32_t getSignalAccentColor(size_t index);
    static bool isNumericType(DeviceDataType dtype);
    static bool hasSelectableOptions(const DeviceParam &param);
    static bool supportsSliderInput(const DeviceParam &param);
    static std::string buildUnitText(const std::string &unit, const char *fallbackText);

public:
    /**
     * @brief Constructor
     * @param deviceManager Reference to the device manager instance
     */
    SignalsVisualizationGui(DeviceManager &deviceManager, DataBundleManager &dataBundleManager);

    /**
     * @brief Destructor
     */
    ~SignalsVisualizationGui() = default;

    /**
     * @brief Initialize the device visualization GUI
     */
    void init();

    /**
     * @brief Check if the visualization GUI has been initialized
     * @return True if initialized, false otherwise
     */
    bool isInitialized() const { return initialized; }

    /**
     * @brief Construct the device visualization widget
     */
    void constructVisualization();

    /**
     * @brief Get parent widget
     */
    lv_obj_t *getParentWidget() const { return ui_SensorWidget; }

    /**
     * @brief Draw/update the currently selected sensor's visualization
     */
    void drawCurrentDevice();

    /**
     * @brief Go to the previous device in the list
     */
    void goToPreviousDevice();

    /**
     * @brief Go to the next device in the list
     */
    void goToNextDevice();

    /**
     * @brief Go to the first sensor in the list
     */
    void goToFirstDevice();

    /**
     * @brief Handle back button click event
     */
    void handleBackButtonClick();

    /**
     * @brief Handle pause button click event
     */
    void handlePauseButtonClick();

    /**
     * @brief Handle sync button click event
     */
    void handleSyncButtonClick();

    /**
     * @brief Handle record button click event
     * @param message Message to display on alert (this handle is called in different ways). Can be empty (default is "Record was saved (view settings)")
     */
    void handleRecordButtonClick(const char *message);

    /**
     * @brief opens a confirmation dialog to clear the current device's history data
     */
    void handleClearButtonClick();

    /**
     * @brief clears the current device's history data upon confirmation
     */
    void handleClearConfirmButtonClick();

    /**
     * @brief Show settings panel
     * @param recordGroup The record panel group to keep above the overlay
     * @param btnSettings The settings button to keep above the overlay
     */
    void handleSettingsButtonClick(lv_obj_t *recordGroup, lv_obj_t *btnSettings, lv_obj_t *parentWidget);

    /**
     * @brief Handle data bundle show button click event
     */
    void handleDataBundleShowButtonClick();

    /**
     * @brief Handle data bundle delete all button click event
     */
    void handleDataBundleDeleteAllButtonClick();

    /**
     * @brief Handle credits button click event
     */
    void handleCreditsButtonClick();

    /**
     * @brief User should be stopped when recording is still ongoing and they want to click on any bundle related buttons
     */
    void handleStillRecording();

    /**
     * @brief Synchronize the current device data
     * @return True if synchronization was successful, false otherwise
     */
    bool syncCurrentDevice();

    /**
     * @brief Show shadow overlay
     * @param popup The popup dialog which is meant to be highlighted
     */
    void showShadowOverlay();

    /**
     * @brief Hide shadow overlay
     */
    void hideShadowOverlay();

    /**
     * @brief Hide settings panel
     */
    void hideSettingsPanel();

    /**
     * @brief Show the device visualization screen
     */
    void showVisualization();

    /**
     * @brief Hide the device visualization screen
     */
    void hideVisualization();

    /**
     * @brief Show alert
     * @param message The alert message to display
     */
    void showAlert(const char *message);

    /**
     * @brief Hide alert
     */
    void hideAlert();
};

#endif // SIGNALS_VISUALIZATION_GUI_HPP
