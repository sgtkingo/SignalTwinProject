#ifndef LIBRARY_GUI_HPP
#define LIBRARY_GUI_HPP

#include "lvgl.h"

#include "device_catalog_browser.hpp"
#include "gui_router.hpp"
#include "../managers/device_browser_state.hpp"
#include "../managers/device_catalog.hpp"

class LibraryGui
{
private:
    DeviceCatalogBrowserModel catalogBrowser;
    DeviceBrowserState &browserState;
    GuiRouter &router;
    bool initialized = false;

    lv_obj_t *ui_Widget = nullptr;
    lv_obj_t *ui_DeviceList = nullptr;
    lv_obj_t *ui_Detail = nullptr;
    lv_obj_t *ui_DetailLabel = nullptr;
    lv_obj_t *ui_DeleteDialog = nullptr;

    void build();
    void populateDeviceList();
    void updateDetail();
    void handleDeleteButtonClick();
    void handleDeleteConfirmButtonClick();
    void closeDeleteDialog();

public:
    explicit LibraryGui(DeviceCatalog &deviceCatalog, DeviceBrowserState &browserState, GuiRouter &router);
    ~LibraryGui() = default;

    void init();
    bool isInitialized() const { return initialized; }
    void showLibrary();
    void hideLibrary();
    void setSelectedDevice(int index);
};

#endif // LIBRARY_GUI_HPP
