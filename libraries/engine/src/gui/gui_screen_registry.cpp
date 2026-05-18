#include "gui_screen_registry.hpp"
#include "expt.hpp"

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
      fileTransferGui(router),
      crashGui(),
      creditsGui(router),
      appSelectionGui(router),
      communicationSelectionGui(router, deviceManager)
{
}

void GuiScreenRegistry::initializeCoreScreens()
{
    debugLogMessage("GuiScreenRegistry::initializeCoreScreens", "gui init", "initializing core screens");
    debugLogMessage("GuiScreenRegistry::initializeCoreScreens", "gui init", "init crashGui");
    crashGui.init();
    debugLogMessage("GuiScreenRegistry::initializeCoreScreens", "gui init", "init crashGui done");

    debugLogMessage("GuiScreenRegistry::initializeCoreScreens", "gui init", "init mainMenuGui");
    mainMenuGui.init();
    debugLogMessage("GuiScreenRegistry::initializeCoreScreens", "gui init", "init mainMenuGui done");

    debugLogMessage("GuiScreenRegistry::initializeCoreScreens", "gui init", "init connectionGui");
    connectionGui.init();
    debugLogMessage("GuiScreenRegistry::initializeCoreScreens", "gui init", "init connectionGui done");

    debugLogMessage("GuiScreenRegistry::initializeCoreScreens", "gui init", "init visualizationGui");
    visualizationGui.init();
    debugLogMessage("GuiScreenRegistry::initializeCoreScreens", "gui init", "init visualizationGui done");

    debugLogMessage("GuiScreenRegistry::initializeCoreScreens", "gui init", "init dataBundleSelectionGui");
    dataBundleSelectionGui.init();
    debugLogMessage("GuiScreenRegistry::initializeCoreScreens", "gui init", "init dataBundleSelectionGui done");

    debugLogMessage("GuiScreenRegistry::initializeCoreScreens", "gui init", "init selectionGui");
    selectionGui.init();
    debugLogMessage("GuiScreenRegistry::initializeCoreScreens", "gui init", "init selectionGui done");

    debugLogMessage("GuiScreenRegistry::initializeCoreScreens", "gui init", "init libraryGui");
    libraryGui.init();
    debugLogMessage("GuiScreenRegistry::initializeCoreScreens", "gui init", "init libraryGui done");

    debugLogMessage("GuiScreenRegistry::initializeCoreScreens", "gui init", "init libraryEditorGui");
    libraryEditorGui.init();
    debugLogMessage("GuiScreenRegistry::initializeCoreScreens", "gui init", "init libraryEditorGui done");

    debugLogMessage("GuiScreenRegistry::initializeCoreScreens", "gui init", "init settingsGui");
    settingsGui.init();
    debugLogMessage("GuiScreenRegistry::initializeCoreScreens", "gui init", "init settingsGui done");

    debugLogMessage("GuiScreenRegistry::initializeCoreScreens", "gui init", "init fileTransferGui");
    fileTransferGui.init();
    debugLogMessage("GuiScreenRegistry::initializeCoreScreens", "gui init", "init fileTransferGui done");
}

void GuiScreenRegistry::hideAll()
{
    debugLogMessage("GuiScreenRegistry::hideAll", "gui operation", "hiding all screens");
    mainMenuGui.hideMainMenu();
    connectionGui.hideConnection();
    visualizationGui.hideVisualization();
    dataBundleSelectionGui.hideDataBundles();
    selectionGui.hideSelection();
    libraryGui.hideLibrary();
    libraryEditorGui.hideEditor();
    settingsGui.hideSettings();
    fileTransferGui.hideFileTransfer();
    crashGui.hideCrash();
    creditsGui.hideCredits();
    communicationSelectionGui.hideCommunicationSelection();
    appSelectionGui.hideAppSelection();
}

void GuiScreenRegistry::render(GuiState targetState)
{
    debugLogMessage("GuiScreenRegistry::render", "gui operation", "targetState=%d", static_cast<int>(targetState));
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
    case GuiState::FILE_TRANSFER:
        fileTransferGui.showFileTransfer();
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
        debugLogMessage("GuiScreenRegistry::render", "gui state invalid", "unknown target state=%d", static_cast<int>(targetState));
        splashMessage("Unknown target GUI state %d, nothing to display...\n", static_cast<int>(targetState));
        break;
    }
}
