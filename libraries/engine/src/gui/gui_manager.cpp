/**
 * @file gui_manager.cpp
 * @brief Implementation of the GuiManager class
 *
 * This source file implements the GuiManager functionality for
 * coordinating between MenuGui and SensorVisualizationGui components.
 *
 * @copyright 2025 MTA
 * @author Ing. Jiri Konecny, Ondřej Wrubel
 */

#include "gui_manager.hpp"
#include "../helpers.hpp"

const int FPS = 60;
const int CYCLE_DRAW_MS = (1000/FPS);
const int CYCLE_SYNC_MS = 100;

const int LOOP_SYNC_TH = CYCLE_SYNC_MS/CYCLE_DRAW_MS;
int LOOP_SYNC_COUNTER = LOOP_SYNC_TH;

GuiManager::GuiManager(SensorManager &manager, DataBundleManager &dataBundleManager) 
    : sensorManager(manager), 
      dataBundleManager(dataBundleManager),
      menuGui(manager), 
      vizGui(manager, dataBundleManager),
      dataBundleSelectionGui(dataBundleManager),
      wikiGui(manager),
      crashGui(),
      creditsGui(),
      appSelectionGui(),
      communicationSelectionGui(),
      currentState(GuiState::NONE),
      initialized(false) {
}

bool GuiManager::init(std::string configFile) {
    initialized = false;  
    currentState = GuiState::NONE;
    logMessage("Initializing GUI Manager...\n");

    try {
        // First initialize the crash GUI
        crashGui.init();

        // Ensure SensorManager is initialized
        if(!sensorManager.init(configFile))
        {
            crashGui.showCrash("SensorManager initialization failed!");
            return false;
        }             

        if(!dataBundleManager.init())
        {
            crashGui.showCrash("DataBundleManager initialization failed!");
            return false;
        }

        // Initialize all GUI components
        menuGui.init();
        vizGui.init();
        dataBundleSelectionGui.init();
        wikiGui.init();
        // These GUIs are initialized on demand
        // crashGui.init();
        // appSelectionGui.init();
        // communicationSelectionGui.init();
    }
    catch (const Exception &e) {
        showCrashScreen(e.flush());
        return false;
    }
    catch (const std::exception &e) {
        showCrashScreen(e.what());
        return false;
    }
    catch (...) {
        showCrashScreen("Unknown exception during GUI initialization!");
        return false;
    }

    currentState = GuiState::READY;
    initialized = true;
    logMessage("GUI Manager initialization completed!\n");
    return initialized;
}

bool GuiManager::init() {
    return init(""); // Call the overload with empty config
}

void GuiManager::hideAllComponents() {
    if (!initialized) {
        // logMessage("GuiManager not initialized, cannot hide components\n");
        return;
    }

    menuGui.hideMenu();
    vizGui.hideVisualization();
    dataBundleSelectionGui.hideDataBundles();
    wikiGui.hideWiki();
    crashGui.hideCrash();
    creditsGui.hideCredits();
    communicationSelectionGui.hideCommunicationSelection();
    appSelectionGui.hideAppSelection();
}

void GuiManager::showMenu() {
    if (!initialized) {
        // logMessage("GuiManager not initialized\n");
        return;
    }
    
    sensorManager.setRunning(false);
    hideAllComponents();
    menuGui.showMenu();
    currentState = GuiState::MENU;
    // logMessage("Switched to MENU state\n");
}

void GuiManager::showVisualization() {
    if (!initialized) {
        // logMessage("GuiManager not initialized\n");
        return;
    }
    
    sensorManager.setRunning(false);
    hideAllComponents();
    vizGui.showVisualization();
    sensorManager.setRunning(true);

    vizGui.drawCurrentSensor(); // Refresh display
    currentState = GuiState::VISUALIZATION;
    // logMessage("Switched to VISUALIZATION state\n");
}

void GuiManager::showDataBundleSelection() {
    if (!initialized) {
        // logMessage("GuiManager not initialized\n");
        return;
    }

    sensorManager.setRunning(false);
    hideAllComponents();
    
    //logMessage("Switched to DATA_BUNDLE_SELECTION state\n");
    
    dataBundleSelectionGui.showDataBundles();
    currentState = GuiState::DATA_BUNDLE_SELECTION;
}

void GuiManager::showWiki() {
    if (!initialized) {
        // logMessage("GuiManager not initialized\n");
        return;
    }

    sensorManager.setRunning(false);
    hideAllComponents();
    wikiGui.showWiki(menuGui.getActivePin());
    currentState = GuiState::WIKI;
    // logMessage("Switched to WIKI state\n");
}

