#include "library_gui.hpp"

#include <cstring>

LibraryGui::LibraryGui(DeviceCatalog &deviceCatalog, DeviceBrowserState &browserState, GuiRouter &router)
    : catalogBrowser(deviceCatalog), browserState(browserState), router(router)
{
}

void LibraryGui::build()
{
    const DeviceCatalogBrowserLayout layout = DeviceCatalogBrowserLayoutFactory::createLayout({
        "Library",
        0xFFFFFF,
        false
    });

    ui_Widget = layout.root;
    ui_DeviceList = layout.primaryList;
    ui_Detail = layout.detailPanel;
    ui_DetailLabel = layout.detailLabel;

    lv_obj_set_size(ui_DeviceList, 300, 285);
    lv_obj_set_pos(ui_DeviceList, 10, 50);
    lv_obj_set_size(ui_Detail, 430, 320);
    lv_obj_set_pos(ui_Detail, 320, 50);
    lv_obj_set_width(ui_DetailLabel, 410);

    lv_obj_t *back = DeviceCatalogBrowserLayoutFactory::createFooterButton(ui_Widget, "Back", LV_ALIGN_BOTTOM_LEFT, 16, -14);
    lv_obj_set_size(back, 90, 36);
    lv_obj_add_event_cb(back, [](lv_event_t *e) {
        if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
            auto *self = static_cast<LibraryGui *>(lv_event_get_user_data(e));
            self->router.showMainMenu();
        }
    }, LV_EVENT_ALL, this);

    lv_obj_t *edit = DeviceCatalogBrowserLayoutFactory::createFooterButton(ui_Widget, "Edit Entity", LV_ALIGN_BOTTOM_RIGHT, -140, -14);
    lv_obj_add_event_cb(edit, [](lv_event_t *e) {
        if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
            auto *self = static_cast<LibraryGui *>(lv_event_get_user_data(e));
            self->router.showLibraryEditor();
        }
    }, LV_EVENT_ALL, this);

    lv_obj_t *create = DeviceCatalogBrowserLayoutFactory::createFooterButton(ui_Widget, "New Entity", LV_ALIGN_BOTTOM_RIGHT, -12, -14);
    lv_obj_add_event_cb(create, [](lv_event_t *e) {
        if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
            auto *self = static_cast<LibraryGui *>(lv_event_get_user_data(e));
            self->router.prepareNewLibraryEntity();
            self->router.showLibraryEditor();
        }
    }, LV_EVENT_ALL, this);

    lv_obj_t *remove = DeviceCatalogBrowserLayoutFactory::createFooterButton(ui_Widget, "Delete", LV_ALIGN_BOTTOM_RIGHT, -268, -14);
    lv_obj_set_size(remove, 110, 36);
    lv_obj_add_event_cb(remove, [](lv_event_t *e) {
        if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
            auto *self = static_cast<LibraryGui *>(lv_event_get_user_data(e));
            self->handleDeleteButtonClick();
        }
    }, LV_EVENT_ALL, this);
    lv_obj_set_style_bg_color(remove, lv_color_hex(0xFF4136), 0);
}

void LibraryGui::populateDeviceList()
{
    if (!ui_DeviceList) {
        return;
    }

    lv_obj_clean(ui_DeviceList);

    const auto &sensors = catalogBrowser.getDevices();
    for (size_t i = 0; i < sensors.size(); ++i) {
        BaseDevice *sensor = sensors[i];
        const std::string label = DeviceCatalogBrowserFormatter::buildDeviceListLabel(sensor);
        lv_obj_t *button = lv_list_add_btn(ui_DeviceList, nullptr, label.c_str());
        DeviceCatalogBrowserRenderer::styleDeviceListButton(button);
        lv_obj_add_event_cb(button, [](lv_event_t *e) {
            if (lv_event_get_code(e) != LV_EVENT_CLICKED) {
                return;
            }

            auto *self = static_cast<LibraryGui *>(lv_event_get_user_data(e));
            int index = static_cast<int>(reinterpret_cast<intptr_t>(lv_obj_get_user_data(lv_event_get_current_target(e))));
            self->setSelectedDevice(index);
        }, LV_EVENT_ALL, this);
        lv_obj_set_user_data(button, reinterpret_cast<void *>(static_cast<intptr_t>(i)));
    }
}

