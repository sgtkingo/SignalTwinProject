/**
 * @file device_browser_state.hpp
 * @brief Shared browse/focus state for catalog-driven GUI screens.
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
    BaseDevice *selectionDevice = nullptr;    ///< Focused catalog device in the Selection screen
    BaseDevice *libraryDevice = nullptr;      ///< Focused catalog device in the Library screen
    DeviceDefinitionSchema libraryDraft;      ///< Active Library editor draft
    bool hasLibraryDraft = false;             ///< Whether a draft is currently active
    bool libraryDraftNewEntity = false;       ///< Whether the draft represents a new entity

    bool isCatalogDevice(const BaseDevice *device) const;

public:
    explicit DeviceBrowserState(DeviceCatalog &catalog);

    /**
     * @brief Reset all browse pointers and editor draft state.
     */
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
