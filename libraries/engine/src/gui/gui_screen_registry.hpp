#ifndef GUI_SCREEN_REGISTRY_HPP
#define GUI_SCREEN_REGISTRY_HPP

#include "app_selection_gui.hpp"
#include "communication_selection_gui.hpp"
#include "crash_gui.hpp"
#include "credits_gui.hpp"
#include "data_bundle_selection_gui.hpp"
#include "device_selection_gui.hpp"
#include "file_transfer_gui.hpp"
#include "gui_router.hpp"
#include "gui_state.hpp"
#include "library_editor_gui.hpp"
#include "library_gui.hpp"
#include "main_menu_gui.hpp"
#include "menu_gui.hpp"
#include "settings_gui.hpp"
#include "signals_visualization_gui.hpp"
#include "../helpers.hpp"
#include "../managers/data_bundle_manager.hpp"
#include "../managers/device_browser_state.hpp"
#include "../managers/device_catalog.hpp"
#include "../managers/device_manager.hpp"
#include "../managers/device_visualization_session.hpp"

class GuiScreenRegistry
{
private:
    MainMenuGui mainMenuGui;
    ConnectionGui connectionGui;
    SignalsVisualizationGui visualizationGui;
    DataBundleSelectionGui dataBundleSelectionGui;
    DeviceSelectionGui selectionGui;
    LibraryGui libraryGui;
    LibraryEditorGui libraryEditorGui;
    SettingsGui settingsGui;
    FileTransferGui fileTransferGui;
    CrashGui crashGui;
    CreditsGui creditsGui;
    AppSelectionGui appSelectionGui;
    CommunicationSelectionGui communicationSelectionGui;

public:
    GuiScreenRegistry(DeviceCatalog &catalog,
                      DeviceBrowserState &browserState,
                      GuiRouter &router,
                      DeviceManager &deviceManager,
                      DeviceVisualizationSession &visualizationSession,
                      DataBundleManager &dataBundleManager);

    void initializeCoreScreens();
    void hideAll();
    void render(GuiState targetState);

    MainMenuGui &getMainMenuGui() { return mainMenuGui; }
    ConnectionGui &getConnectionGui() { return connectionGui; }
    SignalsVisualizationGui &getVisualizationGui() { return visualizationGui; }
    DataBundleSelectionGui &getDataBundleSelectionGui() { return dataBundleSelectionGui; }
    DeviceSelectionGui &getSelectionGui() { return selectionGui; }
    LibraryGui &getLibraryGui() { return libraryGui; }
    LibraryEditorGui &getLibraryEditorGui() { return libraryEditorGui; }
    SettingsGui &getSettingsGui() { return settingsGui; }
    FileTransferGui &getFileTransferGui() { return fileTransferGui; }
    CrashGui &getCrashGui() { return crashGui; }
    CreditsGui &getCreditsGui() { return creditsGui; }
    AppSelectionGui &getAppSelectionGui() { return appSelectionGui; }
    CommunicationSelectionGui &getCommunicationSelectionGui() { return communicationSelectionGui; }
};

#endif // GUI_SCREEN_REGISTRY_HPP
