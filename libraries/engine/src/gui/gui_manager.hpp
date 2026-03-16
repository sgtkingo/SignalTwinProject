/**
 * @file gui_manager.hpp
 * @brief Declaration of the GuiManager class
 */

#ifndef GUI_MANAGER_HPP
#define GUI_MANAGER_HPP

#include "../managers/data_bundle_manager.hpp"
#include "../managers/manager.hpp"
#include "app_selection_gui.hpp"
#include "app_settings.hpp"
#include "communication_selection_gui.hpp"
#include "crash_gui.hpp"
#include "credits_gui.hpp"
#include "data_bundle_selection_gui.hpp"
#include "library_editor_gui.hpp"
#include "library_gui.hpp"
#include "main_menu_gui.hpp"
#include "menu_gui.hpp"
#include "sensor_visualization_gui.hpp"
#include "sensor_wiki_gui.hpp"
#include "settings_gui.hpp"

enum class GuiState
{
    MAIN_MENU,
    CONNECTION,
    VISUALIZATION,
    DATA_BUNDLE_SELECTION,
    SELECTION,
    LIBRARY,
    LIBRARY_EDITOR,
    SETTINGS,
    READY,
    CRASH,
    CREDITS,
    APP_SELECTION,
    COMMUNICATION_SELECTION,
    NONE
};

class GuiManager
{
private:
    SensorManager &sensorManager;
    DataBundleManager &dataBundleManager;
    MainMenuGui mainMenuGui;
    MenuGui menuGui;
    SensorVisualizationGui vizGui;
    DataBundleSelectionGui dataBundleSelectionGui;
    SensorWikiGui wikiGui;
    LibraryGui libraryGui;
    LibraryEditorGui libraryEditorGui;
    SettingsGui settingsGui;
    CrashGui crashGui;
    CreditsGui creditsGui;
    AppSelectionGui appSelectionGui;
    CommunicationSelectionGui communicationSelectionGui;

    GuiState currentState;
    bool initialized;
    bool databankReturnToVisualization = false;
    DefaultCommunicationMode defaultCommunicationMode = DefaultCommunicationMode::ASK;
    DefaultCommunicationMode sessionCommunicationMode = DefaultCommunicationMode::ASK;
    bool selectionBackToMainMenu = false;

    void hideAllComponents();

public:
    explicit GuiManager(SensorManager &manager, DataBundleManager &dataBundleManager);

    bool init();
    bool init(std::string configFile);
    bool isInitialized() const { return initialized; }

    GuiState getCurrentState() const { return currentState; }

    void showMainMenu();
    void showMenu();
    void showVisualization();
    void showDataBundleSelection();
    void showWiki();
    void showLibrary();
    void showLibraryEditor();
    void showSettings();
    void showCrashScreen(const std::string &reason = "Unexpected error");
    void showCreditsScreen();
    void showAppSelectionScreen();
    void showCommunicationSelectionScreen();

    void switchContent(GuiState targetState);
    void redraw();

    void openVisualizationFlow();
    void completeCommunicationSelection(DefaultCommunicationMode mode);
    void openDatabankFromMainMenu();
    void openDatabankFromVisualization();
    void navigateBackFromDatabank();
    bool shouldSelectionBackGoToMainMenu() const { return selectionBackToMainMenu; }

    DefaultCommunicationMode getDefaultCommunicationMode() const { return defaultCommunicationMode; }
    void setDefaultCommunicationMode(DefaultCommunicationMode mode) { defaultCommunicationMode = mode; }

    MenuGui &getMenuGui() { return menuGui; }
    SensorVisualizationGui &getVisualizationGui() { return vizGui; }
    DataBundleSelectionGui &getDataBundleSelectionGui() { return dataBundleSelectionGui; }
    SensorWikiGui &getWikiGui() { return wikiGui; }
    CreditsGui &getCreditsGui() { return creditsGui; }
    SensorManager &getSensorManager() { return sensorManager; }
    DataBundleManager &getDataBundleManager() { return dataBundleManager; }
    AppSelectionGui &getAppSelectionGui() { return appSelectionGui; }
    CommunicationSelectionGui &getCommunicationSelectionGui() { return communicationSelectionGui; }
};

#endif // GUI_MANAGER_HPP
