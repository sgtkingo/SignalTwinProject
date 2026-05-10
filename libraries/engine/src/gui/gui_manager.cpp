/**
 * @file gui_manager.cpp
 * @brief Implementation of the GuiManager class
 */

#include "gui_manager.hpp"

#include "../helpers.hpp"
#include "../managers/storage_manager.hpp"

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
      navigationPolicy(),
      runtimePolicy(deviceManager),
      screenRegistry(deviceCatalog, deviceBrowserState, *this, deviceManager, visualizationSession, dataBundleManager),
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
        screenRegistry.getCrashGui().init();

        if (!storageManager().init()) {
            const std::string reason =
                std::string(storageManager().getStorageLabel()) +
                " storage mount failed.\nCheck the configured partition scheme and restart the device.";
            screenRegistry.getCrashGui().showCrash(reason.c_str(), "STORAGE ERROR", LV_SYMBOL_SD_CARD);
            return false;
        }

        if (!dataBundleManager.init()) {
            const std::string reason = std::string("Persistent storage is not ready on ") + storageManager().getStorageLabel() + ".";
            screenRegistry.getCrashGui().showCrash(reason.c_str(), "STORAGE ERROR", LV_SYMBOL_SD_CARD);
            return false;
        }

        if (!deviceCatalog.init(configFile)) {
            screenRegistry.getCrashGui().showCrash("DeviceCatalog initialization failed!");
            return false;
        }

        deviceBrowserState.clear();
        visualizationSession.clear();

        if (!deviceManager.init()) {
            screenRegistry.getCrashGui().showCrash("DeviceManager initialization failed!");
            return false;
        }

        screenRegistry.initializeCoreScreens();
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
    screenRegistry.hideAll();
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
        screenRegistry.getCrashGui().showCrash(reason);
        currentState = GuiState::CRASH;
        return;
    }

    navigateTo(GuiState::CRASH);
    screenRegistry.getCrashGui().showCrash(reason);
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
    runtimePolicy.apply(targetState);
}

void GuiManager::renderState(GuiState targetState)
{
    if (!initialized) {
        return;
    }

    screenRegistry.render(targetState);
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
    navigateTo(navigationPolicy.resolveBackTarget(currentState));
}

void GuiManager::switchContent(GuiState targetState)
{
    navigateTo(targetState);
}

void GuiManager::openVisualizationFlow()
{
    navigateTo(navigationPolicy.beginVisualizationFlow());
}

void GuiManager::completeCommunicationSelection(DefaultCommunicationMode mode)
{
    if (mode != DefaultCommunicationMode::CABLE) {
        splashMessage("Wireless connection is not supported yet.");
        return;
    }

    navigateTo(navigationPolicy.finishCommunicationSelection(mode));
}

void GuiManager::openDatabankFromMainMenu()
{
    navigateTo(navigationPolicy.openDatabankFromMainMenu());
}

void GuiManager::openDatabankFromVisualization()
{
    navigateTo(navigationPolicy.openDatabankFromVisualization());
}

void GuiManager::navigateBackFromDatabank()
{
    navigateBack();
}

void GuiManager::prepareNewLibraryEntity()
{
    deviceBrowserState.beginNewLibraryDraft();
}

bool GuiManager::saveLibraryDraft(const DeviceDefinitionSchema &draft,
                                  const std::string &originalUid,
                                  bool isNewEntity,
                                  std::string &error)
{
    try {
        deviceCatalog.saveDraft(draft, originalUid, isNewEntity);
        deviceManager.erase();
        visualizationSession.clear();
        deviceBrowserState.clear();
        deviceBrowserState.setLibraryDevice(deviceCatalog.getDevice(draft.uid));
        deviceBrowserState.setLibraryDraft(draft, false);
        return true;
    } catch (const std::exception &e) {
        error = e.what();
        return false;
    } catch (...) {
        error = "Unknown error while saving device draft.";
        return false;
    }
}

bool GuiManager::deleteLibraryEntity(const std::string &uid, std::string &error)
{
    try {
        deviceCatalog.deleteDevice(uid);
        deviceManager.erase();
        visualizationSession.clear();
        deviceBrowserState.clear();
        return true;
    } catch (const std::exception &e) {
        error = e.what();
        return false;
    } catch (...) {
        error = "Unknown error while deleting device entity.";
        return false;
    }
}

bool GuiManager::saveCatalogMetadata(const std::string &application,
                                     const std::string &version,
                                     std::string &error)
{
    try {
        BaseDevice *selectedSelectionDevice = deviceBrowserState.getSelectionDevice();
        BaseDevice *selectedLibraryDevice = deviceBrowserState.getLibraryDevice();
        const std::string selectionUid = selectedSelectionDevice ? selectedSelectionDevice->UID : "";
        const std::string libraryUid = selectedLibraryDevice ? selectedLibraryDevice->UID : "";

        deviceCatalog.saveMetadata(application, version);
        deviceManager.erase();
        visualizationSession.clear();
        deviceBrowserState.clear();
        if (!selectionUid.empty()) {
            deviceBrowserState.setSelectionDevice(deviceCatalog.getDevice(selectionUid));
        }
        if (!libraryUid.empty()) {
            deviceBrowserState.setLibraryDevice(deviceCatalog.getDevice(libraryUid));
        }
        return true;
    } catch (const std::exception &e) {
        error = e.what();
        return false;
    } catch (...) {
        error = "Unknown error while saving catalog metadata.";
        return false;
    }
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
        if (screenRegistry.getVisualizationGui().isInitialized()) {
            screenRegistry.getVisualizationGui().drawCurrentDevice();
        }
        break;
    default:
        break;
    }
}
