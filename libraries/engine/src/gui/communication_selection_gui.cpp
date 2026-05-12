#include "communication_selection_gui.hpp"

#include "../helpers.hpp"
#include "expt.hpp"

#ifndef LV_SYMBOL_SETTINGS
#define LV_SYMBOL_SETTINGS "⚙"
#endif

namespace
{
constexpr uint32_t MIN_CONNECT_LOADING_MS = 500;
}

CommunicationSelectionGui::CommunicationSelectionGui(GuiRouter &router, DeviceManager &deviceManager)
    : router(router), deviceManager(deviceManager)
{
}

void CommunicationSelectionGui::createOptionButton(const char *text, lv_coord_t x, lv_coord_t y, DefaultCommunicationMode mode, bool supported)
{
    lv_obj_t *button = lv_btn_create(ui_Widget);
    lv_obj_set_size(button, 220, 80);
    lv_obj_set_pos(button, x, y);
    if (!supported) {
        lv_obj_set_style_bg_color(button, lv_color_hex(0x8A8F98), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(button, lv_color_hex(0x8A8F98), LV_PART_MAIN | LV_STATE_PRESSED);
        lv_obj_set_style_bg_opa(button, 180, LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    lv_obj_add_event_cb(button, [](lv_event_t *e) {
        if (lv_event_get_code(e) != LV_EVENT_CLICKED) {
            return;
        }

        auto *self = static_cast<CommunicationSelectionGui *>(lv_event_get_user_data(e));
        DefaultCommunicationMode mode = static_cast<DefaultCommunicationMode>(reinterpret_cast<intptr_t>(lv_obj_get_user_data(lv_event_get_current_target(e))));
        self->handleModeSelection(mode);
    }, LV_EVENT_ALL, this);
    lv_obj_set_user_data(button, reinterpret_cast<void *>(static_cast<intptr_t>(mode)));

    lv_obj_t *label = lv_label_create(button);
    lv_label_set_text(label, text);
    lv_obj_center(label);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
}

void CommunicationSelectionGui::createWirelessManualButton(lv_coord_t x, lv_coord_t y)
{
    lv_obj_t *button = lv_btn_create(ui_Widget);
    lv_obj_set_size(button, 58, 80);
    lv_obj_set_pos(button, x, y);
    lv_obj_set_style_bg_color(button, lv_color_hex(0x8A8F98), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(button, lv_color_hex(0x8A8F98), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(button, 180, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_user_data(button, reinterpret_cast<void *>(static_cast<intptr_t>(DefaultCommunicationMode::WIRELESS_MANUAL)));
    lv_obj_add_event_cb(button, [](lv_event_t *e) {
        if (lv_event_get_code(e) != LV_EVENT_CLICKED) {
            return;
        }

        auto *self = static_cast<CommunicationSelectionGui *>(lv_event_get_user_data(e));
        // TODO: Navigate to Wireless Settings when the screen is implemented.
        // TODO: Include a Wireless Explore flow for scanning available endpoints.
        self->handleModeSelection(DefaultCommunicationMode::WIRELESS_MANUAL);
    }, LV_EVENT_ALL, this);

    lv_obj_t *label = lv_label_create(button);
    lv_label_set_text(label, LV_SYMBOL_SETTINGS);
    lv_obj_center(label);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
}

uint32_t CommunicationSelectionGui::showLoading(const char *message)
{
    if (!ui_LoadingOverlay) {
        ui_LoadingOverlay = lv_obj_create(ui_Widget);
        lv_obj_remove_style_all(ui_LoadingOverlay);
        lv_obj_set_size(ui_LoadingOverlay, lv_pct(100), lv_pct(100));
        lv_obj_set_align(ui_LoadingOverlay, LV_ALIGN_CENTER);
        lv_obj_set_style_bg_color(ui_LoadingOverlay, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(ui_LoadingOverlay, 150, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_add_flag(ui_LoadingOverlay, LV_OBJ_FLAG_CLICKABLE);

        lv_obj_t *panel = lv_obj_create(ui_LoadingOverlay);
        lv_obj_set_size(panel, 240, 120);
        lv_obj_center(panel);
        lv_obj_set_style_radius(panel, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(panel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(panel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

        lv_obj_t *spinner = lv_spinner_create(panel, 900, 60);
        lv_obj_set_size(spinner, 38, 38);
        lv_obj_align(spinner, LV_ALIGN_TOP_MID, 0, 16);

        ui_LoadingLabel = lv_label_create(panel);
        lv_label_set_text(ui_LoadingLabel, message ? message : "Connecting...");
        lv_obj_align(ui_LoadingLabel, LV_ALIGN_BOTTOM_MID, 0, -18);
        lv_obj_set_style_text_align(ui_LoadingLabel, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    } else {
        lv_label_set_text(ui_LoadingLabel, message ? message : "Connecting...");
    }

    connectionBusy = true;
    lv_obj_clear_flag(ui_LoadingOverlay, LV_OBJ_FLAG_HIDDEN);
    lv_obj_move_foreground(ui_LoadingOverlay);
    lv_obj_invalidate(ui_LoadingOverlay);
    const uint32_t startTick = lv_tick_get();
    lv_timer_handler();
    lv_refr_now(nullptr);
    delay_ms(20);
    lv_timer_handler();
    lv_refr_now(nullptr);
    return startTick;
}

void CommunicationSelectionGui::finishLoading(uint32_t startTick, bool keepVisible)
{
    while (lv_tick_elaps(startTick) < MIN_CONNECT_LOADING_MS) {
        lv_timer_handler();
        delay_ms(16);
    }

    if (!keepVisible) {
        hideLoading();
    }
}

void CommunicationSelectionGui::hideLoading()
{
    connectionBusy = false;
    if (ui_LoadingOverlay) {
        lv_obj_add_flag(ui_LoadingOverlay, LV_OBJ_FLAG_HIDDEN);
    }
}

void CommunicationSelectionGui::handleModeSelection(DefaultCommunicationMode mode)
{
    if (connectionBusy) {
        return;
    }

    if (mode != DefaultCommunicationMode::CABLE) {
        splashMessage("Wireless connection is not supported yet.");
        return;
    }

    const uint32_t loadingStart = showLoading("Connecting...");
    if (!deviceManager.initializeProtocolConnection()) {
        finishLoading(loadingStart, false);
        splashMessage("VSCP init failed. Check cable connection and emulator.");
        return;
    }

    finishLoading(loadingStart, true);
    router.completeCommunicationSelection(mode);
}

void CommunicationSelectionGui::init(void)
{
    if (initialized) {
        return;
    }

    constructCommunicationSelection();
    initialized = true;
}

void CommunicationSelectionGui::constructCommunicationSelection(void)
{
    ui_Widget = lv_obj_create(lv_scr_act());
    lv_obj_remove_style_all(ui_Widget);
    lv_obj_set_width(ui_Widget, 760);
    lv_obj_set_height(ui_Widget, 440);
    lv_obj_set_align(ui_Widget, LV_ALIGN_CENTER);
    lv_obj_set_style_radius(ui_Widget, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Widget, lv_color_hex(0x055DA9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Widget, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_Widget, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_Widget, 2, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t *title = lv_label_create(ui_Widget);
    lv_label_set_text(title, "Communication");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 30);
    lv_obj_set_style_text_color(title, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_40, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t *subtitle = lv_label_create(ui_Widget);
    lv_label_set_text(subtitle, "Choose how the target platform should be reached");
    lv_obj_align(subtitle, LV_ALIGN_TOP_MID, 0, 75);
    lv_obj_set_style_text_color(subtitle, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);

    createOptionButton("Cable (UART)", 70, 130, DefaultCommunicationMode::CABLE);
    createOptionButton("Wireless Auto", 395, 130, DefaultCommunicationMode::WIRELESS_AUTO, false);
    createWirelessManualButton(625, 130);

    lv_obj_t *back = lv_btn_create(ui_Widget);
    lv_obj_set_size(back, 90, 36);
    lv_obj_align(back, LV_ALIGN_BOTTOM_LEFT, 16, -14);
    lv_obj_add_event_cb(back, [](lv_event_t *e) {
        if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
            auto *self = static_cast<CommunicationSelectionGui *>(lv_event_get_user_data(e));
            self->router.showMainMenu();
        }
    }, LV_EVENT_ALL, this);
    lv_obj_t *backLabel = lv_label_create(back);
    lv_label_set_text(backLabel, "Back");
    lv_obj_center(backLabel);

    lv_obj_t *hint = lv_label_create(ui_Widget);
    lv_label_set_text(hint, "For cable connection (UART), please check if cable is in place.");
    lv_obj_align(hint, LV_ALIGN_BOTTOM_MID, 0, -16);
    lv_obj_set_style_text_color(hint, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(hint, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
}

void CommunicationSelectionGui::hideCommunicationSelection(void)
{
    if (!initialized) {
        return;
    }

    if (ui_Widget) {
        lv_obj_del(ui_Widget);
    }

    ui_Widget = nullptr;
    ui_LoadingOverlay = nullptr;
    ui_LoadingLabel = nullptr;
    connectionBusy = false;
    initialized = false;
}
