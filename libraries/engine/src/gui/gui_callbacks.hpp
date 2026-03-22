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

#include "app_settings.hpp"

/**
 * @brief Switch to connection screen and stop sensors
 * 
 * This function switches the GUI to connection mode and stops all sensor operations.
 */
extern void switchToConnection();

/**
 * @brief Backward-compatible alias for the connection screen.
 */
extern void switchToMenu();

/**
 * @brief Switch to main menu screen
 */
extern void switchToMainMenu();

/**
 * @brief Switch to sensor visualization screen
 * 
 * This function switches the GUI to sensor visualization mode.
 * Typically called after starting sensor operations.
 */
extern void switchToVisualization();

/**
 * @brief Switch to selection screen
 *
 * This function switches the GUI to device selection mode.
 */
extern void switchToSelection();

/**
 * @brief Backward-compatible alias for Selection screen navigation.
 */
extern void switchToWiki();

/**
 * @brief Open the visualization flow according to current settings
 */
extern void openVisualizationFlow();

/**
 * @brief Finish communication selection and enter Selection
 */
extern void completeCommunicationSelection(DefaultCommunicationMode mode);

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
 * @brief Switch to databank screen from main menu
 */
extern void switchToDatabank();

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

/**
 * @brief Switch to library screen
 */
extern void switchToLibrary();

/**
 * @brief Switch to library editor screen
 */
extern void switchToLibraryEditor();

/**
 * @brief Prepare a new library entity editor session
 */
extern void prepareNewLibraryEntity();

/**
 * @brief Switch to settings screen
 */
extern void switchToSettings();

/**
 * @brief Return from data bundle selection based on origin
 */
extern void navigateBackFromDatabank();

/**
 * @brief Change the default communication mode
 */
extern void setDefaultCommunicationMode(DefaultCommunicationMode mode);

/**
 * @brief Get the default communication mode
 */
extern DefaultCommunicationMode getDefaultCommunicationMode();

/**
 * @brief Check whether Selection should go back directly to Main Menu
 */
extern bool shouldSelectionBackGoToMainMenu();

#endif // GUI_CALLBACKS_HPP
