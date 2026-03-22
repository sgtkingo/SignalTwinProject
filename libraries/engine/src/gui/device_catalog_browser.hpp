/**
 * @file device_catalog_browser.hpp
 * @brief Shared catalog browser model and formatting helpers for catalog-based GUIs.
 */

#ifndef DEVICE_CATALOG_BROWSER_HPP
#define DEVICE_CATALOG_BROWSER_HPP

#include <string>
#include <vector>

#include "lvgl.h"

#include "../devices/base_device.hpp"
#include "../managers/device_catalog.hpp"

class DeviceCatalogBrowserModel
{
private:
    DeviceCatalog &deviceCatalog;
    int selectedDeviceIndex = 0;

public:
    explicit DeviceCatalogBrowserModel(DeviceCatalog &deviceCatalog);

    const std::vector<BaseDevice *> &getDevices() const;
    BaseDevice *getSelectedDevice() const;
    void setSelectedDeviceIndex(int index);
};

namespace DeviceCatalogBrowserFormatter
{
std::string buildDeviceListLabel(const BaseDevice *device);
std::string buildSelectionInfoText(const BaseDevice *device);
std::string buildSelectionSpecsText(const BaseDevice *device);
std::string buildLibraryDetailText(const BaseDevice *device);
}

struct DeviceCatalogBrowserLayoutConfig
{
    const char *title = "";
    uint32_t backgroundColor = 0xFFFFFF;
    bool includeSecondaryList = false;
};

struct DeviceCatalogBrowserLayout
{
    lv_obj_t *root = nullptr;
    lv_obj_t *titleLabel = nullptr;
    lv_obj_t *primaryList = nullptr;
    lv_obj_t *detailPanel = nullptr;
    lv_obj_t *detailLabel = nullptr;
    lv_obj_t *secondaryList = nullptr;
};

namespace DeviceCatalogBrowserLayoutFactory
{
DeviceCatalogBrowserLayout createLayout(const DeviceCatalogBrowserLayoutConfig &config);
lv_obj_t *createFooterButton(lv_obj_t *parent,
                             const char *text,
                             lv_align_t align,
                             lv_coord_t xOffset,
                             lv_coord_t yOffset);
}

#endif // DEVICE_CATALOG_BROWSER_HPP
