/**
 * @file device_browser_state.hpp
 * @brief Shared browse/highlight state for catalog-driven GUI screens.
 */

#ifndef DEVICE_BROWSER_STATE_HPP
#define DEVICE_BROWSER_STATE_HPP

#include "../devices/base_device.hpp"
#include "device_catalog.hpp"

class DeviceBrowserState
{
private:
    DeviceCatalog &catalog;
    BaseDevice *selectionDevice = nullptr;
    BaseDevice *libraryDevice = nullptr;

    bool isCatalogDevice(const BaseDevice *device) const;

public:
    explicit DeviceBrowserState(DeviceCatalog &catalog);

    void clear();

    BaseDevice *getSelectionDevice() const;
    void setSelectionDevice(BaseDevice *device);

    BaseDevice *getLibraryDevice() const;
    void setLibraryDevice(BaseDevice *device);
};

#endif // DEVICE_BROWSER_STATE_HPP
