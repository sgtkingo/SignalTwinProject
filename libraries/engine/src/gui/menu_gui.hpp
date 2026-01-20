/**
 * @file menu_gui.hpp
 * @brief Declaration of the MenuGui widget
 *
 * This header defines the MenuGui class which provides
 * a widget with a Connect button and pin selection/assignment functionality.
 * Focused on menu visualization and sensor-to-pin assignment operations.
 *
 * @copyright 2025 MTA
 * @author Ing. Jiri Konecny, Ondřej Wrubel
 */

#ifndef MENU_GUI_HPP
#define MENU_GUI_HPP

#include "lvgl.h"
#include <array>

#include "gui_callbacks.hpp"
#include "../managers/manager.hpp"

/**
 * @class MenuGui
 * @brief Handles menu visualization and sensor-to-pin assignment functionality.
 *
 * This class is responsible for:
 * - Displaying the main menu with Connect button and pin selection
 * - Managing pin assignment interface and active pin selection
 * - Handling menu-related events and navigation
 * - Providing interface for sensor-to-pin mapping operations
 */
class MenuGui
{
private:
    SensorManager& sensorManager;  ///< Reference to the SensorManager instance
    bool initialized = false;      ///< Initialization state flag
    int activePinIndex = -1;       ///< Currently selected/active pin index (-1 = none)

    // --- MENU GUI MEMBERS ---
    lv_obj_t *ui_MenuWidget;                ///< Main menu widget
    lv_obj_t *ui_PinScrollContainer;        ///< Scrollable container for pins
    lv_obj_t *ui_ButtonConnectGroup;          ///< Group for Connect button
    lv_obj_t *ui_ButtonConnectCornerTopLeft;  ///< Top-left corner decoration
    lv_obj_t *ui_ButtonConnectCornerBottomRight; ///< Bottom-right corner decoration
    lv_obj_t *ui_btnConnect;                  ///< Connect button
    lv_obj_t *ui_ButtonConnectLabel;          ///< Label for Connect button
    std::array<lv_obj_t *, NUM_PINS> pinContainers; ///< Containers for pin selection
    std::array<lv_obj_t *, NUM_PINS> pinLabels;     ///< Labels for pin selection

    /**
     * @brief Build the menu GUI widgets
     */
    void buildMenu();

    /**
     * @brief Update visual state of pin buttons based on assignments and selection
     */
    void updatePinVisualStates();
    
    /**
     * @brief Handle Connect button click event
     */
    void handleConnectButtonClick();
    
    /**
     * @brief Handle pin button click event
     * @param pinIndex Index of the clicked pin
     */
    void handlePinClick(int pinIndex);

public:
    /**
     * @brief Constructor
     * @param sensorManager Reference to the SensorManager instance
     */
    MenuGui(SensorManager& sensorManager);
    
    /**
     * @brief Destructor
     */
    ~MenuGui() = default;

    /**
     * @brief Initialize the menu GUI
     */
    void init();

    /**
     * @brief Check if the menu GUI has been initialized
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
     * @brief Update the text labels for pin selections based on assigned sensors
     */
    void updatePinLabels();

    /**
     * @brief Set the currently active/selected pin
     * @param pinIndex Index of the pin to set as active (-1 for none)
     */
    void setActivePin(int pinIndex);

    /**
     * @brief Get the currently active/selected pin index
     * @return Index of the active pin (-1 if none selected)
     */
    int getActivePin() const { return activePinIndex; }

    /**
     * @brief Initialize pin configuration with locking logic
     */
    void initializePins();

    /**
     * @brief Get pin state color for visualization
     * @param pinIndex Pin index
     * @return LVGL color value for pin state
     */
    uint32_t getPinStateColor(int pinIndex) const;
};

#endif // MENU_GUI_HPP