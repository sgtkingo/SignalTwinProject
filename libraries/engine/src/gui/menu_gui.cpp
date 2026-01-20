/**
 * @file menu_gui.cpp
 * @brief Implementation of the MenuGui class
 *
 * This source file implements the MenuGui functionality for
 * menu visualization and sensor-to-pin assignment operations.
 *
 * @copyright 2025 MTA
 * @author Ing. Jiri Konecny, Ondřej Wrubel
 */

#include "menu_gui.hpp"
#include "../helpers.hpp"

MenuGui::MenuGui(SensorManager &sensorManager) : sensorManager(sensorManager), activePinIndex(-1)
{
    // Initialize all GUI pointers to nullptr
    ui_MenuWidget = nullptr;
    ui_ButtonConnectGroup = nullptr;
    ui_ButtonConnectCornerTopLeft = nullptr;
    ui_ButtonConnectCornerBottomRight = nullptr;
    ui_btnConnect = nullptr;
    ui_ButtonConnectLabel = nullptr;

    pinContainers.fill(nullptr);
    pinLabels.fill(nullptr);

    // Initialize GPIO pin numbers
    initializePins();
}

void MenuGui::init()
{
    if (initialized)
        return;

    try
    {
        // logMessage("Initializing MenuGui...\n");
        buildMenu();
        initialized = true;
        // logMessage("MenuGui initialization completed!\n");
    }
    catch (const std::exception &e)
    {
        // logMessage("MenuGui initialization failed: %s\n", e.what());
        initialized = false;
    }
}

