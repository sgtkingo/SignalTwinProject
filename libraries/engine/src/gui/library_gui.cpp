#include "library_gui.hpp"

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
        lv_obj_add_event_cb(button, [](lv_event_t *e) {
            if (lv_event_get_code(e) != LV_EVENT_CLICKED) {
                return;
            }

            auto *self = static_cast<LibraryGui *>(lv_event_get_user_data(e));
            int index = static_cast<int>(reinterpret_cast<intptr_t>(lv_obj_get_user_data(lv_event_get_target(e))));
            self->setSelectedDevice(index);
        }, LV_EVENT_ALL, this);
        lv_obj_set_user_data(button, reinterpret_cast<void *>(static_cast<intptr_t>(i)));
    }
}

void LibraryGui::updateDetail()
{
    if (!ui_DetailLabel) {
        return;
    }

    BaseDevice *sensor = catalogBrowser.getSelectedDevice();
    if (!sensor) {
        lv_label_set_text(ui_DetailLabel, "No entity selected.");
        return;
    }

    browserState.setLibraryDevice(sensor);
    lv_label_set_text(ui_DetailLabel, DeviceCatalogBrowserFormatter::buildLibraryDetailText(sensor).c_str());
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

    populateDeviceList();
    updateDetail();
    lv_obj_clear_flag(ui_Widget, LV_OBJ_FLAG_HIDDEN);
}

void LibraryGui::hideLibrary()
{
    if (!initialized || !ui_Widget) {
        return;
    }

    lv_obj_add_flag(ui_Widget, LV_OBJ_FLAG_HIDDEN);
}

void LibraryGui::setSelectedDevice(int index)
{
    catalogBrowser.setSelectedDeviceIndex(index);
    updateDetail();
}
