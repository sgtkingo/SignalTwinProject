/**
 * @file device_browser_state.cpp
 * @brief Shared browse/highlight state for catalog-driven GUI screens.
 */

#include "device_browser_state.hpp"

#include <algorithm>

DeviceBrowserState::DeviceBrowserState(DeviceCatalog &catalog) : catalog(catalog)
{
}

bool DeviceBrowserState::isCatalogDevice(const BaseDevice *device) const
{
    if (!device) {
        return false;
    }

    const auto &devices = catalog.getDevices();
    return std::find(devices.begin(), devices.end(), device) != devices.end();
}

void DeviceBrowserState::clear()
{
    selectionDevice = nullptr;
    libraryDevice = nullptr;
    clearLibraryDraft();
}

BaseDevice *DeviceBrowserState::getSelectionDevice() const
{
    return isCatalogDevice(selectionDevice) ? selectionDevice : nullptr;
}

void DeviceBrowserState::setSelectionDevice(BaseDevice *device)
{
    selectionDevice = isCatalogDevice(device) ? device : nullptr;
}

BaseDevice *DeviceBrowserState::getLibraryDevice() const
{
    return isCatalogDevice(libraryDevice) ? libraryDevice : nullptr;
}

void DeviceBrowserState::setLibraryDevice(BaseDevice *device)
{
    BaseDevice *nextDevice = isCatalogDevice(device) ? device : nullptr;
    if (libraryDevice != nextDevice) {
        if (!hasLibraryDraft || !nextDevice || libraryDraft.uid != nextDevice->UID) {
            clearLibraryDraft();
        }
    }
    libraryDevice = nextDevice;
}

void DeviceBrowserState::clearLibraryDraft()
{
    libraryDraft = DeviceDefinitionSchema();
    hasLibraryDraft = false;
    libraryDraftNewEntity = false;
}

void DeviceBrowserState::beginNewLibraryDraft()
{
    libraryDevice = nullptr;
    libraryDraft = DeviceDefinitionSchema();
    libraryDraft.role = DeviceRole::SENSOR;
    hasLibraryDraft = true;
    libraryDraftNewEntity = true;
}

bool DeviceBrowserState::beginLibraryDraftFromLibraryDevice()
{
    BaseDevice *device = getLibraryDevice();
    if (!device) {
        return false;
    }

    const DeviceDefinitionSchema *deviceSchema = catalog.getDeviceSchema(device->UID);
    if (!deviceSchema) {
        return false;
    }

    libraryDraft = *deviceSchema;
    hasLibraryDraft = true;
    libraryDraftNewEntity = false;
    return true;
}

const DeviceDefinitionSchema *DeviceBrowserState::getLibraryDraft() const
{
    return hasLibraryDraft ? &libraryDraft : nullptr;
}

DeviceDefinitionSchema *DeviceBrowserState::editLibraryDraft()
{
    return hasLibraryDraft ? &libraryDraft : nullptr;
}

void DeviceBrowserState::setLibraryDraft(const DeviceDefinitionSchema &draft, bool isNewEntity)
{
    libraryDraft = draft;
    hasLibraryDraft = true;
    libraryDraftNewEntity = isNewEntity;
}
