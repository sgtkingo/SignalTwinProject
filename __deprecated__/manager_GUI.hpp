/**
 * @file main_menu.hpp
 * @brief Declaration of the GuiManager widget
 *
 * This header defines the GuiManager class which provides
 * a widget with a Start button and 6 pin selection buttons.
 *
 * @copyright 2025 MTA
 * @author Ondřej Wrubel
 */

#ifndef MAIN_MENU_HPP
#define MAIN_MENU_HPP

#include "lvgl.h"
#include <array>
#include <map>

#include "../managers/manager.hpp"
#include "../sensors/base_sensor.hpp"

class GuiManager
{
private:
    SensorManager &sensorManager;  ///< Reference to the SensorManager instance
    bool initialized = false;  ///< Initialization state flag

    // --- MENU GUI MEMBERS ---

    lv_obj_t *ui_MenuWidget;                ///< Main menu widget
    lv_obj_t *ui_ButtonStartGroup;          ///< Group for start button
    lv_obj_t *ui_ButtonStartCornerTopLeft;  ///< Top-left corner decoration
    lv_obj_t *ui_ButtonStartCornerBottomRight; ///< Bottom-right corner decoration
    lv_obj_t *ui_btnStart;                  ///< Start button
    lv_obj_t *ui_ButtonStartLabel;          ///< Label for start button
    std::array<lv_obj_t *, NUM_PINS> pinContainers;///< Containers for pin selection
    std::array<lv_obj_t *, NUM_PINS> pinLabels;    ///< Labels for pin selection

    /**
     * @brief Build the menu GUI widgets
     */
    void buildMenu();

    // --- SENSOR GUI MEMBERS ---

    lv_obj_t *ui_SensorWidget;              ///< Widget for sensor visualisation
    lv_obj_t *ui_SensorLabel;               ///< Label for sensor name
    // VALUE_1
    lv_obj_t *ui_ContainerForValue_1;       ///< Container for first value
    lv_obj_t *ui_VisualColorForValue_1;     ///< Color indicator for first value
    lv_obj_t *ui_LabelValueValue_1;         ///< Value label for first value
    lv_obj_t *ui_LabelDescValue_1;          ///< Description label for first value
    lv_obj_t *ui_LabelTypeValue_1;          ///< Type label for first value
    // VALUE_2
    lv_obj_t *ui_VisualColorForValue_2;     ///< Color indicator for second value
    lv_obj_t *ui_ContainerForValue_2;       ///< Container for second value
    lv_obj_t *ui_LabelValueValue_2;         ///< Value label for second value
    lv_obj_t *ui_LabelDescValue_2;          ///< Description label for second value
    lv_obj_t *ui_LabelTypeValue_2;          ///< Type label for second value
    // CHART
    lv_obj_t *ui_Chart;                     ///< Chart widget for sensor data
    lv_chart_series_t *ui_Chart_series_V1;  ///< Chart series for value 1

    // --- WIKI MEMBERS ---

    lv_obj_t *ui_SensorWidgetWiki;          ///< Widget for sensor wiki
    lv_obj_t *ui_SensorLabelWiki;           ///< Wiki label for sensor name
    lv_obj_t *ui_SensorLabelDescription;    ///< Wiki label for sensor description
    lv_obj_t *ui_SensorImage;               ///< Image for sensor wiki

    // --- SENSOR EVENT GUI MEMBERS ---

    lv_obj_t *ui_btnPrev;                   ///< Previous sensor button
    lv_obj_t *ui_btnPrevLabel;              ///< Label for previous button
    lv_obj_t *ui_btnNext;                   ///< Next sensor button
    lv_obj_t *ui_btnNextLabel;              ///< Label for next button
    lv_obj_t *ui_btnConfirm;                ///< Confirm button
    lv_obj_t *ui_btnConfirmLabel;           ///< Label for confirm button

    /**
     * @brief Add navigation buttons to a widget
     * @param parentWidget The parent widget to add buttons to
     * @param isVisualisation If true, for visualisation, else for wiki
     */
    void addNavButtonsToWidget(lv_obj_t *parentWidget, bool isVisualisation = true);

