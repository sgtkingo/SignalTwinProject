/**
 * @file device_browser_state.hpp
 * @brief Shared browse/highlight state for catalog-driven GUI screens.
 */

#ifndef DEVICE_BROWSER_STATE_HPP
#define DEVICE_BROWSER_STATE_HPP

#include "../devices/json_device_builder.hpp"
#include "../devices/base_device.hpp"
#include "device_catalog.hpp"

class DeviceBrowserState
{
private:
    DeviceCatalog &catalog;
    BaseDevice *selectionDevice = nullptr;
    BaseDevice *libraryDevice = nullptr;
    DeviceDefinitionSchema libraryDraft;
    bool hasLibraryDraft = false;
    bool libraryDraftNewEntity = false;

    bool isCatalogDevice(const BaseDevice *device) const;

public:
    explicit DeviceBrowserState(DeviceCatalog &catalog);

    void clear();

    BaseDevice *getSelectionDevice() const;
    void setSelectionDevice(BaseDevice *device);

    BaseDevice *getLibraryDevice() const;
    void setLibraryDevice(BaseDevice *device);

    void clearLibraryDraft();
    void beginNewLibraryDraft();
    bool beginLibraryDraftFromLibraryDevice();
    bool hasActiveLibraryDraft() const { return hasLibraryDraft; }
    bool isLibraryDraftNewEntity() const { return libraryDraftNewEntity; }
    const DeviceDefinitionSchema *getLibraryDraft() const;
    DeviceDefinitionSchema *editLibraryDraft();
    void setLibraryDraft(const DeviceDefinitionSchema &draft, bool isNewEntity);
};

#endif // DEVICE_BROWSER_STATE_HPP
