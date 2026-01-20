/**
 * @file gui_manager.hpp
 * @brief Declaration of the GuiManager class
 *
 * This header defines the GuiManager class which coordinates
 * between MenuGui and SensorVisualizationGui components, providing
 * a central access point for GUI operations.
 *
 * @copyright 2025 MTA
 * @author Ing. Jiri Konecny, Ondřej Wrubel
 */

#ifndef GUI_MANAGER_HPP
#define GUI_MANAGER_HPP

#include "../managers/manager.hpp"
#include "../managers/data_bundle_manager.hpp"
#include "menu_gui.hpp"
#include "sensor_visualization_gui.hpp"
#include "data_bundle_selection_gui.hpp"
#include "sensor_wiki_gui.hpp"
#include "crash_gui.hpp"
#include "credits_gui.hpp"
#include "app_selection_gui.hpp"
#include "communication_selection_gui.hpp"

/**
 * @brief Enumeration for different GUI states
 */
enum class GuiState
{
    MENU,                    ///< Main menu with pin assignment
    VISUALIZATION,           ///< Sensor data visualization
    DATA_BUNDLE_SELECTION,   ///< Data bundles visualization
    WIKI,                    ///< Sensor documentation/wiki
    READY,                   ///< No active GUI
    CRASH,                   ///< Crash screen
    CREDITS,                 ///< Credits screen
    APP_SELECTION,           ///< App selection screen
    COMMUNICATION_SELECTION, ///< Communication selection screen
    NONE                     ///< Not ready / No active GUI
};

/**
 * @brief GUI Manager class
 *
 * This class serves as a central coordinator that initializes GUI components
 * and handles screen switching between menu and sensor visualization.
 */
class GuiManager
{
private:
    SensorManager &sensorManager;                        ///< Reference to sensor manager
    DataBundleManager &dataBundleManager;                ///< Reference to data bundle manager
    MenuGui menuGui;                                     ///< Menu and pin assignment component
    SensorVisualizationGui vizGui;                       ///< Sensor visualization component
    DataBundleSelectionGui dataBundleSelectionGui;       ///< Data bundle selection component
    SensorWikiGui wikiGui;                               ///< Sensor wiki component
    CrashGui crashGui;                                   ///< Crash screen component
    CreditsGui creditsGui;                               ///< Credits screen component
    AppSelectionGui appSelectionGui;                     ///< App selection component
    CommunicationSelectionGui communicationSelectionGui; ///< Communication selection component

    GuiState currentState; ///< Current GUI state
    bool initialized;      ///< Initialization flag

    /**
     * @brief Hide all GUI components
     */
    void hideAllComponents();

public:
    /**
     * @brief Constructor
     * @param manager Reference to the SensorManager instance
     * @param dataBundleManager reference to the DataBundleManager instance
     */
    explicit GuiManager(SensorManager &manager, DataBundleManager &dataBundleManager);

    /**
     * @brief Initialize the unified GUI manager and all components
     * @return true if initialization was successful, false otherwise
     */
    bool init();

    /**
     * @brief Initialize the unified GUI manager and all components with config file
     * @param configFile Path to configuration file
     * @return true if initialization was successful, false otherwise
     */
    bool init(std::string configFile);

    /**
     * @brief Check if the GUI manager is initialized
     * @return true if initialized, false otherwise
     */
    bool isInitialized() const { return initialized; }

    // === SCREEN SWITCHING ===

    /**
     * @brief Get current GUI state
     * @return Current GuiState
     */
    GuiState getCurrentState() const { return currentState; }

    /**
     * @brief Switch to menu screen
     */
    void showMenu();

    /**
     * @brief Switch to sensor visualization screen
     */
    void showVisualization();

    /**
     * @brief Switch to data bundle selection screen
     */
    void showDataBundleSelection();

    /**
     * @brief Switch to sensor wiki screen
     */
    void showWiki();

    /**
     * @brief Switch to crash screen
     */
    void showCrashScreen(const std::string &reason = "Unexpected error");

    /**
     * @brief Switch to credits screen
     */
    void showCreditsScreen();

    /**
     * @brief Switch to app selection screen
     */
    void showAppSelectionScreen();

    /**
     * @brief Switch to communication selection screen
     */
    void showCommunicationSelectionScreen();

    /**
     * @brief Switch content to specified GUI state
     *
     * This method switches to the specified GuiState and manages
     * SensorManager running state accordingly:
     * - APP_SELECTION: stops sensors (setRunning(false))
     * - COMMUNICATION_SELECTION: stops sensors (setRunning(false))
     * - MENU: stops sensors (setRunning(false))
     * - VISUALIZATION: starts sensors (setRunning(true))
     * - WIKI: no sensor state change
     * - DATA_BUNDLE_SELECTION: stops sensors (setRunning(false))
     * - CREDITS: stops sensors (setRunning(false))
     * - CRASH: stops sensors (setRunning(false))
     *
     * @param targetState The GUI state to switch to
     */
    void switchContent(GuiState targetState);

    /**
     * @brief Redraw GUI content based on current state
     *
     * This method redraws GUI components depending on the current GuiState.
     * For VISUALIZATION state, it redraws the current sensor data.
     * Note: This method only handles GUI redrawing, not data synchronization.
     */
    void redraw();

    // === COMPONENT ACCESS ===

    /**
     * @brief Get reference to menu GUI component
     * @return Reference to MenuGui
     */
    MenuGui &getMenuGui() { return menuGui; }

    /**
     * @brief Get reference to visualization GUI component
     * @return Reference to SensorVisualizationGui
     */
    SensorVisualizationGui &getVisualizationGui() { return vizGui; }

    /**
     * @brief Get reference to data bundle selection GUI component
     * @return Reference to DataBundleSelectionGui
     */
    DataBundleSelectionGui &getDataBundleSelectionGui() { return dataBundleSelectionGui; }

    /**
     * @brief Get reference to sensor wiki GUI component
     * @return Reference to SensorWikiGui
     */
    SensorWikiGui &getWikiGui() { return wikiGui; }

    /**
     * @brief Get reference to credits GUI component
     * @return Reference to CreditsGui
     */
    CreditsGui &getCreditsGui() { return creditsGui; }

    /**
     * @brief Get reference to sensor manager
     * @return Reference to SensorManager
     */
    SensorManager &getSensorManager() { return sensorManager; }

    /**
     * @brief Get reference to data bundle manager
     * @return Reference to DataBundleManager
     */
    DataBundleManager &getDataBundleManager() { return dataBundleManager; }

    /**
     * @brief Get reference to app selection GUI component
     * @return Reference to AppSelectionGui
     */
    AppSelectionGui &getAppSelectionGui() { return appSelectionGui; }

    /**
     * @brief Get reference to communication selection GUI component
     * @return Reference to CommunicationSelectionGui
     */
    CommunicationSelectionGui &getCommunicationSelectionGui() { return communicationSelectionGui; }
};

#endif // GUI_MANAGER_HPP