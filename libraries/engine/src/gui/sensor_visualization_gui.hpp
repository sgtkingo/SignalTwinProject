/**
 * @file sensor_visualization_gui.hpp
 * @brief Declaration of the SensorVisualizationGui widget
 *
 * This header defines the SensorVisualizationGui class which handles
 * active sensor visualization, data display, synchronization, and navigation.
 *
 * @copyright 2025 MTA
 * @author Ing. Jiri Konecny, Ondřej Wrubel
 */

#ifndef SENSOR_VISUALIZATION_GUI_HPP
#define SENSOR_VISUALIZATION_GUI_HPP

#include "lvgl.h"
#include <array>
#include <map>

#include "gui_callbacks.hpp"
#include "../managers/manager.hpp"
#include "../managers/data_bundle_manager.hpp"
#include "../exceptions/data_exceptions.hpp"

/**
 * @class SensorVisualizationGui
 * @brief Handles active sensor visualization, data display, and navigation.
 *
 * This class is responsible for:
 * - Displaying current sensor data and values
 * - Handling sensor navigation (prev/next)
 * - Managing sensor synchronization operations
 * - Handling sensor-specific events and interactions
 */
class SensorVisualizationGui
{
private:
    SensorManager &sensorManager;        ///< Reference to the sensor manager instance
    DataBundleManager &dataBundleManager;///< Reference to the databundle manager instance
    BaseSensor *currentSensor = nullptr; ///< Currently visualized sensor

    /// Static buffers for chart data
    std::map<std::string, std::array<lv_coord_t, HISTORY_CAP>> bufMap;
    std::map<std::string, bool> initedMap;

    bool initialized = false; ///< Initialization state flag
    bool paused = false;      ///< Pause state flag
    bool recording = false;   ///< Recording state flag

    // --- SENSOR VISUALIZATION MEMBERS ---
    lv_obj_t *ui_SensorWidget; ///< Widget for sensor visualisation
    lv_obj_t *ui_SensorLabel;  ///< Label for sensor name

    // VALUE_1
    lv_obj_t *ui_ContainerForValue_1;   ///< Container for first value
    lv_obj_t *ui_VisualColorForValue_1; ///< Color indicator for first value
    lv_obj_t *ui_LabelValueValue_1;     ///< Value label for first value
    lv_obj_t *ui_LabelDescValue_1;      ///< Description label for first value
    lv_obj_t *ui_LabelTypeValue_1;      ///< Type label for first value

    // VALUE_2
    lv_obj_t *ui_VisualColorForValue_2; ///< Color indicator for second value
    lv_obj_t *ui_ContainerForValue_2;   ///< Container for second value
    lv_obj_t *ui_LabelValueValue_2;     ///< Value label for second value
    lv_obj_t *ui_LabelDescValue_2;      ///< Description label for second value
    lv_obj_t *ui_LabelTypeValue_2;      ///< Type label for second value

    // CHART
    lv_obj_t *ui_Chart;                    ///< Chart widget for sensor data
    lv_chart_series_t *ui_Chart_series_V1; ///< Chart series for value 1
    lv_chart_series_t *ui_Chart_series_V2; ///< Chart series for value 2

    // --- NAVIGATION AND CONTROL MEMBERS ---
    lv_obj_t *ui_btnPrev;                                ///< Previous sensor button
    lv_obj_t *ui_btnPrevLabel;                           ///< Label for previous button
    lv_obj_t *ui_btnNext;                                ///< Next sensor button
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
     * @brief Build sensor history data for chart display
     * @param sensor Pointer to the sensor
     * @param key The key of the sensor parameter
     * @param history The history array to store the history
     */
    template <typename T>
    void buildSensorHistory(BaseSensor *sensor, const std::string &key, lv_coord_t *history)
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

            if(recording){
                dataBundleManager.saveNewDataPoint(key, s);
            }   
        }
        catch (const std::exception &e)
        {
            throw InvalidDataTypeException("SensorVisualizationGui::buildSensorHistory", e.what());
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
                throw InvalidDataTypeException("SensorVisualizationGui::buildSensorHistory", e.what());
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
     * @brief Update sensor data display
     */
    void updateSensorDataDisplay();

    /**
     * @brief Update chart with current sensor data
     */
    void updateChart();

public:
    /**
     * @brief Constructor
     * @param sensorManager Reference to the sensor manager instance
     */
    SensorVisualizationGui(SensorManager &sensorManager, DataBundleManager &dataBundleManager);

    /**
     * @brief Destructor
     */
    ~SensorVisualizationGui() = default;

    /**
     * @brief Initialize the sensor visualization GUI
     */
    void init();

    /**
     * @brief Check if the visualization GUI has been initialized
     * @return True if initialized, false otherwise
     */
    bool isInitialized() const { return initialized; }

    /**
     * @brief Construct the sensor visualization widget
     */
    void constructVisualization();

    /**
     * @brief Get parent widget
     */
    lv_obj_t *getParentWidget() const { return ui_SensorWidget; }

    /**
     * @brief Draw/update the currently selected sensor's visualization
     */
    void drawCurrentSensor();

    /**
     * @brief Go to the previous sensor in the list
     */
    void goToPreviousSensor();

    /**
     * @brief Go to the next sensor in the list
     */
    void goToNextSensor();

    /**
     * @brief Go to the first sensor in the list
     */
    void goToFirstSensor();

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
     * @brief opens a confirmation dialog to clear the current sensor's history data
     */
    void handleClearButtonClick();

    /**
     * @brief clears the current sensor's history data upon confirmation
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
     * @brief Synchronize the current sensor data
     * @return True if synchronization was successful, false otherwise
     */
    bool syncCurrentSensor();

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
     * @brief Show the sensor visualization screen
     */
    void showVisualization();

    /**
     * @brief Hide the sensor visualization screen
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

#endif // SENSOR_VISUALIZATION_GUI_HPP