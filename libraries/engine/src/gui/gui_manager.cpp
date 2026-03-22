/**
 * @file gui_manager.cpp
 * @brief Implementation of the GuiManager class
 */

#include "gui_manager.hpp"

#include "../helpers.hpp"

const int FPS = 60;
const int CYCLE_DRAW_MS = (1000 / FPS);
const int CYCLE_SYNC_MS = 100;

const int LOOP_SYNC_TH = CYCLE_SYNC_MS / CYCLE_DRAW_MS;
int LOOP_SYNC_COUNTER = LOOP_SYNC_TH;

GuiManager::GuiManager(DeviceCatalog &catalog, DeviceBrowserState &browserState, DeviceManager &manager, DeviceVisualizationSession &visualizationSession, DataBundleManager &dataBundleManager)
    : deviceCatalog(catalog),
      deviceBrowserState(browserState),
      deviceManager(manager),
      visualizationSession(visualizationSession),
      dataBundleManager(dataBundleManager),
      mainMenuGui(),
      connectionGui(browserState, manager),
      vizGui(manager, visualizationSession, dataBundleManager),
      dataBundleSelectionGui(dataBundleManager),
      selectionGui(catalog, browserState, manager, visualizationSession),
      libraryGui(catalog, browserState),
      libraryEditorGui(browserState),
      settingsGui(),
      crashGui(),
      creditsGui(),
      appSelectionGui(),
      communicationSelectionGui(),
      currentState(GuiState::NONE),
      initialized(false)
{
}

bool GuiManager::init(std::string configFile)
{
    initialized = false;
    currentState = GuiState::NONE;
    logMessage("Initializing GUI Manager...\n");

    try {
        crashGui.init();

        if (!dataBundleManager.init()) {
            crashGui.showCrash("SD card missing or not readable.\nInsert SD card and restart the device.", "SD ERROR", LV_SYMBOL_SD_CARD);
            return false;
        }

        if (!deviceCatalog.init(configFile)) {
            crashGui.showCrash("DeviceCatalog initialization failed!");
            return false;
        }

        deviceBrowserState.clear();
        visualizationSession.clear();

        if (!deviceManager.init()) {
            crashGui.showCrash("DeviceManager initialization failed!");
            return false;
        }

        mainMenuGui.init();
        connectionGui.init();
        vizGui.init();
        dataBundleSelectionGui.init();
        selectionGui.init();
        libraryGui.init();
        libraryEditorGui.init();
        settingsGui.init();
    } catch (const Exception &e) {
        showCrashScreen(e.flush());
        return false;
    } catch (const std::exception &e) {
        showCrashScreen(e.what());
        return false;
    } catch (...) {
        showCrashScreen("Unknown exception during GUI initialization!");
        return false;
    }

    currentState = GuiState::READY;
    initialized = true;
    logMessage("GUI Manager initialization completed!\n");
    return initialized;
}

bool GuiManager::init()
{
    return init("");
}

