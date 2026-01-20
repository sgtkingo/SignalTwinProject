/**
 * @file sensor_wiki_gui.hpp
 * @brief Declaration of the SensorWikiGui class
 *
 * This header defines the SensorWikiGui class which provides
 * sensor information display and selection functionality.
 *
 * @copyright 2025 MTA
 * @author Ing. Jiri Konecny, Ondřej Wrubel
 */

#ifndef SENSOR_WIKI_GUI_HPP
#define SENSOR_WIKI_GUI_HPP

#include <vector>
#include <string>

#include "lvgl.h"
#include "gui_callbacks.hpp"
#include "../managers/manager.hpp"


/**
 * @class SensorWikiGui
 * @brief Handles sensor wiki display and sensor selection functionality
 *
 * This class provides:
 * - Display of sensor information, specifications, and usage
 * - List of available sensors for selection
 * - Select button to assign chosen sensor to active pin
 * - Navigation between different sensor types
 */
class SensorWikiGui {
private:
    SensorManager& sensorManager;   ///< Reference to sensor manager
    bool initialized = false;       ///< Initialization flag
    int selectedSensorIndex = 0;    ///< Currently selected sensor index
    int activePinIndex = -1;        ///< Pin to assign sensor to
    
    // GUI Components
    lv_obj_t* ui_WikiWidget;        ///< Main wiki container
    lv_obj_t* ui_SensorList;        ///< List of available sensors
    lv_obj_t* ui_SensorInfo;        ///< Sensor information display
    lv_obj_t* ui_SensorTitle;       ///< Sensor name/title
    lv_obj_t* ui_SensorDescriptionTitle; ///< Sensor description title
    lv_obj_t* ui_SensorDescription; ///< Sensor description text
    lv_obj_t* ui_SensorSpecsTitle;  ///< Sensor specifications title
    lv_obj_t* ui_SensorSpecs;       ///< Sensor specifications
    lv_obj_t* ui_SensorConfTitle;   ///< Sensor configuration title
    lv_obj_t* ui_SensorConf;        ///< Sensor configuration
    lv_obj_t* ui_SelectButton;      ///< Select sensor button
    lv_obj_t* ui_StartButton;       ///< Back to menu button    
    lv_obj_t* ui_StartButtonLabel;  ///< Back to menu button label
    lv_obj_t* ui_PrevButton;        ///< Previous sensor button
    lv_obj_t* ui_NextButton;        ///< Next sensor button
    
    /**
     * @brief Build the wiki GUI components
     */
    void buildWikiGui();
    
    /**
     * @brief Update sensor information display
     */
    void updateSensorInfo();
    
    /**
     * @brief Handle sensor selection from list
     * @param sensorIndex Index of selected sensor
     */
    void handleSensorSelection(int sensorIndex);
    
    /**
     * @brief Handle select button click (assign sensor to pin)
     */
    void handleSelectButtonClick();
    
    /**
     * @brief Handle back button click (return to menu)
     */
    void handleStartButtonClick();
    
    /**
     * @brief Handle previous/next sensor navigation
     * @param direction -1 for previous, +1 for next
     */
    void navigateSensor(int direction);
    
    /**
     * @brief Get sensor information text
     * @param sensor Pointer to sensor
     * @return Formatted information string
     */
    std::string getSensorInfoText(BaseSensor* sensor);
    
    /**
     * @brief Get sensor specifications text
     * @param sensor Pointer to sensor
     * @return Formatted specifications string
     */
    std::string getSensorSpecsText(BaseSensor* sensor);

    /**
     * @brief Get sensor configuration text
     * @param sensor Pointer to sensor
     * @return Formatted configuration string
     */
    std::string getSensorConfText(BaseSensor* sensor);

public:
    /**
     * @brief Constructor
     * @param sensorManager Reference to sensor manager
     */
    explicit SensorWikiGui(SensorManager& sensorManager);
    
    /**
     * @brief Destructor
     */
    ~SensorWikiGui() = default;
    
    /**
     * @brief Initialize the wiki GUI
     */
    void init();
    
    /**
     * @brief Check if wiki GUI is initialized
     * @return True if initialized
     */
    bool isInitialized() const { return initialized; }
    
    /**
     * @brief Show the wiki GUI
     * @param pinIndex Pin index that sensor will be assigned to
     */
    void showWiki(int pinIndex = -1);
    
    /**
     * @brief Hide the wiki GUI
     */
    void hideWiki();
    
    /**
     * @brief Set the active pin for sensor assignment
     * @param pinIndex Pin index (-1 for no pin)
     */
    void setActivePin(int pinIndex) { activePinIndex = pinIndex; }
    
    /**
     * @brief Get the currently selected sensor
     * @return Pointer to selected sensor or nullptr
     */
    BaseSensor* getSelectedSensor();
    
    /**
     * @brief Set the selected sensor by index
     * @param index Sensor index
     */
    void setSelectedSensor(int index);
};

#endif // SENSOR_WIKI_GUI_HPP