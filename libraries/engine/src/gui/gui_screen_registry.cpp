#include "gui_screen_registry.hpp"

GuiScreenRegistry::GuiScreenRegistry(DeviceCatalog &catalog,
                                     DeviceBrowserState &browserState,
                                     GuiRouter &router,
                                     DeviceManager &deviceManager,
                                     DeviceVisualizationSession &visualizationSession,
                                     DataBundleManager &dataBundleManager)
    : mainMenuGui(router),
      connectionGui(browserState, router, deviceManager),
      visualizationGui(router, deviceManager, visualizationSession, dataBundleManager),
      dataBundleSelectionGui(router, dataBundleManager),
      selectionGui(catalog, browserState, router, deviceManager, visualizationSession),
      libraryGui(catalog, browserState, router),
      libraryEditorGui(catalog, browserState, router),
      settingsGui(router),
      crashGui(),
      creditsGui(router),
      appSelectionGui(router),
      communicationSelectionGui(router, deviceManager)
{
}

void GuiScreenRegistry::initializeCoreScreens()
{
    crashGui.init();
    mainMenuGui.init();
    connectionGui.init();
    visualizationGui.init();
    dataBundleSelectionGui.init();
    selectionGui.init();
    libraryGui.init();
    libraryEditorGui.init();
    settingsGui.init();
}

void GuiScreenRegistry::hideAll()
{
    mainMenuGui.hideMainMenu();
    connectionGui.hideConnection();
    visualizationGui.hideVisualization();
    dataBundleSelectionGui.hideDataBundles();
    selectionGui.hideSelection();
    libraryGui.hideLibrary();
    libraryEditorGui.hideEditor();
    settingsGui.hideSettings();
    crashGui.hideCrash();
    creditsGui.hideCredits();
    communicationSelectionGui.hideCommunicationSelection();
    appSelectionGui.hideAppSelection();
}

void GuiScreenRegistry::render(GuiState targetState)
{
    hideAll();

    switch (targetState) {
    case GuiState::MAIN_MENU:
        mainMenuGui.showMainMenu();
        break;
    case GuiState::CONNECTION:
        connectionGui.showConnection();
        break;
    case GuiState::VISUALIZATION:
        visualizationGui.showVisualization();
        visualizationGui.drawCurrentDevice();
        break;
    case GuiState::DATA_BUNDLE_SELECTION:
        dataBundleSelectionGui.showDataBundles();
        break;
    case GuiState::SELECTION:
        selectionGui.showSelection();
        break;
    case GuiState::LIBRARY:
        libraryGui.showLibrary();
        break;
    case GuiState::LIBRARY_EDITOR:
        libraryEditorGui.showEditor();
        break;
    case GuiState::SETTINGS:
        settingsGui.showSettings();
        break;
    case GuiState::READY:
    case GuiState::CRASH:
        break;
    case GuiState::CREDITS:
        creditsGui.showCredits();
        break;
    case GuiState::APP_SELECTION:
        appSelectionGui.init();
        break;
    case GuiState::COMMUNICATION_SELECTION:
        communicationSelectionGui.init();
        break;
    default:
        splashMessage("Unknown target GUI state %d, nothing to display...\n", static_cast<int>(targetState));
        break;
    }
}