    /**
     * @brief Add a confirm button to a widget
     * @param parentWidget The parent widget to add the button to
     */
    void addConfirmButtonToWidget(lv_obj_t *parentWidget);

    /**
     * @brief Add a back button to a widget
     * @param parentWidget The parent widget to add the button to
     */
    void addBackButtonToWidget(lv_obj_t *parentWidget);

    /**
     * @brief Get history from sensor.
     *
     * This function retrieves the history of a sensor parameter by key.
     *
     * @param key The key of the sensor parameter.
     * @param history The history array to store the history.
     */
    template <typename T>
    void buildSensorHistory(BaseSensor *sensor, const std::string &key, lv_coord_t *history) {
        if (!history || !sensor) return;

        auto it = sensor->getValues().find(key);
        if (it == sensor->getValues().end()) return;

        // statické úložiště mezi voláními
        static std::map<std::string, std::array<lv_coord_t, HISTORY_CAP>> bufMap;
        static std::map<std::string, bool> initedMap;
    
        auto &buf    = bufMap[key];
        bool &inited = initedMap[key];
    
        // 1) Místo čtení z History[], načti aktuální hodnotu jako string a převeď
        lv_coord_t curr;
        try {
            // getValue vrací string, kterej jste dřív ukládali např. "100"
            std::string s = sensor->getValue<std::string>(key);
            curr = convertStringToType<T>(s);
        }
        catch (const std::exception &e) {
            throw InvalidDataTypeException("BaseSensor::getValue", e.what());
        }
    
        if (!inited) {
            // první volání: celý buffer naplň aktuální hodnotou
            for (int i = 0; i < HISTORY_CAP; ++i) {
                buf[i] = curr;
            }
            inited = true;
        }
        else {
            // posuň doleva o jednu pozici …
            for (int i = 0; i < HISTORY_CAP - 1; ++i) {
                buf[i] = buf[i + 1];
            }
            // … a na konec vlož okamžitý curr
            buf[HISTORY_CAP - 1] = curr;
        }
    
        // 2) Zkopíruj celý buffer do výstupního pole
        for (int i = 0; i < HISTORY_CAP; ++i) {
            try {
                history[i] = buf[i];
            }
            catch (const std::exception &e) {
                throw InvalidDataTypeException("BaseSensor::getValue", e.what());
            }
        }
    }

public:
    /**
     * @brief Private constructor for singleton pattern
     */
    GuiManager(SensorManager &manager);
    ~GuiManager() = default;
    
    /**
     * @brief Get the SensorManager instance
     */
    SensorManager &getSensorManager();

    /**
     * @brief Initialize the GUI manager and sensors
     */
    void init();

    /**
     * @brief Check if the manager has been initialized
     * @return True if initialized, false otherwise
     */
    bool isInitialized() const { return initialized; }

    /**
     * @brief Show the main menu GUI
     */
    void showMenu();

    /**
     * @brief Hide the main menu GUI
     */
    void hideMenu();

    /**
     * @brief Update the text labels for pin selections
     */
    void updatePinLabelText();

    /**
     * @brief Draw the currently selected sensor's GUI
     */
    void drawCurrentSensor();

    /**
     * @brief Construct the main menu and sensor widgets
     */
    void construct();

    /**
     * @brief Construct the sensor wiki widget
     */
    void constructWiki();

    /**
     * @brief Go to the first sensor in the list
     * @param isVisualisation If true, show visualisation, otherwise show wiki
     */
    void goToFirstSensor(bool isVisualisation);

    /**
     * @brief Show the sensor wiki screen
     */
    void showSensorWiki();

    /**
     * @brief Hide the sensor wiki screen
     */
    void hideSensorWiki();

    /**
     * @brief Show the sensor visualisation screen
     */
    void showSensorVisualisation();

    /**
     * @brief Hide the sensor visualisation screen
     */
    void hideSensorVisualisation();
};

#endif
