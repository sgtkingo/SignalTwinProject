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

GuiManager::GuiManager(SensorManager &manager, DataBundleManager &dataBundleManager)
    : sensorManager(manager),
      dataBundleManager(dataBundleManager),
      mainMenuGui(),
      menuGui(manager),
      vizGui(manager, dataBundleManager),
      dataBundleSelectionGui(dataBundleManager),
      wikiGui(manager),
      libraryGui(manager),
      libraryEditorGui(manager),
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

        if (!sensorManager.init(configFile)) {
            crashGui.showCrash("SensorManager initialization failed!");
            return false;
        }

        mainMenuGui.init();
        menuGui.init();
        vizGui.init();
        dataBundleSelectionGui.init();
        wikiGui.init();
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
    menuGui.hideMenu();
    vizGui.hideVisualization();
    dataBundleSelectionGui.hideDataBundles();
    wikiGui.hideWiki();
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

    sensorManager.setRunning(false);
    hideAllComponents();
    mainMenuGui.showMainMenu();
    currentState = GuiState::MAIN_MENU;
}

void GuiManager::showMenu()
{
    if (!initialized) {
        return;
    }

    sensorManager.setRunning(false);
    hideAllComponents();
    menuGui.showMenu();
    currentState = GuiState::CONNECTION;
}

void GuiManager::showVisualization()
{
    if (!initialized) {
        return;
    }

    sensorManager.setRunning(false);
    hideAllComponents();
    vizGui.showVisualization();
    sensorManager.setRunning(true);

    vizGui.drawCurrentSensor();
    currentState = GuiState::VISUALIZATION;
}

void GuiManager::showDataBundleSelection()
{
    if (!initialized) {
        return;
    }

    sensorManager.setRunning(false);
    hideAllComponents();
    dataBundleSelectionGui.showDataBundles();
    currentState = GuiState::DATA_BUNDLE_SELECTION;
}

void GuiManager::showWiki()
{
    if (!initialized) {
        return;
    }

    sensorManager.setRunning(false);
    hideAllComponents();
    wikiGui.showWiki(menuGui.getActivePin());
    currentState = GuiState::SELECTION;
}

void GuiManager::showLibrary()
{
    if (!initialized) {
        return;
    }

    sensorManager.setRunning(false);
    hideAllComponents();
    libraryGui.showLibrary();
    currentState = GuiState::LIBRARY;
}

void GuiManager::showLibraryEditor()
{
    if (!initialized) {
        return;
    }

    sensorManager.setRunning(false);
    hideAllComponents();
    libraryEditorGui.showEditor();
    currentState = GuiState::LIBRARY_EDITOR;
}

void GuiManager::showSettings()
{
    if (!initialized) {
        return;
    }

    sensorManager.setRunning(false);
    hideAllComponents();
    settingsGui.showSettings();
    currentState = GuiState::SETTINGS;
}

void GuiManager::showCrashScreen(const std::string &reason)
{
    sensorManager.setRunning(false);
    currentState = GuiState::CRASH;
    hideAllComponents();
    crashGui.showCrash(reason);
}

void GuiManager::showCreditsScreen()
{
    if (!initialized) {
        return;
    }

    sensorManager.setRunning(false);
    hideAllComponents();
    creditsGui.showCredits();
    currentState = GuiState::CREDITS;
}

void GuiManager::showAppSelectionScreen()
{
    if (!initialized) {
        return;
    }

    sensorManager.setRunning(false);
    hideAllComponents();
    appSelectionGui.init();
    currentState = GuiState::APP_SELECTION;
}

void GuiManager::showCommunicationSelectionScreen()
{
    if (!initialized) {
        return;
    }

    sensorManager.setRunning(false);
    hideAllComponents();
    communicationSelectionGui.init();
    currentState = GuiState::COMMUNICATION_SELECTION;
}

void GuiManager::switchContent(GuiState targetState)
{
    if (!initialized) {
        return;
    }

    if (currentState == targetState) {
        return;
    }

    switch (targetState) {
    case GuiState::MAIN_MENU:
        showMainMenu();
        break;
    case GuiState::CONNECTION:
        showMenu();
        break;
    case GuiState::VISUALIZATION:
        showVisualization();
        break;
    case GuiState::DATA_BUNDLE_SELECTION:
        showDataBundleSelection();
        break;
    case GuiState::SELECTION:
        showWiki();
        break;
    case GuiState::LIBRARY:
        showLibrary();
        break;
    case GuiState::LIBRARY_EDITOR:
        showLibraryEditor();
        break;
    case GuiState::SETTINGS:
        showSettings();
        break;
    case GuiState::READY:
        hideAllComponents();
        sensorManager.setRunning(false);
        break;
    case GuiState::CRASH:
        showCrashScreen("Unexpected error");
        break;
    case GuiState::CREDITS:
        showCreditsScreen();
        break;
    case GuiState::APP_SELECTION:
        showAppSelectionScreen();
        break;
    case GuiState::COMMUNICATION_SELECTION:
        showCommunicationSelectionScreen();
        break;
    default:
        splashMessage("Unknown target GUI state %d, nothing to display...\n", static_cast<int>(targetState));
        sensorManager.setRunning(false);
        break;
    }
}

void GuiManager::openVisualizationFlow()
{
    if (defaultCommunicationMode == DefaultCommunicationMode::CABLE) {
        sessionCommunicationMode = DefaultCommunicationMode::CABLE;
        selectionBackToMainMenu = true;
        switchContent(GuiState::SELECTION);
        return;
    }

    selectionBackToMainMenu = false;
    switchContent(GuiState::COMMUNICATION_SELECTION);
}

void GuiManager::completeCommunicationSelection(DefaultCommunicationMode mode)
{
    sessionCommunicationMode = mode;
    selectionBackToMainMenu = false;
    switchContent(GuiState::SELECTION);
}

void GuiManager::openDatabankFromMainMenu()
{
    databankReturnToVisualization = false;
    switchContent(GuiState::DATA_BUNDLE_SELECTION);
}

void GuiManager::openDatabankFromVisualization()
{
    databankReturnToVisualization = true;
    switchContent(GuiState::DATA_BUNDLE_SELECTION);
}

void GuiManager::navigateBackFromDatabank()
{
    if (databankReturnToVisualization) {
        switchContent(GuiState::VISUALIZATION);
        return;
    }

    switchContent(GuiState::MAIN_MENU);
}

void GuiManager::redraw()
{
    lv_timer_handler();
    delay_ms(CYCLE_DRAW_MS);

    if (!initialized) {
        return;
    }

    if (LOOP_SYNC_COUNTER-- < 0) {
        sensorManager.resync();
        LOOP_SYNC_COUNTER = LOOP_SYNC_TH;
        delay_ms(1);
    }

    switch (currentState) {
    case GuiState::VISUALIZATION:
        if (vizGui.isInitialized()) {
            vizGui.drawCurrentSensor();
        }
        break;
    default:
        break;
    }
}