void GuiManager::showCrashScreen(const std::string &reason) {
    sensorManager.setRunning(false);
    currentState = GuiState::CRASH;
    hideAllComponents();

    crashGui.showCrash(reason);   
}

void GuiManager::showCreditsScreen() {
    if (!initialized) {
        // logMessage("GuiManager not initialized\n");
        return;
    }

    sensorManager.setRunning(false);
    hideAllComponents();
    creditsGui.showCredits();
    currentState = GuiState::CREDITS;
    //logMessage("Switched to CREDITS state\n");
}

void GuiManager::showAppSelectionScreen() {
    if (!initialized) {
        // logMessage("GuiManager not initialized\n");
        return;
    }

    sensorManager.setRunning(false);
    hideAllComponents();
    // app selection gui is deleted after hidden    
    appSelectionGui.init();
    currentState = GuiState::APP_SELECTION;
    // logMessage("Switched to APP_SELECTION state\n");
}

void GuiManager::showCommunicationSelectionScreen() {
    if (!initialized) {
        // logMessage("GuiManager not initialized\n");
        return;
    }

    sensorManager.setRunning(false);
    hideAllComponents();
    // communication selectio gui is deleted after hidden
    communicationSelectionGui.init();
    currentState = GuiState::COMMUNICATION_SELECTION;
    // logMessage("Switched to COMMUNICATION_SELECTION state\n");
}

void GuiManager::switchContent(GuiState targetState) {
    if (!initialized) {
        // logMessage("GuiManager not initialized\n");
        return;
    }
    
    // Don't switch if already in target state
    if (currentState == targetState) {
        // logMessage("Already in target state %d\n", static_cast<int>(targetState));
        return;
    }
    
    switch (targetState) {
        case GuiState::MENU:
            showMenu();
            // logMessage("Switched content to MENU\n");
            break;
            
        case GuiState::VISUALIZATION:
            showVisualization();
            // logMessage("Switched content to VISUALIZATION\n");
            break;
            
        case GuiState::DATA_BUNDLE_SELECTION:
            showDataBundleSelection();
            // logMessage("DATA_BUNDLE_SELECTION not implemented, switched content to MENU\n");
            break;

        case GuiState::WIKI:
            showWiki();
            // Don't change sensor running state for wiki
            // logMessage("Switched content to WIKI\n");
            break;

        case GuiState::READY:
            hideAllComponents();
            sensorManager.setRunning(false);
            // logMessage("Switched content to NONE (all components hidden)\n");
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
            // logMessage("Unknown target GUI state %d, switching to MENU\n", static_cast<int>(targetState));
            splashMessage("Unknown target GUI state %d, nothing to display...\n", static_cast<int>(targetState));
            sensorManager.setRunning(false);
            break;
    }
}

void GuiManager::redraw() {
    lv_timer_handler();
    delay_ms(CYCLE_DRAW_MS);

    if (!initialized) {
        return;
    }

    // Sync sensor data periodically
    if (LOOP_SYNC_COUNTER-- < 0) {
        sensorManager.resync(); // Sync sensor data, if running
        LOOP_SYNC_COUNTER = LOOP_SYNC_TH;   
        delay_ms(1);
    }
    
    switch (currentState) {
        case GuiState::VISUALIZATION:
            // Redraw current sensor in visualization mode
            if (vizGui.isInitialized()) {
                vizGui.drawCurrentSensor();
            }
            break;
            
        case GuiState::DATA_BUNDLE_SELECTION:
            // Data bundle selection doesn't need periodic redraw - it's event-driven
            // Each bundle is added after the end of visualsiation recording
            break;

        case GuiState::MENU:
            // Menu doesn't need periodic redraw - it's event-driven
            break;
            
        case GuiState::WIKI:
            // Wiki doesn't need periodic redraw - it's event-driven
            break;
            
        case GuiState::CREDITS:
            // Credits doesn't need periodic redraw - it's static
            break;

        case GuiState::APP_SELECTION:
            // App selection doesn't need periodic redraw - it's event-driven
            break;

        case GuiState::COMMUNICATION_SELECTION:
            // Communication selection doesn't need periodic redraw - it's event-driven
            break;
            
        default:
            break;
    }
}

// All other functionality is accessed directly through the components
// GuiManager only handles initialization and screen switching, and redrawing of the current screen