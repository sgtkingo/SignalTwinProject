#include "settings_gui.hpp"

#include <string>

SettingsGui::SettingsGui(GuiRouter &router) : router(router)
{
}

lv_obj_t *SettingsGui::createSingleLineInput(lv_obj_t *parent, lv_coord_t x, lv_coord_t y, lv_coord_t width)
{
    lv_obj_t *input = lv_textarea_create(parent);
    lv_obj_set_size(input, width, 34);
    lv_obj_set_pos(input, x, y);
    lv_textarea_set_one_line(input, true);
    return input;
}

void SettingsGui::addModeButton(const char *text, DefaultCommunicationMode mode, lv_coord_t y)
{
    lv_obj_t *button = lv_btn_create(ui_Widget);
    lv_obj_set_size(button, 230, 38);
    lv_obj_set_pos(button, 26, y);
    lv_obj_add_event_cb(button, [](lv_event_t *e) {
        if (lv_event_get_code(e) != LV_EVENT_CLICKED) {
            return;
        }

        auto *self = static_cast<SettingsGui *>(lv_event_get_user_data(e));
        auto mode = static_cast<DefaultCommunicationMode>(reinterpret_cast<intptr_t>(lv_obj_get_user_data(lv_event_get_target(e))));
        self->router.setDefaultCommunicationMode(mode);
        self->showSettings();
    }, LV_EVENT_ALL, this);
    lv_obj_set_user_data(button, reinterpret_cast<void *>(static_cast<intptr_t>(mode)));

    lv_obj_t *label = lv_label_create(button);
    lv_label_set_text(label, text);
    lv_obj_center(label);
}

void SettingsGui::build()
{
    ui_Widget = lv_obj_create(lv_scr_act());
    lv_obj_remove_style_all(ui_Widget);
    lv_obj_set_size(ui_Widget, 760, 440);
    lv_obj_set_align(ui_Widget, LV_ALIGN_CENTER);
    lv_obj_set_style_radius(ui_Widget, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Widget, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Widget, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_Widget, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_Widget, 2, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t *title = lv_label_create(ui_Widget);
    lv_label_set_text(title, "Settings");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 16);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_CurrentMode = lv_label_create(ui_Widget);
    lv_obj_set_pos(ui_CurrentMode, 26, 62);
    lv_obj_set_style_text_font(ui_CurrentMode, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);

    addModeButton("Ask every time", DefaultCommunicationMode::ASK, 104);
    addModeButton("Cable (UART)", DefaultCommunicationMode::CABLE, 148);
    addModeButton("Wireless Auto", DefaultCommunicationMode::WIRELESS_AUTO, 192);
    addModeButton("Wireless Manual", DefaultCommunicationMode::WIRELESS_MANUAL, 236);

    lv_obj_t *metadataPanel = lv_obj_create(ui_Widget);
    lv_obj_set_size(metadataPanel, 420, 238);
    lv_obj_set_pos(metadataPanel, 314, 82);

    lv_obj_t *metadataTitle = lv_label_create(metadataPanel);
    lv_label_set_text(metadataTitle, "Catalog Metadata");
    lv_obj_set_pos(metadataTitle, 12, 10);
    lv_obj_set_style_text_font(metadataTitle, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t *appLabel = lv_label_create(metadataPanel);
    lv_label_set_text(appLabel, "Application");
    lv_obj_set_pos(appLabel, 12, 50);
    ui_ApplicationInput = createSingleLineInput(metadataPanel, 120, 42, 270);

    lv_obj_t *versionLabel = lv_label_create(metadataPanel);
    lv_label_set_text(versionLabel, "Version");
    lv_obj_set_pos(versionLabel, 12, 96);
    ui_VersionInput = createSingleLineInput(metadataPanel, 120, 88, 270);

    lv_obj_t *saveMetadata = lv_btn_create(metadataPanel);
    lv_obj_set_size(saveMetadata, 132, 36);
    lv_obj_set_pos(saveMetadata, 258, 138);
    lv_obj_add_event_cb(saveMetadata, [](lv_event_t *e) {
        if (lv_event_get_code(e) != LV_EVENT_CLICKED) {
            return;
        }

        auto *self = static_cast<SettingsGui *>(lv_event_get_user_data(e));
        std::string error;
        if (!self->ui_ApplicationInput || !self->ui_VersionInput || !self->ui_MetadataStatus) {
            return;
        }

        const std::string application = lv_textarea_get_text(self->ui_ApplicationInput);
        const std::string version = lv_textarea_get_text(self->ui_VersionInput);
        if (application.empty() || version.empty()) {
            lv_label_set_text(self->ui_MetadataStatus, "Application and Version are required.");
            return;
        }

        if (!self->router.saveCatalogMetadata(application, version, error)) {
            lv_label_set_text(self->ui_MetadataStatus, error.c_str());
            return;
        }

        lv_label_set_text(self->ui_MetadataStatus, "Catalog metadata saved.");
        self->refresh();
    }, LV_EVENT_ALL, this);
    lv_obj_t *saveMetadataLabel = lv_label_create(saveMetadata);
    lv_label_set_text(saveMetadataLabel, "Save Metadata");
    lv_obj_center(saveMetadataLabel);

    ui_MetadataStatus = lv_label_create(metadataPanel);
    lv_obj_set_width(ui_MetadataStatus, 390);
    lv_obj_set_pos(ui_MetadataStatus, 12, 188);
    lv_label_set_long_mode(ui_MetadataStatus, LV_LABEL_LONG_WRAP);
    lv_label_set_text(ui_MetadataStatus, "");

    lv_obj_t *back = lv_btn_create(ui_Widget);
    lv_obj_set_size(back, 90, 36);
    lv_obj_align(back, LV_ALIGN_BOTTOM_LEFT, 16, -14);
    lv_obj_add_event_cb(back, [](lv_event_t *e) {
        if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
            auto *self = static_cast<SettingsGui *>(lv_event_get_user_data(e));
            self->router.showMainMenu();
        }
    }, LV_EVENT_ALL, this);
    lv_obj_t *backLabel = lv_label_create(back);
    lv_label_set_text(backLabel, "Back");
    lv_obj_center(backLabel);
}

void SettingsGui::refresh()
{
    if (!ui_CurrentMode) {
        return;
    }

    const char *modeText = "Ask every time";
    switch (router.getDefaultCommunicationMode()) {
    case DefaultCommunicationMode::CABLE:
        modeText = "Cable (UART)";
        break;
    case DefaultCommunicationMode::WIRELESS_AUTO:
        modeText = "Wireless Auto";
        break;
    case DefaultCommunicationMode::WIRELESS_MANUAL:
        modeText = "Wireless Manual";
        break;
    default:
        break;
    }

    std::string label = "Default communication: ";
    label += modeText;
    lv_label_set_text(ui_CurrentMode, label.c_str());

    if (ui_ApplicationInput) {
        lv_textarea_set_text(ui_ApplicationInput, router.getCatalogApplication().c_str());
    }
    if (ui_VersionInput) {
        lv_textarea_set_text(ui_VersionInput, router.getCatalogVersion().c_str());
    }
}

void SettingsGui::init()
{
    if (initialized) {
        return;
    }

    build();
    initialized = true;
}

void SettingsGui::showSettings()
{
    if (!initialized || !ui_Widget) {
        return;
    }

    refresh();
    lv_obj_clear_flag(ui_Widget, LV_OBJ_FLAG_HIDDEN);
}

void SettingsGui::hideSettings()
{
    if (!initialized || !ui_Widget) {
        return;
    }

    lv_obj_add_flag(ui_Widget, LV_OBJ_FLAG_HIDDEN);
}