void MenuGui::buildMenu()
{
    // logMessage("\t>building menu...\n");

    // Main container widget
    ui_MenuWidget = lv_obj_create(lv_scr_act());
    lv_obj_remove_style_all(ui_MenuWidget);
    lv_obj_set_size(ui_MenuWidget, 760, 440);
    lv_obj_set_align(ui_MenuWidget, LV_ALIGN_CENTER);
    lv_obj_set_style_radius(ui_MenuWidget, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_MenuWidget, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_MenuWidget, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_MenuWidget, 2, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Connect button group
    ui_ButtonConnectGroup = lv_obj_create(ui_MenuWidget);
    lv_obj_remove_style_all(ui_ButtonConnectGroup);
    lv_obj_set_width(ui_ButtonConnectGroup, 100);
    lv_obj_set_height(ui_ButtonConnectGroup, 45);
    lv_obj_set_align(ui_ButtonConnectGroup, LV_ALIGN_TOP_RIGHT);
    lv_obj_clear_flag(ui_ButtonConnectGroup, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);

    // Connect button decorative corners
    ui_ButtonConnectCornerTopLeft = lv_obj_create(ui_ButtonConnectGroup);
    lv_obj_remove_style_all(ui_ButtonConnectCornerTopLeft);
    lv_obj_set_width(ui_ButtonConnectCornerTopLeft, 20);
    lv_obj_set_height(ui_ButtonConnectCornerTopLeft, 20);
    lv_obj_clear_flag(ui_ButtonConnectCornerTopLeft, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(ui_ButtonConnectCornerTopLeft, lv_color_hex(0x009BFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_ButtonConnectCornerTopLeft, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui_ButtonConnectCornerTopLeft, false, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_ButtonConnectCornerBottomRight = lv_obj_create(ui_ButtonConnectGroup);
    lv_obj_remove_style_all(ui_ButtonConnectCornerBottomRight);
    lv_obj_set_width(ui_ButtonConnectCornerBottomRight, 20);
    lv_obj_set_height(ui_ButtonConnectCornerBottomRight, 20);
    lv_obj_set_align(ui_ButtonConnectCornerBottomRight, LV_ALIGN_BOTTOM_RIGHT);
    lv_obj_clear_flag(ui_ButtonConnectCornerBottomRight, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(ui_ButtonConnectCornerBottomRight, lv_color_hex(0x009BFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_ButtonConnectCornerBottomRight, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui_ButtonConnectCornerBottomRight, false, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Connect button
    ui_btnConnect = lv_btn_create(ui_ButtonConnectGroup);
    lv_obj_set_width(ui_btnConnect, 100);
    lv_obj_set_height(ui_btnConnect, 45);
    lv_obj_add_flag(ui_btnConnect, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_clear_flag(ui_btnConnect, LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE |
                                         LV_OBJ_FLAG_SNAPPABLE | LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                                         LV_OBJ_FLAG_SCROLL_CHAIN);
    lv_obj_set_style_clip_corner(ui_btnConnect, false, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Connect button event handler
    lv_obj_add_event_cb(ui_btnConnect, [](lv_event_t *e)
                        {
        auto self = static_cast<MenuGui*>(lv_event_get_user_data(e));
        self->handleConnectButtonClick(); }, LV_EVENT_CLICKED, this);

    // Connect button label
    ui_ButtonConnectLabel = lv_label_create(ui_btnConnect);
    lv_obj_set_width(ui_ButtonConnectLabel, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_ButtonConnectLabel, LV_SIZE_CONTENT);
    lv_obj_set_align(ui_ButtonConnectLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_ButtonConnectLabel, "Connect");
    lv_obj_set_style_text_font(ui_ButtonConnectLabel, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Create scrollable container for pins
    ui_PinScrollContainer = lv_obj_create(ui_MenuWidget);
    lv_obj_set_size(ui_PinScrollContainer, 750, 375);
    lv_obj_align(ui_PinScrollContainer, LV_ALIGN_CENTER, 0, 20);

    // Configure scrollable container
    lv_obj_set_scroll_dir(ui_PinScrollContainer, LV_DIR_VER);
    lv_obj_set_scrollbar_mode(ui_PinScrollContainer, LV_SCROLLBAR_MODE_AUTO);
    lv_obj_set_style_pad_all(ui_PinScrollContainer, 10, LV_PART_MAIN);
    lv_obj_set_style_pad_gap(ui_PinScrollContainer, 10, LV_PART_MAIN);

    // Set flex layout for automatic arrangement
    lv_obj_set_flex_flow(ui_PinScrollContainer, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(ui_PinScrollContainer,
                          LV_FLEX_ALIGN_START,
                          LV_FLEX_ALIGN_START,
                          LV_FLEX_ALIGN_START);

    // Create pin containers within the scrollable container
    for (int i = 0; i < NUM_PINS; ++i)
    {
        pinContainers[i] = lv_btn_create(ui_PinScrollContainer);
        lv_obj_set_size(pinContainers[i], 234, 100);

        // Pin container event handler
        lv_obj_add_event_cb(pinContainers[i], [](lv_event_t *e)
                            {
            auto self = static_cast<MenuGui*>(lv_event_get_user_data(e));
            int index = (intptr_t)lv_obj_get_user_data(lv_event_get_target(e));
            self->handlePinClick(index); }, LV_EVENT_CLICKED, this);

        // Store pin index as user data
        lv_obj_set_user_data(pinContainers[i], (void *)(intptr_t)i);

        // Pin labels
        pinLabels[i] = lv_label_create(pinContainers[i]);
        lv_label_set_text_fmt(pinLabels[i], "Pin %d", i + 1);
        lv_obj_center(pinLabels[i]);
    }

    // Title
    lv_obj_t *title = lv_label_create(ui_MenuWidget);
    lv_label_set_text(title, "Assign Sensors to Pins");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);

    // logMessage("\t>menu built successfully!\n");
}

void MenuGui::showMenu()
{
    if (!initialized || !ui_MenuWidget)
        return;

    lv_obj_clear_flag(ui_MenuWidget, LV_OBJ_FLAG_HIDDEN);
    updatePinVisualStates();
    // logMessage("Showing menu GUI\n");
}

void MenuGui::hideMenu()
{
    if (!initialized || !ui_MenuWidget)
        return;

    lv_obj_add_flag(ui_MenuWidget, LV_OBJ_FLAG_HIDDEN);
    // logMessage("Hiding menu GUI\n");
}

void MenuGui::updatePinLabels()
{
    for (int i = 0; i < NUM_PINS; i++)
    {
        if (!pinLabels[i])
            continue;

        std::string labelText;
        int gpioNumber = sensorManager.getPinNumber(i);
        BaseSensor *assignedSensor = sensorManager.getAssignedSensor(i);

        if (assignedSensor != nullptr)
        {
            // Show sensor name for assigned pins
            labelText = "GPIO " + std::to_string(gpioNumber) + "\n" + assignedSensor->getTypeName();
        }
        else
        {
            // Show GPIO number for available pins
            labelText = "GPIO " + std::to_string(gpioNumber) + "\nAvailable";
        }

        lv_label_set_text(pinLabels[i], labelText.c_str());
    }
}

void MenuGui::updatePinVisualStates()
{
    updatePinLabels(); // Ensure labels are up to date

    for (int i = 0; i < NUM_PINS; i++)
    {
        if (!pinContainers[i])
            continue;

        bool isActive = (i == activePinIndex);
        bool isAvailable = sensorManager.isPinAvailable(i);
        BaseSensor *assignedSensor = sensorManager.getAssignedSensor(i);
        int gpioNumber = sensorManager.getPinNumber(i);

        // Determine colors: Green (available), Red (used), Blue (active)
        uint32_t pinStateColor;
        if (isAvailable)
        {
            pinStateColor = 0x00CC00; // Green for available
        }
        else
        {
            pinStateColor = 0xFF0000; // Red for used
        }

        // Set background color based on pin state
        if (isActive)
        {
            // Active pin - highlighted with blue border
            lv_obj_set_style_bg_color(pinContainers[i], lv_color_hex(pinStateColor), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(pinContainers[i], 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(pinContainers[i], 3, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_color(pinContainers[i], lv_color_hex(0x0066CC), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        else
        {
            // Set color based on pin state: Red (used), Green (available)
            lv_obj_set_style_bg_color(pinContainers[i], lv_color_hex(pinStateColor), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(pinContainers[i], 200, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(pinContainers[i], 1, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_color(pinContainers[i], lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
        }

        // Set text color for contrast based on background
        if (!isAvailable || isActive)
        {
            lv_obj_set_style_text_color(pinLabels[i], lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        else
        {
            lv_obj_set_style_text_color(pinLabels[i], lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
        }

        // logMessage("Pin %d (GPIO %d) - Available: %s, Active: %s, Assigned: %s\n",
        //           i, gpioNumber,
        //           isAvailable ? "Yes" : "No",
        //           isActive ? "Yes" : "No",
        //           assignedSensor ? assignedSensor->getName().c_str() : "None");
    }
}

void MenuGui::setActivePin(int pinIndex)
{
    if (pinIndex < -1 || pinIndex >= NUM_PINS)
    {
        // logMessage("Invalid pin index: %d\n", pinIndex);
        return;
    }

    activePinIndex = pinIndex;
    updatePinVisualStates();
    // logMessage("Active pin set to: %d\n", activePinIndex);
}

void MenuGui::handleConnectButtonClick()
{
    switchToWiki();
}

void MenuGui::handlePinClick(int pinIndex)
{
    BaseSensor *sensor = sensorManager.getCurrentWikiSensor();
    if (!sensor || pinIndex < 0)
    {
        logMessage("Cannot select sensor: no sensor or no active pin\n");
        return;
    }
    setActivePin(pinIndex);
    if (!sensorManager.isPinAvailable(activePinIndex))
    {
        if (sensor != sensorManager.getAssignedSensor(activePinIndex))
        {
            if (sensorManager.unassignSensorFromPin(activePinIndex))
            {
                bool success = sensorManager.assignSensorToPin(sensor, activePinIndex);
                initializePins();
            }
            else
            {
                splashMessage("Failed to unassign sensor from pin\n");
                return;
            }
        }
        else if (sensorManager.unassignSensorFromPin(activePinIndex))
        {
            initializePins();
        }
        else
        {
            splashMessage("Failed to unassign sensor from pin\n");
            return;
        }
    }
    else
    {
        bool success = sensorManager.assignSensorToPin(sensor, activePinIndex);
        if (success)
        {
            initializePins();
        }
        else
        {
            splashMessage("Failed to assign sensor to pin\n");
            return;
        }
    }
}

void MenuGui::initializePins()
{
    updatePinVisualStates();
}