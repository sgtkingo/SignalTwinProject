/**
 * @file gui_manager.hpp
 * @brief Declaration of the GuiManager class
 */

#ifndef GUI_MANAGER_HPP
#define GUI_MANAGER_HPP

#include "../managers/data_bundle_manager.hpp"
#include "../managers/device_browser_state.hpp"
#include "../managers/device_catalog.hpp"
#include "../managers/device_manager.hpp"
#include "../managers/device_visualization_session.hpp"
#include "app_settings.hpp"
#include "gui_navigation_policy.hpp"
#include "gui_router.hpp"
#include "gui_runtime_policy.hpp"
#include "gui_screen_registry.hpp"
#include "gui_state.hpp"

class GuiManager : public GuiRouter
{
private:
    DeviceCatalog &deviceCatalog;
    DeviceBrowserState &deviceBrowserState;
    DeviceManager &deviceManager;
    DeviceVisualizationSession &visualizationSession;
    DataBundleManager &dataBundleManager;
    GuiNavigationPolicy navigationPolicy;
    GuiRuntimePolicy runtimePolicy;
    GuiScreenRegistry screenRegistry;

    GuiState currentState;
    bool initialized;

    void hideAllComponents();
    void applyRuntimePolicy(GuiState targetState);
    void renderState(GuiState targetState);

public:
    explicit GuiManager(DeviceCatalog &catalog, DeviceBrowserState &browserState, DeviceManager &manager, DeviceVisualizationSession &visualizationSession, DataBundleManager &dataBundleManager);

    bool init();
    bool init(std::string configFile);
    bool isInitialized() const { return initialized; }

    GuiState getCurrentState() const { return currentState; }

    void showMainMenu() override;
    void showConnection() override;
    void showMenu();
    void showVisualization() override;
    void showDataBundleSelection();
    void showSelection() override;
    void showLibrary() override;
    void showLibraryEditor() override;
    void showSettings() override;
    void showCrashScreen(const std::string &reason = "Unexpected error");
    void showCreditsScreen() override;
    void showAppSelectionScreen() override;
    void showCommunicationSelectionScreen() override;

    void navigateTo(GuiState targetState);
    void navigateBack();
    void switchContent(GuiState targetState);
    void redraw();

    void openVisualizationFlow() override;
    void completeCommunicationSelection(DefaultCommunicationMode mode) override;
    void openDatabankFromMainMenu() override;
    void openDatabankFromVisualization() override;
    void navigateBackFromDatabank() override;
    void prepareNewLibraryEntity() override;
    bool saveLibraryDraft(const DeviceDefinitionSchema &draft,
                          const std::string &originalUid,
                          bool isNewEntity,
                          std::string &error) override;
    bool deleteLibraryEntity(const std::string &uid, std::string &error) override;
    bool saveCatalogMetadata(const std::string &application,
                             const std::string &version,
                             std::string &error) override;
    bool shouldSelectionBackGoToMainMenu() const override { return navigationPolicy.shouldSelectionBackGoToMainMenu(); }

    DefaultCommunicationMode getDefaultCommunicationMode() const override { return navigationPolicy.getDefaultCommunicationMode(); }
    void setDefaultCommunicationMode(DefaultCommunicationMode mode) override { navigationPolicy.setDefaultCommunicationMode(mode); }
    std::string getCatalogApplication() const override { return deviceCatalog.getApplication(); }
    std::string getCatalogVersion() const override { return deviceCatalog.getVersion(); }

    ConnectionGui &getConnectionGui() { return screenRegistry.getConnectionGui(); }
    MenuGui &getMenuGui() { return screenRegistry.getConnectionGui(); }
    SignalsVisualizationGui &getVisualizationGui() { return screenRegistry.getVisualizationGui(); }
    DataBundleSelectionGui &getDataBundleSelectionGui() { return screenRegistry.getDataBundleSelectionGui(); }
    DeviceSelectionGui &getSelectionGui() { return screenRegistry.getSelectionGui(); }
    DeviceSelectionGui &getWikiGui() { return screenRegistry.getSelectionGui(); }
    CreditsGui &getCreditsGui() { return screenRegistry.getCreditsGui(); }
    DeviceCatalog &getDeviceCatalog() { return deviceCatalog; }
    DeviceBrowserState &getDeviceBrowserState() { return deviceBrowserState; }
    DeviceManager &getDeviceManager() { return deviceManager; }
    DataBundleManager &getDataBundleManager() { return dataBundleManager; }
    AppSelectionGui &getAppSelectionGui() { return screenRegistry.getAppSelectionGui(); }
    CommunicationSelectionGui &getCommunicationSelectionGui() { return screenRegistry.getCommunicationSelectionGui(); }
};

#endif // GUI_MANAGER_HPP
