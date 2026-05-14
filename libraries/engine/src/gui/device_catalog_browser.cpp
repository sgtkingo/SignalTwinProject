/**
 * @file device_catalog_browser.cpp
 * @brief Shared catalog browser model and formatting helpers for catalog-based GUIs.
 */

#include "device_catalog_browser.hpp"

namespace
{
std::string buildAllowedPinsText(const BaseDevice *device)
{
    if (!device) {
        return "Any";
    }

    std::string allowedPinsText;
    const auto allowedPins = device->getAllowedPinsList();
    if (allowedPins.empty()) {
        return "Any";
    }

    for (size_t i = 0; i < allowedPins.size(); ++i) {
        if (i > 0) {
            allowedPinsText += ", ";
        }
        allowedPinsText += std::to_string(allowedPins[i]);
    }

    return allowedPinsText;
}

std::string buildPinsText(const BaseDevice *device)
{
    if (!device || device->getPinDefinitions().empty()) {
        return "None";
    }

    std::string pinsText;
    for (const auto &pin : device->getPinDefinitions()) {
        if (!pinsText.empty()) {
            pinsText += ", ";
        }
        pinsText += pin;
    }
    return pinsText;
}

std::string buildValuesText(const BaseDevice *device)
{
    if (!device) {
        return "- none\n";
    }

    std::string valuesText;
    const auto valueKeys = device->getValuesKeys();
    if (valueKeys.empty()) {
        return "- none\n";
    }

    for (const auto &key : valueKeys) {
        valuesText += "- " + key + " [" + device->getValueUnits(key) + "]\n";
    }

    return valuesText;
}

std::string buildConfigsText(const BaseDevice *device)
{
    if (!device) {
        return "- none\n";
    }

    std::string configsText;
    const auto configKeys = device->getConfigsKeys();
    if (configKeys.empty()) {
        return "- none\n";
    }

    for (const auto &key : configKeys) {
        configsText += "- " + key + "\n";
    }

    return configsText;
}
}

DeviceCatalogBrowserModel::DeviceCatalogBrowserModel(DeviceCatalog &deviceCatalog)
    : deviceCatalog(deviceCatalog)
{
}

const std::vector<BaseDevice *> &DeviceCatalogBrowserModel::getDevices() const
{
    return deviceCatalog.getDevices();
}

BaseDevice *DeviceCatalogBrowserModel::getSelectedDevice() const
{
    const auto &devices = getDevices();
    if (devices.empty() || selectedDeviceIndex < 0 || selectedDeviceIndex >= static_cast<int>(devices.size())) {
        return nullptr;
    }

    return devices[selectedDeviceIndex];
}

void DeviceCatalogBrowserModel::setSelectedDeviceIndex(int index)
{
    selectedDeviceIndex = index;
}

std::string DeviceCatalogBrowserFormatter::buildDeviceListLabel(const BaseDevice *device)
{
    if (!device) {
        return "Unknown";
    }

    return device->getName() + " [" + device->getRoleLabel() + "]";
}

std::string DeviceCatalogBrowserFormatter::buildSelectionInfoText(const BaseDevice *device)
{
    if (!device) {
        return "No device information available.";
    }

    std::string info = device->getDescription();
    if (info.empty()) {
        info = "No description available.";
    }

    info += "\nRole: " + device->getRoleLabel();
    info += "\nPicture: " + device->getPicture();
    info += "\nPins: " + buildPinsText(device);
    info += "\n\nAllowed Pins: " + buildAllowedPinsText(device);
    return info;
}

std::string DeviceCatalogBrowserFormatter::buildSelectionSpecsText(const BaseDevice *device)
{
    if (!device) {
        return "";
    }

    std::string specs = "Values:\n";
    specs += buildValuesText(device);
    specs += "\nConfigs:\n";
    specs += buildConfigsText(device);
    return specs;
}

std::string DeviceCatalogBrowserFormatter::buildLibraryDetailText(const BaseDevice *device)
{
    if (!device) {
        return "No entity selected.";
    }

    std::string detail = "Entity\n";
    detail += device->getName() + "\n\n";
    detail += "Role:\n" + device->getRoleLabel() + "\n\n";
    detail += "Picture:\n" + device->getPicture() + "\n\n";
    detail += "Description:\n" + device->getDescription() + "\n\n";
    detail += "Pins:\n" + buildPinsText(device) + "\n\n";
    detail += "Allowed Pins:\n" + buildAllowedPinsText(device) + "\n\n";
    detail += "Values:\n";
    detail += buildValuesText(device);
    detail += "\nConfigs:\n";
    detail += buildConfigsText(device);
    return detail;
}

DeviceCatalogBrowserLayout DeviceCatalogBrowserLayoutFactory::createLayout(const DeviceCatalogBrowserLayoutConfig &config)
{
    DeviceCatalogBrowserLayout layout;

    layout.root = lv_obj_create(lv_scr_act());
    lv_obj_remove_style_all(layout.root);
    lv_obj_set_size(layout.root, 760, 440);
    lv_obj_set_align(layout.root, LV_ALIGN_CENTER);
    lv_obj_set_style_radius(layout.root, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(layout.root, lv_color_hex(config.backgroundColor), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(layout.root, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(layout.root, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(layout.root, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_flag(layout.root, LV_OBJ_FLAG_HIDDEN);

    layout.titleLabel = lv_label_create(layout.root);
    lv_label_set_text(layout.titleLabel, config.title ? config.title : "");
    lv_obj_align(layout.titleLabel, LV_ALIGN_TOP_MID, 0, 12);
    lv_obj_set_style_text_font(layout.titleLabel, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);

    layout.primaryList = lv_list_create(layout.root);
    lv_obj_set_size(layout.primaryList, 250, 285);
    lv_obj_set_pos(layout.primaryList, 10, 50);

    layout.detailPanel = lv_obj_create(layout.root);
    if (config.includeSecondaryList) {
        lv_obj_set_size(layout.detailPanel, 300, 285);
        lv_obj_set_pos(layout.detailPanel, 230, 50);
    } else {
        lv_obj_set_size(layout.detailPanel, 410, 320);
        lv_obj_set_pos(layout.detailPanel, 270, 50);
    }

    layout.detailLabel = lv_label_create(layout.detailPanel);
    lv_obj_set_width(layout.detailLabel, config.includeSecondaryList ? 270 : lv_pct(100));
    lv_label_set_long_mode(layout.detailLabel, LV_LABEL_LONG_WRAP);
    lv_obj_align(layout.detailLabel, LV_ALIGN_TOP_LEFT, 10, 10);

    if (config.includeSecondaryList) {
        layout.secondaryList = lv_list_create(layout.root);
        lv_obj_set_size(layout.secondaryList, 210, 285);
        lv_obj_set_pos(layout.secondaryList, 540, 50);
    }

    return layout;
}

lv_obj_t *DeviceCatalogBrowserLayoutFactory::createFooterButton(lv_obj_t *parent,
                                                                const char *text,
                                                                lv_align_t align,
                                                                lv_coord_t xOffset,
                                                                lv_coord_t yOffset)
{
    if (!parent) {
        return nullptr;
    }

    lv_obj_t *button = lv_btn_create(parent);
    lv_obj_set_size(button, 120, 36);
    lv_obj_align(button, align, xOffset, yOffset);

    lv_obj_t *label = lv_label_create(button);
    lv_label_set_text(label, text ? text : "");
    lv_obj_center(label);
    return button;
}
