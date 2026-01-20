/**
 * @file gui_callbacks.hpp
 * @brief Declaration of global GUI callback functions
 *
 * This header defines global callback functions that GUI components
 * can use to request screen switching operations. These functions
 * are implemented in the main ui.ino file and provide communication
 * between GUI components and the main GuiManager.
 *
 * @copyright 2025 MTA
 * @author Ing. Jiri Konecny, Ondřej Wrubel
 */

#ifndef GUI_CALLBACKS_HPP
#define GUI_CALLBACKS_HPP

#include <string>

/**
 * @brief Switch to menu screen and stop sensors
 * 
 * This function switches the GUI to menu mode and stops all sensor operations.
 * Typically called by the Back button in visualization screens.
 */
extern void switchToMenu();

/**
 * @brief Switch to sensor visualization screen
 * 
 * This function switches the GUI to sensor visualization mode.
 * Typically called after starting sensor operations.
 */
extern void switchToVisualization();

/**
 * @brief Switch to sensor wiki screen
 * 
 * This function switches the GUI to sensor wiki/documentation mode.
 */
extern void switchToWiki();

/**
 * @brief Switch to crash screen with specified reason
 * 
 * This function switches the GUI to the crash screen and displays
 * the provided reason for the crash. It is typically called when
 * a critical error occurs in the system.
 * 
 * @param reason The reason for the crash to display
 */
extern void switchToCrashScreen(const std::string &reason);

/**
 * @brief Switch to data bundle selection screen
 * 
 * This function switches the GUI to the data bundle selection mode.
 */
extern void switchToDataBundleSelection();

/**
 * @brief Switch to credits screen
 * 
 * This function switches the GUI to the third-party credits screen.
 * Called from the settings section in visualization.
 */
extern void switchToCreditsScreen();

/**
 * @brief Switch to application selection screen
 * 
 * This function switches the GUI to the application selection screen.
 */
extern void switchToAppSelectionScreen();

/**
 * @brief Switch to communication selection screen
 * 
 * This function switches the GUI to the communication selection screen.
 */
extern void switchToCommunicationSelectionScreen();

#endif // GUI_CALLBACKS_HPP