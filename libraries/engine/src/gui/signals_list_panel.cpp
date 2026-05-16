/**
 * @file signals_list_panel.cpp
 * @brief Scrollable signal/config panel helper for SignalsVisualizationGui.
 */

#include "signals_list_panel.hpp"

#include <cstdint>

namespace
{
constexpr lv_coord_t RUNTIME_LIST_EDITOR_WIDTH = 142;
constexpr lv_coord_t RUNTIME_LIST_EDITOR_HEIGHT = 34;
constexpr lv_coord_t RUNTIME_LIST_SLIDER_HEIGHT = 16;
constexpr lv_coord_t RUNTIME_LIST_EDITOR_RIGHT_OFFSET = -8;
constexpr lv_coord_t RUNTIME_LIST_EDITOR_BOTTOM_OFFSET = -2;
constexpr lv_coord_t RUNTIME_LIST_SLIDER_BOTTOM_OFFSET = -12;
}

void SignalsListPanel::create(lv_obj_t *parent,
                              void *userData,
                              lv_event_cb_t dropdownCb,
                              lv_event_cb_t sliderCb,
                              lv_event_cb_t textCb)
{
    eventUserData = userData;
    dropdownChangedCb = dropdownCb;
    sliderChangedCb = sliderCb;
    textSubmittedCb = textCb;

    container = lv_obj_create(parent);
    lv_obj_set_size(container, 200, 260);
    lv_obj_set_x(container, -267);
    lv_obj_set_y(container, -15);
    lv_obj_set_align(container, LV_ALIGN_CENTER);
    lv_obj_set_scroll_dir(container, LV_DIR_VER);
    lv_obj_set_scrollbar_mode(container, LV_SCROLLBAR_MODE_AUTO);
    lv_obj_set_style_pad_all(container, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_gap(container, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(container, lv_color_hex(0xF8FAFC), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(container, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(container, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(container, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(container, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_flex_flow(container, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(container, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
}

void SignalsListPanel::ensureSignalCards(size_t count)
{
    if (!container) {
        return;
    }

    while (signalCards.size() < count) {
        SignalCard card;
        card.container = lv_obj_create(container);
        lv_obj_set_width(card.container, lv_pct(100));
        lv_obj_set_height(card.container, 74);
        lv_obj_clear_flag(card.container, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_set_style_pad_left(card.container, 16, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_right(card.container, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_top(card.container, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_bottom(card.container, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_radius(card.container, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(card.container, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_color(card.container, lv_color_hex(0xD0D7DE), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(card.container, 1, LV_PART_MAIN | LV_STATE_DEFAULT);

        card.accent = lv_obj_create(card.container);
        lv_obj_remove_style_all(card.accent);
        lv_obj_set_size(card.accent, 6, 54);
        lv_obj_align(card.accent, LV_ALIGN_LEFT_MID, -8, 0);
        lv_obj_set_style_radius(card.accent, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(card.accent, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

        card.nameLabel = lv_label_create(card.container);
        lv_obj_align(card.nameLabel, LV_ALIGN_TOP_LEFT, 6, 0);
        lv_obj_set_style_text_font(card.nameLabel, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(card.nameLabel, lv_color_hex(0x3B4252), LV_PART_MAIN | LV_STATE_DEFAULT);

        card.valueLabel = lv_label_create(card.container);
        lv_obj_align(card.valueLabel, LV_ALIGN_CENTER, 0, 6);
        lv_obj_set_style_text_font(card.valueLabel, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(card.valueLabel, lv_color_hex(0x111111), LV_PART_MAIN | LV_STATE_DEFAULT);

        card.unitLabel = lv_label_create(card.container);
        lv_obj_align(card.unitLabel, LV_ALIGN_BOTTOM_LEFT, 6, 0);
        lv_obj_set_width(card.unitLabel, 190);
        lv_obj_set_style_text_font(card.unitLabel, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(card.unitLabel, lv_color_hex(0x5F6B7A), LV_PART_MAIN | LV_STATE_DEFAULT);

        signalCards.push_back(card);
    }
}

void SignalsListPanel::clearUnusedSignalCards(size_t usedCount)
{
    for (size_t i = 0; i < signalCards.size(); ++i) {
        if (!signalCards[i].container) {
            continue;
        }

        if (i < usedCount) {
            lv_obj_clear_flag(signalCards[i].container, LV_OBJ_FLAG_HIDDEN);
        } else {
            lv_obj_add_flag(signalCards[i].container, LV_OBJ_FLAG_HIDDEN);
        }
    }
}

void SignalsListPanel::setSignalCard(size_t index,
                                     uint32_t accentColor,
                                     const std::string &name,
                                     const std::string &value,
                                     const std::string &unit)
{
    if (index >= signalCards.size()) {
        return;
    }

    lv_obj_set_style_bg_color(signalCards[index].accent, lv_color_hex(accentColor), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text(signalCards[index].nameLabel, name.c_str());
    lv_label_set_text(signalCards[index].valueLabel, value.c_str());
    lv_label_set_text(signalCards[index].unitLabel, unit.c_str());
}

void SignalsListPanel::setAllSignalCardValues(const char *value)
{
    for (auto &card : signalCards) {
        if (card.valueLabel) {
            lv_label_set_text(card.valueLabel, value ? value : "");
        }
    }
}

void SignalsListPanel::ensureConfigControls(size_t count)
{
    if (!container) {
        return;
    }

    while (configControls.size() < count) {
        const size_t nextIndex = configControls.size();

        ConfigControl control;
        control.container = lv_obj_create(container);
        lv_obj_set_width(control.container, lv_pct(100));
        lv_obj_set_height(control.container, 116);
        lv_obj_clear_flag(control.container, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_set_style_pad_left(control.container, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_right(control.container, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_top(control.container, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_bottom(control.container, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_radius(control.container, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(control.container, lv_color_hex(0xF3F9FF), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_color(control.container, lv_color_hex(0x8FBDE8), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(control.container, 1, LV_PART_MAIN | LV_STATE_DEFAULT);

        control.accent = lv_obj_create(control.container);
        lv_obj_remove_style_all(control.accent);
        lv_obj_set_size(control.accent, 6, 88);
        lv_obj_align(control.accent, LV_ALIGN_LEFT_MID, -5, 0);
        lv_obj_set_style_radius(control.accent, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(control.accent, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

        control.nameLabel = lv_label_create(control.container);
        lv_obj_align(control.nameLabel, LV_ALIGN_TOP_LEFT, 6, 0);
        lv_obj_set_width(control.nameLabel, 100);
        lv_label_set_long_mode(control.nameLabel, LV_LABEL_LONG_DOT);
        lv_obj_set_style_text_font(control.nameLabel, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(control.nameLabel, lv_color_hex(0x24415E), LV_PART_MAIN | LV_STATE_DEFAULT);

        control.valueLabel = lv_label_create(control.container);
        lv_obj_align(control.valueLabel, LV_ALIGN_TOP_RIGHT, -8, 0);
        lv_obj_set_width(control.valueLabel, 58);
        lv_label_set_long_mode(control.valueLabel, LV_LABEL_LONG_DOT);
        lv_obj_set_style_text_align(control.valueLabel, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_font(control.valueLabel, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(control.valueLabel, lv_color_hex(0x0B7285), LV_PART_MAIN | LV_STATE_DEFAULT);

        control.unitLabel = lv_label_create(control.container);
        lv_obj_align(control.unitLabel, LV_ALIGN_BOTTOM_LEFT, 6, 0);
        lv_obj_set_width(control.unitLabel, 120);
        lv_label_set_long_mode(control.unitLabel, LV_LABEL_LONG_DOT);
        lv_obj_set_style_text_font(control.unitLabel, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(control.unitLabel, lv_color_hex(0x5F6B7A), LV_PART_MAIN | LV_STATE_DEFAULT);

        control.editor = lv_textarea_create(control.container);
        lv_obj_set_size(control.editor, RUNTIME_LIST_EDITOR_WIDTH, RUNTIME_LIST_EDITOR_HEIGHT);
        lv_obj_align(control.editor, LV_ALIGN_BOTTOM_RIGHT, RUNTIME_LIST_EDITOR_RIGHT_OFFSET, RUNTIME_LIST_EDITOR_BOTTOM_OFFSET);
        lv_textarea_set_one_line(control.editor, true);
        lv_textarea_set_max_length(control.editor, 24);
        lv_obj_set_user_data(control.editor, reinterpret_cast<void *>(static_cast<intptr_t>(nextIndex)));
        if (textSubmittedCb) {
            lv_obj_add_event_cb(control.editor, textSubmittedCb, LV_EVENT_ALL, eventUserData);
        }

        configControls.push_back(control);
    }
}

void SignalsListPanel::clearUnusedConfigControls(size_t usedCount)
{
    for (size_t i = 0; i < configControls.size(); ++i) {
        if (!configControls[i].container) {
            continue;
        }

        if (i < usedCount) {
            lv_obj_clear_flag(configControls[i].container, LV_OBJ_FLAG_HIDDEN);
        } else {
            lv_obj_add_flag(configControls[i].container, LV_OBJ_FLAG_HIDDEN);
        }
    }
}

SignalsListPanel::ConfigControl *SignalsListPanel::getConfigControl(size_t index)
{
    if (index >= configControls.size()) {
        return nullptr;
    }

    return &configControls[index];
}

void SignalsListPanel::setControlVisual(size_t index,
                                        uint32_t accentColor,
                                        const std::string &name,
                                        const std::string &value,
                                        const std::string &unit,
                                        const std::string &key,
                                        bool isValueControl)
{
    ConfigControl *control = getConfigControl(index);
    if (!control) {
        return;
    }

    control->key = key;
    control->isValueControl = isValueControl;
    lv_obj_set_style_bg_color(control->container, lv_color_hex(isValueControl ? 0xFFF4E6 : 0xF3F9FF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(control->container, lv_color_hex(isValueControl ? 0xF08C00 : 0x8FBDE8), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(control->accent, lv_color_hex(accentColor), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text(control->nameLabel, name.c_str());
    lv_label_set_text(control->valueLabel, value.c_str());
    lv_label_set_text(control->unitLabel, unit.c_str());
}

void SignalsListPanel::ensureDropdownEditor(size_t index, const std::string &optionsText, uint16_t selectedIndex)
{
    ConfigControl *control = getConfigControl(index);
    if (!control) {
        return;
    }

    if (control->editor) {
        lv_obj_del(control->editor);
    }

    control->editor = lv_dropdown_create(control->container);
    control->usesDropdown = true;
    control->usesSlider = false;
    lv_obj_set_size(control->editor, RUNTIME_LIST_EDITOR_WIDTH, RUNTIME_LIST_EDITOR_HEIGHT);
    lv_obj_align(control->editor, LV_ALIGN_BOTTOM_RIGHT, RUNTIME_LIST_EDITOR_RIGHT_OFFSET, RUNTIME_LIST_EDITOR_BOTTOM_OFFSET);
    lv_obj_set_user_data(control->editor, reinterpret_cast<void *>(static_cast<intptr_t>(index)));
    if (dropdownChangedCb) {
        lv_obj_add_event_cb(control->editor, dropdownChangedCb, LV_EVENT_ALL, eventUserData);
    }
    lv_dropdown_set_options(control->editor, optionsText.c_str());
    lv_dropdown_set_selected(control->editor, selectedIndex);
}

void SignalsListPanel::ensureSliderEditor(size_t index, int minValue, int maxValue, int currentValue)
{
    ConfigControl *control = getConfigControl(index);
    if (!control) {
        return;
    }

    if (control->editor) {
        lv_obj_del(control->editor);
    }

    control->editor = lv_slider_create(control->container);
    control->usesDropdown = false;
    control->usesSlider = true;
    lv_obj_set_size(control->editor, RUNTIME_LIST_EDITOR_WIDTH, RUNTIME_LIST_SLIDER_HEIGHT);
    lv_obj_align(control->editor, LV_ALIGN_BOTTOM_RIGHT, RUNTIME_LIST_EDITOR_RIGHT_OFFSET, RUNTIME_LIST_SLIDER_BOTTOM_OFFSET);
    lv_obj_set_user_data(control->editor, reinterpret_cast<void *>(static_cast<intptr_t>(index)));
    if (sliderChangedCb) {
        lv_obj_add_event_cb(control->editor, sliderChangedCb, LV_EVENT_ALL, eventUserData);
    }
    lv_slider_set_range(control->editor, minValue, maxValue);
    lv_slider_set_value(control->editor, currentValue, LV_ANIM_OFF);
}

void SignalsListPanel::ensureTextEditor(size_t index, const std::string &value)
{
    ConfigControl *control = getConfigControl(index);
    if (!control) {
        return;
    }

    if (!control->editor || control->usesDropdown || control->usesSlider) {
        if (control->editor) {
            lv_obj_del(control->editor);
        }

        control->editor = lv_textarea_create(control->container);
        lv_obj_set_size(control->editor, RUNTIME_LIST_EDITOR_WIDTH, RUNTIME_LIST_EDITOR_HEIGHT);
        lv_obj_align(control->editor, LV_ALIGN_BOTTOM_RIGHT, RUNTIME_LIST_EDITOR_RIGHT_OFFSET, RUNTIME_LIST_EDITOR_BOTTOM_OFFSET);
        lv_textarea_set_one_line(control->editor, true);
        lv_textarea_set_max_length(control->editor, 24);
        lv_obj_set_user_data(control->editor, reinterpret_cast<void *>(static_cast<intptr_t>(index)));
        if (textSubmittedCb) {
            lv_obj_add_event_cb(control->editor, textSubmittedCb, LV_EVENT_ALL, eventUserData);
        }
    }

    control->usesDropdown = false;
    control->usesSlider = false;
    lv_textarea_set_text(control->editor, value.c_str());
}
