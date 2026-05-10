#include "device_selection_gui.hpp"

#include "../helpers.hpp"

DeviceSelectionGui::DeviceSelectionGui(DeviceCatalog &deviceCatalog, DeviceBrowserState &browserState, GuiRouter &router, DeviceManager &deviceManager, DeviceVisualizationSession &visualizationSession)
    : catalogBrowser(deviceCatalog), browserState(browserState), router(router), deviceManager(deviceManager), visualizationSession(visualizationSession)
{
}

void DeviceSelectionGui::buildSelectionGui()
{
    const DeviceCatalogBrowserLayout layout = DeviceCatalogBrowserLayoutFactory::createLayout({
        "Selection",
        0xF5F5F5,
        true
    });

    ui_SelectionWidget = layout.root;
    ui_AvailableList = layout.primaryList;
    ui_SelectedList = layout.secondaryList;
    lv_obj_t *infoPanel = layout.detailPanel;
    ui_DeviceDescription = layout.detailLabel;

    ui_DeviceTitle = lv_label_create(infoPanel);
    lv_obj_set_width(ui_DeviceTitle, 270);
    lv_obj_set_pos(ui_DeviceTitle, 10, 10);
    lv_obj_set_style_text_font(ui_DeviceTitle, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_pos(ui_DeviceDescription, 10, 50);

    ui_DeviceSpecs = lv_label_create(infoPanel);
    lv_obj_set_width(ui_DeviceSpecs, 270);
    lv_obj_set_pos(ui_DeviceSpecs, 10, 150);
    lv_label_set_long_mode(ui_DeviceSpecs, LV_LABEL_LONG_WRAP);

    ui_btnBack = DeviceCatalogBrowserLayoutFactory::createFooterButton(ui_SelectionWidget, "Back", LV_ALIGN_BOTTOM_LEFT, 12, -16);
    lv_obj_set_size(ui_btnBack, 90, 36);
    lv_obj_add_event_cb(ui_btnBack, [](lv_event_t *e) {
        if (lv_event_get_code(e) != LV_EVENT_CLICKED) {
            return;
        }
        auto *self = static_cast<DeviceSelectionGui *>(lv_event_get_user_data(e));
        self->handleBackButtonClick();
    }, LV_EVENT_ALL, this);

    ui_btnConfigure = DeviceCatalogBrowserLayoutFactory::createFooterButton(ui_SelectionWidget, "Configure", LV_ALIGN_BOTTOM_MID, 0, -60);
    lv_obj_set_size(ui_btnConfigure, 145, 36);
    lv_obj_add_event_cb(ui_btnConfigure, [](lv_event_t *e) {
        if (lv_event_get_code(e) != LV_EVENT_CLICKED) {
            return;
        }
        auto *self = static_cast<DeviceSelectionGui *>(lv_event_get_user_data(e));
        self->handleConfigureButtonClick();
    }, LV_EVENT_ALL, this);

    ui_btnRemove = DeviceCatalogBrowserLayoutFactory::createFooterButton(ui_SelectionWidget, "Remove", LV_ALIGN_BOTTOM_MID, 0, -16);
    lv_obj_set_size(ui_btnRemove, 145, 36);
    lv_obj_set_style_bg_color(ui_btnRemove, lv_color_hex(0xD96464), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(ui_btnRemove, [](lv_event_t *e) {
        if (lv_event_get_code(e) != LV_EVENT_CLICKED) {
            return;
        }
        auto *self = static_cast<DeviceSelectionGui *>(lv_event_get_user_data(e));
        self->handleRemoveButtonClick();
    }, LV_EVENT_ALL, this);

    ui_btnStart = DeviceCatalogBrowserLayoutFactory::createFooterButton(ui_SelectionWidget, "Start Visualization", LV_ALIGN_BOTTOM_RIGHT, -10, -16);
    lv_obj_set_size(ui_btnStart, 145, 36);
    lv_obj_add_event_cb(ui_btnStart, [](lv_event_t *e) {
        if (lv_event_get_code(e) != LV_EVENT_CLICKED) {
            return;
        }
        auto *self = static_cast<DeviceSelectionGui *>(lv_event_get_user_data(e));
        self->handleStartButtonClick();
    }, LV_EVENT_ALL, this);
}

void DeviceSelectionGui::populateAvailableList()
{
    if (!ui_AvailableList) {
        return;
    }

    lv_obj_clean(ui_AvailableList);
    const auto &devices = catalogBrowser.getDevices();
    for (size_t i = 0; i < devices.size(); ++i) {
        BaseDevice *device = devices[i];
        const std::string label = DeviceCatalogBrowserFormatter::buildDeviceListLabel(device);
        lv_obj_t *button = lv_list_add_btn(ui_AvailableList, nullptr, label.c_str());
        lv_obj_set_user_data(button, reinterpret_cast<void *>(static_cast<intptr_t>(i)));
        lv_obj_add_event_cb(button, [](lv_event_t *e) {
            if (lv_event_get_code(e) != LV_EVENT_CLICKED) {
                return;
            }

            auto *self = static_cast<DeviceSelectionGui *>(lv_event_get_user_data(e));
            int index = static_cast<int>(reinterpret_cast<intptr_t>(lv_obj_get_user_data(lv_event_get_current_target(e))));
            self->handleDeviceSelection(index);
        }, LV_EVENT_ALL, this);
    }
}

void DeviceSelectionGui::populateSelectedList()
{
    if (!ui_SelectedList) {
        return;
    }

    lv_obj_clean(ui_SelectedList);
    lv_obj_t *header = lv_label_create(ui_SelectedList);
    lv_label_set_text(header, "Configured devices");

    const auto &devices = catalogBrowser.getDevices();
    for (BaseDevice *device : devices) {
        if (!device || device->getPins().empty()) {
            continue;
        }

        std::string entry = device->getTypeName() + " [" + device->getRoleLabel() + "] -> " + device->getPins();
        lv_list_add_text(ui_SelectedList, entry.c_str());
    }
}

void DeviceSelectionGui::updateDeviceInfo()
{
    BaseDevice *device = catalogBrowser.getSelectedDevice();
    if (!device) {
        lv_label_set_text(ui_DeviceTitle, "No device selected");
        lv_label_set_text(ui_DeviceDescription, "Choose a device from the list.");
        lv_label_set_text(ui_DeviceSpecs, "");
        return;
    }

    browserState.setSelectionDevice(device);
    lv_label_set_text(ui_DeviceTitle, device->getName().c_str());
    lv_label_set_text(ui_DeviceDescription, DeviceCatalogBrowserFormatter::buildSelectionInfoText(device).c_str());
    lv_label_set_text(ui_DeviceSpecs, DeviceCatalogBrowserFormatter::buildSelectionSpecsText(device).c_str());
}

void DeviceSelectionGui::updateStartButtonState()
{
    if (!ui_btnStart) {
        return;
    }

    if (deviceManager.hasAssignedDevices()) {
        lv_obj_clear_state(ui_btnStart, LV_STATE_DISABLED);
        return;
    }

    lv_obj_add_state(ui_btnStart, LV_STATE_DISABLED);
}

void DeviceSelectionGui::handleDeviceSelection(int deviceIndex)
{
    catalogBrowser.setSelectedDeviceIndex(deviceIndex);
    updateDeviceInfo();
}

void DeviceSelectionGui::handleConfigureButtonClick()
{
    BaseDevice *device = getSelectedDevice();
    if (!device) {
        splashMessage("Select a device first.");
        return;
    }

    browserState.setSelectionDevice(device);
    router.showConnection();
}

void DeviceSelectionGui::handleRemoveButtonClick()
{
    BaseDevice *device = getSelectedDevice();
    if (!device) {
        splashMessage("Select a configured device to remove.");
        return;
    }

    deviceManager.unassignAllPinsForDevice(device);
    populateSelectedList();
    updateStartButtonState();
}

void DeviceSelectionGui::handleStartButtonClick()
{
    deviceManager.setRunning(false);
    visualizationSession.setDevices(deviceManager.getAssignedDevices());
    browserState.setSelectionDevice(nullptr);

    if (!deviceManager.hasAssignedDevices()) {
        splashMessage("No devices configured.");
        return;
    }

    if (!deviceManager.connect()) {
        splashMessage("Error during device connection.");
        return;
    }

    router.showVisualization();
}

void DeviceSelectionGui::handleBackButtonClick()
{
    if (router.shouldSelectionBackGoToMainMenu()) {
        router.showMainMenu();
        return;
    }

    router.showCommunicationSelectionScreen();
}

void DeviceSelectionGui::init()
{
    if (initialized) {
        return;
    }

    buildSelectionGui();
    initialized = true;
}

void DeviceSelectionGui::showSelection()
{
    if (!initialized) {
        return;
    }

    populateAvailableList();
    populateSelectedList();
    updateDeviceInfo();
    updateStartButtonState();
    lv_obj_clear_flag(ui_SelectionWidget, LV_OBJ_FLAG_HIDDEN);
}

void DeviceSelectionGui::hideSelection()
{
    if (!initialized || !ui_SelectionWidget) {
        return;
    }

    lv_obj_add_flag(ui_SelectionWidget, LV_OBJ_FLAG_HIDDEN);
}

BaseDevice *DeviceSelectionGui::getSelectedDevice()
{
    return catalogBrowser.getSelectedDevice();
}

void DeviceSelectionGui::setSelectedDevice(int index)
{
    catalogBrowser.setSelectedDeviceIndex(index);
    updateDeviceInfo();
}