void GuiManager::hideAllComponents()
{
    if (!initialized) {
        return;
    }

    mainMenuGui.hideMainMenu();
    connectionGui.hideConnection();
    vizGui.hideVisualization();
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

void GuiManager::showMainMenu()
{
    if (!initialized) {
        return;
    }

    navigateTo(GuiState::MAIN_MENU);
}

void GuiManager::showConnection()
{
    if (!initialized) {
        return;
    }

    navigateTo(GuiState::CONNECTION);
}

void GuiManager::showMenu()
{
    showConnection();
}

void GuiManager::showVisualization()
{
    if (!initialized) {
        return;
    }

    navigateTo(GuiState::VISUALIZATION);
}

void GuiManager::showDataBundleSelection()
{
    if (!initialized) {
        return;
    }

    navigateTo(GuiState::DATA_BUNDLE_SELECTION);
}

void GuiManager::showSelection()
{
    if (!initialized) {
        return;
    }

    navigateTo(GuiState::SELECTION);
}

void GuiManager::showLibrary()
{
    if (!initialized) {
        return;
    }

    navigateTo(GuiState::LIBRARY);
}

void GuiManager::showLibraryEditor()
{
    if (!initialized) {
        return;
    }

    navigateTo(GuiState::LIBRARY_EDITOR);
}

void GuiManager::showSettings()
{
    if (!initialized) {
        return;
    }

    navigateTo(GuiState::SETTINGS);
}

void GuiManager::showCrashScreen(const std::string &reason)
{
    if (!initialized) {
        deviceManager.setRunning(false);
        hideAllComponents();
        crashGui.showCrash(reason);
        currentState = GuiState::CRASH;
        return;
    }

    navigateTo(GuiState::CRASH);
    crashGui.showCrash(reason);
}

void GuiManager::showCreditsScreen()
{
    if (!initialized) {
        return;
    }

    navigateTo(GuiState::CREDITS);
}

void GuiManager::showAppSelectionScreen()
{
    if (!initialized) {
        return;
    }

    navigateTo(GuiState::APP_SELECTION);
}

void GuiManager::showCommunicationSelectionScreen()
{
    if (!initialized) {
        return;
    }

    navigateTo(GuiState::COMMUNICATION_SELECTION);
}

void GuiManager::applyRuntimePolicy(GuiState targetState)
{
    const bool shouldRunRuntime = targetState == GuiState::VISUALIZATION;
    deviceManager.setRunning(shouldRunRuntime);
}

void GuiManager::renderState(GuiState targetState)
{
    if (!initialized) {
        return;
    }

    hideAllComponents();

    switch (targetState) {
    case GuiState::MAIN_MENU:
        mainMenuGui.showMainMenu();
        break;
    case GuiState::CONNECTION:
        connectionGui.showConnection();
        break;
    case GuiState::VISUALIZATION:
        vizGui.showVisualization();
        vizGui.drawCurrentDevice();
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
        break;
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

GuiState GuiManager::resolveBackTarget(GuiState fromState) const
{
    switch (fromState) {
    case GuiState::DATA_BUNDLE_SELECTION:
        return databankReturnToVisualization ? GuiState::VISUALIZATION : GuiState::MAIN_MENU;
    case GuiState::VISUALIZATION:
        return GuiState::SELECTION;
    case GuiState::CONNECTION:
        return GuiState::SELECTION;
    case GuiState::SELECTION:
        return selectionBackToMainMenu ? GuiState::MAIN_MENU : GuiState::COMMUNICATION_SELECTION;
    case GuiState::COMMUNICATION_SELECTION:
    case GuiState::LIBRARY:
    case GuiState::SETTINGS:
        return GuiState::MAIN_MENU;
    case GuiState::LIBRARY_EDITOR:
        return GuiState::LIBRARY;
    case GuiState::CREDITS:
        return GuiState::VISUALIZATION;
    default:
        return GuiState::MAIN_MENU;
    }
}

void GuiManager::navigateTo(GuiState targetState)
{
    if (!initialized) {
        return;
    }

    if (currentState == targetState) {
        return;
    }

    applyRuntimePolicy(targetState);
    renderState(targetState);
    currentState = targetState;
}

void GuiManager::navigateBack()
{
    navigateTo(resolveBackTarget(currentState));
}

void GuiManager::switchContent(GuiState targetState)
{
    navigateTo(targetState);
}

void GuiManager::openVisualizationFlow()
{
    if (defaultCommunicationMode == DefaultCommunicationMode::CABLE) {
        sessionCommunicationMode = DefaultCommunicationMode::CABLE;
        selectionBackToMainMenu = true;
        navigateTo(GuiState::SELECTION);
        return;
    }

    selectionBackToMainMenu = false;
    navigateTo(GuiState::COMMUNICATION_SELECTION);
}

void GuiManager::completeCommunicationSelection(DefaultCommunicationMode mode)
{
    sessionCommunicationMode = mode;
    selectionBackToMainMenu = false;
    navigateTo(GuiState::SELECTION);
}

void GuiManager::openDatabankFromMainMenu()
{
    databankReturnToVisualization = false;
    navigateTo(GuiState::DATA_BUNDLE_SELECTION);
}

void GuiManager::openDatabankFromVisualization()
{
    databankReturnToVisualization = true;
    navigateTo(GuiState::DATA_BUNDLE_SELECTION);
}

void GuiManager::navigateBackFromDatabank()
{
    navigateBack();
}

void GuiManager::prepareNewLibraryEntity()
{
    deviceBrowserState.setLibraryDevice(nullptr);
}

void GuiManager::redraw()
{
    lv_timer_handler();
    delay_ms(CYCLE_DRAW_MS);

    if (!initialized) {
        return;
    }

    if (LOOP_SYNC_COUNTER-- < 0) {
        deviceManager.resync(visualizationSession.getCurrentDevice());
        LOOP_SYNC_COUNTER = LOOP_SYNC_TH;
        delay_ms(1);
    }

    switch (currentState) {
    case GuiState::VISUALIZATION:
        if (vizGui.isInitialized()) {
            vizGui.drawCurrentDevice();
        }
        break;
    default:
        break;
    }
}