void LibraryGui::updateDetail()
{
    if (!ui_Detail) {
        return;
    }

    BaseDevice *sensor = catalogBrowser.getSelectedDevice();
    const DeviceDefinitionSchema *draft = browserState.getLibraryDraft();

    if (draft && sensor && draft->uid == sensor->UID) {
        browserState.setLibraryDevice(sensor);
        DeviceCatalogBrowserRenderer::renderDeviceDetail(ui_Detail, sensor);
        return;
    }

    browserState.setLibraryDevice(sensor);
    DeviceCatalogBrowserRenderer::renderDeviceDetail(ui_Detail, sensor);
}

void LibraryGui::handleDeleteButtonClick()
{
    BaseDevice *device = catalogBrowser.getSelectedDevice();
    if (!device) {
        splashMessage("No entity selected.");
        return;
    }

    if (ui_DeleteDialog) {
        closeDeleteDialog();
    }

    static const char *buttons[] = {"Cancel", "Delete", ""};
    ui_DeleteDialog = lv_msgbox_create(lv_scr_act(), "Delete Entity", "Delete selected entity from catalog?", buttons, true);
    lv_obj_center(ui_DeleteDialog);
    lv_obj_move_foreground(ui_DeleteDialog);
    lv_obj_add_event_cb(ui_DeleteDialog, [](lv_event_t *e) {
        if (lv_event_get_code(e) != LV_EVENT_VALUE_CHANGED) {
            return;
        }

        auto *self = static_cast<LibraryGui *>(lv_event_get_user_data(e));
        const char *activeButton = lv_msgbox_get_active_btn_text(lv_event_get_current_target(e));
        if (!activeButton) {
            return;
        }

        if (strcmp(activeButton, "Delete") == 0) {
            self->handleDeleteConfirmButtonClick();
            return;
        }

        self->closeDeleteDialog();
    }, LV_EVENT_ALL, this);
}

void LibraryGui::handleDeleteConfirmButtonClick()
{
    BaseDevice *device = catalogBrowser.getSelectedDevice();
    if (!device) {
        closeDeleteDialog();
        return;
    }

    std::string error;
    const std::string uid = device->UID;
    if (!router.deleteLibraryEntity(uid, error)) {
        closeDeleteDialog();
        splashMessage(error.c_str());
        return;
    }

    closeDeleteDialog();
    catalogBrowser.setSelectedDeviceIndex(0);
    browserState.clearLibraryDraft();
    populateDeviceList();
    updateDetail();
}

void LibraryGui::closeDeleteDialog()
{
    if (!ui_DeleteDialog) {
        return;
    }

    lv_obj_del(ui_DeleteDialog);
    ui_DeleteDialog = nullptr;
}

void LibraryGui::init()
{
    if (initialized) {
        return;
    }

    build();
    initialized = true;
}

void LibraryGui::showLibrary()
{
    if (!initialized || !ui_Widget) {
        return;
    }

    BaseDevice *libraryDevice = browserState.getLibraryDevice();
    if (libraryDevice) {
        const auto &devices = catalogBrowser.getDevices();
        for (size_t i = 0; i < devices.size(); ++i) {
            if (devices[i] == libraryDevice) {
                catalogBrowser.setSelectedDeviceIndex(static_cast<int>(i));
                break;
            }
        }
    }

    populateDeviceList();
    updateDetail();
    lv_obj_clear_flag(ui_Widget, LV_OBJ_FLAG_HIDDEN);
}

void LibraryGui::hideLibrary()
{
    if (!initialized || !ui_Widget) {
        return;
    }

    closeDeleteDialog();
    lv_obj_add_flag(ui_Widget, LV_OBJ_FLAG_HIDDEN);
}

void LibraryGui::setSelectedDevice(int index)
{
    catalogBrowser.setSelectedDeviceIndex(index);
    updateDetail();
}
