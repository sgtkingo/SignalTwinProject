/**
 * @file device_catalog_browser.cpp
 * @brief Shared catalog browser model and formatting helpers for catalog-based GUIs.
 */

#include "device_catalog_browser.hpp"

#include "./images/ui_images.h"
#include "lvgl_storage_fs.hpp"
#include "../config.hpp"
#include "../managers/storage_manager.hpp"

namespace
{
constexpr uint32_t COLOR_TEXT = 0x1C1F23;
constexpr uint32_t COLOR_MUTED = 0x6C7680;
constexpr uint32_t COLOR_PANEL = 0xFFFFFF;
constexpr uint32_t COLOR_BORDER = 0xD8DEE8;
constexpr uint32_t COLOR_VALUE = 0x2F80ED;
constexpr uint32_t COLOR_CONFIG = 0xD96464;
constexpr uint32_t COLOR_PIN = 0xE9F2FF;
constexpr uint32_t COLOR_ALLOWED_PIN = 0xEEF7ED;

std::string pictureSourcePath;

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

std::string ellipsize(const std::string &text, size_t maxLength)
{
    if (text.size() <= maxLength) {
        return text;
    }
    if (maxLength <= 3) {
        return text.substr(0, maxLength);
    }
    return text.substr(0, maxLength - 3) + "...";
}

uint32_t roleColor(DeviceRole role)
{
    switch (role) {
    case DeviceRole::ACTUATOR:
        return 0xF2A23A;
    case DeviceRole::HYBRID:
        return 0x7B61FF;
    case DeviceRole::SENSOR:
    default:
        return 0x2EAD5F;
    }
}

const lv_img_dsc_t *placeholderImageForRole(DeviceRole role)
{
    switch (role) {
    case DeviceRole::ACTUATOR:
        return &ui_img_placeholder_actuator;
    case DeviceRole::HYBRID:
        return &ui_img_placeholder_hybrid;
    case DeviceRole::SENSOR:
    default:
        return &ui_img_placeholder_sensor;
    }
}

lv_obj_t *createLabel(lv_obj_t *parent,
                      const std::string &text,
                      lv_coord_t x,
                      lv_coord_t y,
                      lv_coord_t width,
                      const lv_font_t *font,
                      uint32_t color = COLOR_TEXT)
{
    lv_obj_t *label = lv_label_create(parent);
    lv_label_set_text(label, text.c_str());
    lv_obj_set_width(label, width);
    lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(label, x, y);
    lv_obj_set_style_text_font(label, font, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(label, lv_color_hex(color), LV_PART_MAIN | LV_STATE_DEFAULT);
    return label;
}

lv_obj_t *createBadge(lv_obj_t *parent,
                      const std::string &text,
                      lv_coord_t x,
                      lv_coord_t y,
                      lv_coord_t width,
                      uint32_t color,
                      uint32_t textColor = 0xFFFFFF)
{
    lv_obj_t *badge = lv_obj_create(parent);
    lv_obj_remove_style_all(badge);
    lv_obj_set_size(badge, width, 24);
    lv_obj_set_pos(badge, x, y);
    lv_obj_set_style_radius(badge, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(badge, lv_color_hex(color), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(badge, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_clear_flag(badge, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t *label = lv_label_create(badge);
    lv_label_set_text(label, text.c_str());
    lv_obj_center(label);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(label, lv_color_hex(textColor), LV_PART_MAIN | LV_STATE_DEFAULT);
    return badge;
}

lv_coord_t createChipRow(lv_obj_t *parent,
                         const std::vector<std::string> &items,
                         lv_coord_t x,
                         lv_coord_t y,
                         lv_coord_t maxWidth,
                         uint32_t color,
                         uint32_t textColor = COLOR_TEXT)
{
    if (items.empty()) {
        createBadge(parent, "None", x, y, 64, 0xEEF0F3, COLOR_MUTED);
        return y + 30;
    }

    lv_coord_t cursorX = x;
    lv_coord_t cursorY = y;
    for (const auto &item : items) {
        const lv_coord_t width = static_cast<lv_coord_t>(item.size() * 8 + 24);
        const lv_coord_t chipWidth = width < 48 ? 48 : (width > 132 ? 132 : width);
        if (cursorX + chipWidth > x + maxWidth) {
            cursorX = x;
            cursorY += 30;
        }

        createBadge(parent, item, cursorX, cursorY, chipWidth, color, textColor);
        cursorX += chipWidth + 6;
    }
    return cursorY + 30;
}

std::vector<std::string> buildAllowedPinItems(const BaseDevice *device)
{
    std::vector<std::string> items;
    if (!device) {
        return items;
    }

    const auto allowedPins = device->getAllowedPinsList();
    for (int pin : allowedPins) {
        items.push_back(std::to_string(pin));
    }
    if (items.empty()) {
        items.push_back("Any");
    }
    return items;
}

std::vector<std::string> buildValueItems(const BaseDevice *device)
{
    std::vector<std::string> items;
    if (!device) {
        return items;
    }

    const auto values = device->getValues();
    for (const auto &key : device->getValuesKeys()) {
        auto it = values.find(key);
        std::string item = key;
        if (it != values.end() && !it->second.Unit.empty()) {
            item += " " + it->second.Unit;
        }
        if (it != values.end() && it->second.Access == DeviceParamAccess::WRITE) {
            item += " OUT";
        }
        items.push_back(item);
    }
    return items;
}

std::vector<std::string> buildConfigItems(const BaseDevice *device)
{
    std::vector<std::string> items;
    if (!device) {
        return items;
    }

    const auto configs = device->getConfigs();
    for (const auto &key : device->getConfigsKeys()) {
        auto it = configs.find(key);
        std::string item = key;
        if (it != configs.end() && !it->second.Unit.empty()) {
            item += " " + it->second.Unit;
        }
        items.push_back(item);
    }
    return items;
}

bool isStoragePicturePath(const std::string &path)
{
    return path.rfind(STORAGE_DEVICE_PICTURE_DIR, 0) == 0;
}

std::string findDevicePicturePath(const BaseDevice *device)
{
    if (!device || (!storageManager().isAvailable() && !storageManager().init())) {
        return "";
    }

    const std::string storedPicture = device->getPicture();
    if (!storedPicture.empty() && isStoragePicturePath(storedPicture) && storageManager().exists(storedPicture)) {
        return storedPicture;
    }

    const char *extensions[] = {".png", ".jpg", ".gif"};
    for (const char *extension : extensions) {
        const std::string candidate = std::string(STORAGE_DEVICE_PICTURE_DIR) + "/" + device->UID + extension;
        if (storageManager().exists(candidate)) {
            return candidate;
        }
    }

    return "";
}

void renderPicture(lv_obj_t *parent, const BaseDevice *device)
{
    lv_obj_t *picture = lv_obj_create(parent);
    lv_obj_set_size(picture, 112, 112);
    lv_obj_set_pos(picture, 280, 52);
    lv_obj_clear_flag(picture, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_radius(picture, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(picture, lv_color_hex(0xF5F7FA), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(picture, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(picture, lv_color_hex(COLOR_BORDER), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(picture, 1, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t *image = lv_img_create(picture);
    const std::string imagePath = findDevicePicturePath(device);
    if (imagePath.empty()) {
        lv_img_set_src(image, placeholderImageForRole(device ? device->getRole() : DeviceRole::SENSOR));
        lv_img_set_zoom(image, 256);
        lv_obj_center(image);
        return;
    }

    ensureLvglStorageFsRegistered();
    pictureSourcePath = "S:" + imagePath;
    lv_img_set_src(image, pictureSourcePath.c_str());
    lv_img_set_zoom(image, 180);
    lv_obj_center(image);
}

void createSectionTitle(lv_obj_t *parent, const std::string &title, lv_coord_t x, lv_coord_t y, uint32_t color)
{
    createLabel(parent, title, x, y, 360, &lv_font_montserrat_14, color);
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

void DeviceCatalogBrowserRenderer::styleDeviceListButton(lv_obj_t *button)
{
    if (!button) {
        return;
    }

    lv_obj_set_height(button, 58);
    lv_obj_set_style_pad_top(button, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(button, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(button, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t *label = lv_obj_get_child(button, 0);
    if (label) {
        lv_obj_set_style_text_font(label, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
    }
}

void DeviceCatalogBrowserRenderer::renderDeviceDetail(lv_obj_t *panel, const BaseDevice *device, const std::string &footerText)
{
    if (!panel) {
        return;
    }

    lv_obj_clean(panel);
    lv_obj_set_style_bg_color(panel, lv_color_hex(COLOR_PANEL), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(panel, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(panel, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(panel, lv_color_hex(COLOR_BORDER), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(panel, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(panel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_scrollbar_mode(panel, LV_SCROLLBAR_MODE_AUTO);

    if (!device) {
        createLabel(panel, "No device selected", 14, 14, 360, &lv_font_montserrat_20);
        createLabel(panel, "Choose a device from the list.", 14, 48, 360, &lv_font_montserrat_14, COLOR_MUTED);
        return;
    }

    createLabel(panel, ellipsize(device->getName(), 30), 14, 14, 370, &lv_font_montserrat_20);
    createLabel(panel, device->UID, 14, 42, 252, &lv_font_montserrat_12, COLOR_MUTED);
    renderPicture(panel, device);
    createBadge(panel, device->getRoleLabel(), 14, 66, 104, roleColor(device->getRole()));

    const std::string description = device->getDescription().empty() ? "No description available." : device->getDescription();
    lv_obj_t *descriptionLabel = createLabel(panel, ellipsize(description, 75), 14, 96, 252, &lv_font_montserrat_14, COLOR_MUTED);
    lv_obj_set_height(descriptionLabel, 40);
    lv_label_set_long_mode(descriptionLabel, LV_LABEL_LONG_DOT);

    lv_coord_t y = 176;
    if (!footerText.empty()) {
        createBadge(panel, footerText, 14, y, 116, 0xFFF8D6, COLOR_TEXT);
        y += 34;
    }

    createSectionTitle(panel, "Pins", 14, y, COLOR_TEXT);
    y = createChipRow(panel, device->getPinDefinitions(), 14, y + 22, 370, COLOR_PIN, COLOR_TEXT);

    createSectionTitle(panel, "Allowed Pins", 14, y + 4, COLOR_TEXT);
    y = createChipRow(panel, buildAllowedPinItems(device), 14, y + 26, 370, COLOR_ALLOWED_PIN, COLOR_TEXT);

    createSectionTitle(panel, "Values", 14, y + 4, COLOR_VALUE);
    y = createChipRow(panel, buildValueItems(device), 14, y + 26, 370, COLOR_VALUE, 0xFFFFFF);

    createSectionTitle(panel, "Configs", 14, y + 4, COLOR_CONFIG);
    y = createChipRow(panel, buildConfigItems(device), 14, y + 26, 370, COLOR_CONFIG, 0xFFFFFF);

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
