/**
 * @file sensor_wiki_gui.cpp
 * @brief Implementation of the SensorWikiGui class
 *
 * This source file implements the sensor wiki functionality for
 * displaying sensor information and handling sensor selection.
 *
 * @copyright 2025 MTA
 * @author Ing. Jiri Konecny, Ondřej Wrubel
 */

#include "sensor_wiki_gui.hpp"
#include "../helpers.hpp"

SensorWikiGui::SensorWikiGui(SensorManager &sensorManager)
    : sensorManager(sensorManager)
{
    // Initialize all GUI pointers to nullptr
    ui_WikiWidget = nullptr;
    ui_SensorList = nullptr;
    ui_SensorInfo = nullptr;
    ui_SensorTitle = nullptr;
    ui_SensorDescriptionTitle = nullptr;
    ui_SensorDescription = nullptr;
    ui_SensorSpecsTitle = nullptr;
    ui_SensorSpecs = nullptr;
    ui_SensorConfTitle = nullptr;
    ui_SensorConf = nullptr;
    ui_SelectButton = nullptr;
    ui_StartButton = nullptr;
    ui_StartButtonLabel = nullptr;
    ui_PrevButton = nullptr;
    ui_NextButton = nullptr;
}

void SensorWikiGui::init()
{
    if (initialized)
        return;

    try
    {
        // logMessage("Initializing SensorWikiGui...\n");
        buildWikiGui();
        initialized = true;
        // logMessage("SensorWikiGui initialization completed!\n");
    }
    catch (const std::exception &e)
    {
        // logMessage("SensorWikiGui initialization failed: %s\n", e.what());
        initialized = false;
    }
}

void SensorWikiGui::buildWikiGui()
{
    // logMessage("\t>building sensor wiki gui...\n");

    // Main wiki container
    ui_WikiWidget = lv_obj_create(lv_scr_act());
    lv_obj_remove_style_all(ui_WikiWidget);
    lv_obj_set_size(ui_WikiWidget, 760, 440);
    lv_obj_set_align(ui_WikiWidget, LV_ALIGN_CENTER);
    lv_obj_set_style_radius(ui_WikiWidget, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_WikiWidget, lv_color_hex(0xF0F0F0), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_WikiWidget, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_WikiWidget, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_flag(ui_WikiWidget, LV_OBJ_FLAG_HIDDEN);

    // Sensor information panel
    ui_SensorInfo = lv_obj_create(ui_WikiWidget);
    lv_obj_set_size(ui_SensorInfo, 480, 415);
    lv_obj_set_pos(ui_SensorInfo, 10, 10);
    lv_obj_set_style_bg_color(ui_SensorInfo, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_SensorInfo, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui_SensorInfo, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_clear_flag(ui_SensorDescription, LV_OBJ_FLAG_SCROLL_ON_FOCUS);

    // Sensor title
    ui_SensorTitle = lv_label_create(ui_SensorInfo);
    lv_obj_set_size(ui_SensorTitle, 400, 40);
    lv_obj_set_pos(ui_SensorTitle, 5, -5);
    lv_label_set_text(ui_SensorTitle, "Sensor Name");
    lv_obj_set_style_text_font(ui_SensorTitle, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_SensorTitle, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);

    // Sensor description title
    ui_SensorDescriptionTitle = lv_label_create(ui_SensorInfo);
    lv_obj_set_size(ui_SensorDescriptionTitle, 200, 40);
    lv_obj_set_pos(ui_SensorDescriptionTitle, 10, 30);
    lv_label_set_text(ui_SensorDescriptionTitle, "Description:");
    lv_obj_set_style_text_font(ui_SensorDescriptionTitle, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_SensorDescriptionTitle, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);

    // Sensor description
    ui_SensorDescription = lv_textarea_create(ui_SensorInfo);
    lv_obj_set_size(ui_SensorDescription, 420, 90);
    lv_obj_set_pos(ui_SensorDescription, 10, 60);
    lv_textarea_set_text(ui_SensorDescription, "Sensor description will appear here...");
    lv_obj_add_flag(ui_SensorDescription, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    lv_obj_clear_flag(ui_SensorDescription, LV_OBJ_FLAG_CLICKABLE);

    // Sensor specifications title
    ui_SensorSpecsTitle = lv_label_create(ui_SensorInfo);
    lv_obj_set_size(ui_SensorSpecsTitle, 200, 40);
    lv_obj_set_pos(ui_SensorSpecsTitle, 10, 160);
    lv_label_set_text(ui_SensorSpecsTitle, "Specifications:");
    lv_obj_set_style_text_font(ui_SensorSpecsTitle, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_SensorSpecsTitle, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);

    // Sensor specifications
    ui_SensorSpecs = lv_textarea_create(ui_SensorInfo);
    lv_obj_set_size(ui_SensorSpecs, 200, 180);
    lv_obj_set_pos(ui_SensorSpecs, 10, 190);
    lv_textarea_set_text(ui_SensorSpecs, "Specifications will appear here...");
    lv_obj_add_flag(ui_SensorSpecs, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    lv_obj_clear_flag(ui_SensorSpecs, LV_OBJ_FLAG_CLICKABLE);

    // Sensor specifications title
    ui_SensorConfTitle = lv_label_create(ui_SensorInfo);
    lv_obj_set_size(ui_SensorConfTitle, 180, 40);
    lv_obj_set_pos(ui_SensorConfTitle, 230, 160);
    lv_label_set_text(ui_SensorConfTitle, "Configurations:");
    lv_obj_set_style_text_font(ui_SensorConfTitle, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_SensorConfTitle, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);

    // Sensor specifications
    ui_SensorConf = lv_textarea_create(ui_SensorInfo);
    lv_obj_set_size(ui_SensorConf, 200, 180);
    lv_obj_set_pos(ui_SensorConf, 230, 190);
    lv_textarea_set_text(ui_SensorConf, "Configuration will appear here...");
    lv_obj_add_flag(ui_SensorConf, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    lv_obj_clear_flag(ui_SensorConf, LV_OBJ_FLAG_CLICKABLE);

    // Control panel
    lv_obj_t *controlPanel = lv_obj_create(ui_WikiWidget);
    lv_obj_set_size(controlPanel, 245, 415);
    lv_obj_set_pos(controlPanel, 500, 10);
    lv_obj_set_style_bg_color(controlPanel, lv_color_hex(0xE0E0E0), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(controlPanel, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(controlPanel, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_scrollbar_mode(controlPanel, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_pad_all(controlPanel, 0, 0);
    lv_obj_set_style_pad_hor(controlPanel, 0, 0);

    // Navigation buttons
    ui_PrevButton = lv_btn_create(controlPanel);
    lv_obj_set_size(ui_PrevButton, 90, 40);
    lv_obj_set_pos(ui_PrevButton, 27, 20);
    lv_obj_t *prevLabel = lv_label_create(ui_PrevButton);
    lv_label_set_text(prevLabel, "< Prev");
    lv_obj_center(prevLabel);
    lv_obj_add_event_cb(ui_PrevButton, [](lv_event_t *e)
                        {
        auto self = static_cast<SensorWikiGui*>(lv_event_get_user_data(e));
        self->navigateSensor(-1); }, LV_EVENT_CLICKED, this);

    ui_NextButton = lv_btn_create(controlPanel);
    lv_obj_set_size(ui_NextButton, 90, 40);
    lv_obj_set_pos(ui_NextButton, 127, 20);
    lv_obj_t *nextLabel = lv_label_create(ui_NextButton);
    lv_label_set_text(nextLabel, "Next >");
    lv_obj_center(nextLabel);
    lv_obj_add_event_cb(ui_NextButton, [](lv_event_t *e)
                        {
        auto self = static_cast<SensorWikiGui*>(lv_event_get_user_data(e));
        self->navigateSensor(1); }, LV_EVENT_CLICKED, this);

    // Select button
    ui_SelectButton = lv_btn_create(controlPanel);
    lv_obj_set_size(ui_SelectButton, 210, 50);
    lv_obj_set_pos(ui_SelectButton, 17, 80);
    lv_obj_set_style_bg_color(ui_SelectButton, lv_color_hex(0x00AA00), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_t *selectLabel = lv_label_create(ui_SelectButton);
    lv_label_set_text(selectLabel, "SELECT SENSOR");
    lv_obj_center(selectLabel);
    lv_obj_set_style_text_color(selectLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(ui_SelectButton, [](lv_event_t *e)
                        {
        auto self = static_cast<SensorWikiGui*>(lv_event_get_user_data(e));
        self->handleSelectButtonClick(); }, LV_EVENT_CLICKED, this);

    // Back button
    ui_StartButton = lv_btn_create(controlPanel);
    lv_obj_set_size(ui_StartButton, 210, 40);
    lv_obj_set_pos(ui_StartButton, 17, 330);
    ui_StartButtonLabel = lv_label_create(ui_StartButton);
    lv_label_set_text(ui_StartButtonLabel, "START VISUALISATION");
    lv_obj_center(ui_StartButtonLabel);
    lv_obj_set_style_text_color(ui_StartButtonLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(ui_StartButton, [](lv_event_t *e)
                        {
        auto self = static_cast<SensorWikiGui*>(lv_event_get_user_data(e));
        self->handleStartButtonClick(); }, LV_EVENT_CLICKED, this);

    // Sensor list (simple for now)
    ui_SensorList = lv_list_create(controlPanel);
    lv_obj_set_size(ui_SensorList, 210, 180);
    lv_obj_set_pos(ui_SensorList, 17, 140);
}

void SensorWikiGui::updateSensorInfo()
{
    BaseSensor *sensor = getSelectedSensor();
    if (!sensor)
    {
        lv_label_set_text(ui_SensorTitle, "No Sensor Selected");
        lv_textarea_set_text(ui_SensorDescription, "No sensor available for display.");
        lv_textarea_set_text(ui_SensorSpecs, "No specifications available.");
        return;
    }

    // Update title
    lv_label_set_text(ui_SensorTitle, sensor->getName().c_str());

    // Update description
    std::string description = getSensorInfoText(sensor);
    lv_textarea_set_text(ui_SensorDescription, description.c_str());

    // Update specifications
    std::string specs = getSensorSpecsText(sensor);
    lv_textarea_set_text(ui_SensorSpecs, specs.c_str());

    // Update configuration
    std::string conf = getSensorConfText(sensor);
    lv_textarea_set_text(ui_SensorConf, conf.c_str());
}

std::string SensorWikiGui::getSensorInfoText(BaseSensor *sensor)
{
    if (!sensor)
        return "No sensor information available.";

    std::string info = "Type: " + sensor->getTypeName() + "\n\n";
    // info += "Description: " + sensor->getDescription() + "\n\n";
    info += sensor->getDescription();

    return info;
}

std::string SensorWikiGui::getSensorSpecsText(BaseSensor *sensor)
{
    if (!sensor)
        return "No specifications available.";

    std::string specs = "";
    specs += "Sensor ID: " + sensor->getId() + "\n";
    specs += "Type: " + sensor->getTypeName() + "\n";
    // Loops values and configs
    specs += "Values:\n";
    for (const auto &[key, param] : sensor->getValues())
    {
        specs += "\t" + key + ": " + param.Value + " (" + param.Unit + ")\n";
    }

    return specs;
}

std::string SensorWikiGui::getSensorConfText(BaseSensor *sensor)
{
    if (!sensor)
        return "No configuration available.";

    std::string conf = "";

    for (const auto &[key, param] : sensor->getConfigs())
    {
        conf += "\t" + key + ": " + param.Value + " (" + param.Unit + ")\n";
    }

    return conf;
}

void SensorWikiGui::showWiki(int pinIndex)
{
    if (!initialized)
        return;

    activePinIndex = pinIndex;
    updateSensorInfo();
    lv_obj_clear_flag(ui_WikiWidget, LV_OBJ_FLAG_HIDDEN);

    // logMessage("Showing sensor wiki for pin %d\n", pinIndex);
}

void SensorWikiGui::hideWiki()
{
    if (!initialized)
        return;

    lv_obj_add_flag(ui_WikiWidget, LV_OBJ_FLAG_HIDDEN);
    activePinIndex = -1;
}

BaseSensor *SensorWikiGui::getSelectedSensor()
{
    const auto &sensors = sensorManager.getSensors();
    if (sensors.empty() || selectedSensorIndex >= static_cast<int>(sensors.size()))
    {
        return nullptr;
    }
    return sensors[selectedSensorIndex];
}

void SensorWikiGui::setSelectedSensor(int index)
{
    const auto &sensors = sensorManager.getSensors();
    if (index >= 0 && index < static_cast<int>(sensors.size()))
    {
        selectedSensorIndex = index;
        updateSensorInfo();
    }
}

void SensorWikiGui::navigateSensor(int direction)
{
    const auto &sensors = sensorManager.getSensors();
    if (sensors.empty())
        return;

    selectedSensorIndex += direction;
    if (selectedSensorIndex < 0)
    {
        selectedSensorIndex = sensors.size() - 1;
    }
    else if (selectedSensorIndex >= static_cast<int>(sensors.size()))
    {
        selectedSensorIndex = 0;
    }

    updateSensorInfo();
}


void SensorWikiGui::handleSelectButtonClick()
{
    BaseSensor *sensor = getSelectedSensor();
    if (!sensor)
    {
        return;
    }
    sensorManager.setCurrentWikiSensor(sensor);
    switchToMenu();
}

void SensorWikiGui::handleStartButtonClick()
{
    // Stop sensor manager to allow configuration changes
    sensorManager.setRunning(false);
    sensorManager.selectSensorsFromPinMap(); // Update selected sensors from pin assignments
    sensorManager.setCurrentWikiSensor(nullptr); // Clear current wiki sensor

    // Check if any sensors are assigned to pins
    const auto& pinMap = sensorManager.getPinMap();
    size_t count = 0;
    for (const auto& virtualPin : pinMap) {
        if (virtualPin.isAssigned()) count++;
    }

    if (count == 0) {
        splashMessage("No sensors assigned to pins!\n");
        return;
    }

    // Connect all assigned sensors to pins
    lv_label_set_text(ui_StartButtonLabel, "Wait..");
    //Disable button to prevent multiple clicks
    lv_obj_clear_flag(ui_StartButton, LV_OBJ_FLAG_CLICKABLE);

    lv_timer_handler(); // Ensure UI updates
    lv_refr_now(NULL);  // Force immediate screen refresh

    if (!sensorManager.connect()) {
        lv_label_set_text(ui_StartButtonLabel, "START VISUALISATION");
        lv_obj_add_flag(ui_StartButton, LV_OBJ_FLAG_CLICKABLE);
        splashMessage("Error during sensor connection!\n");
        return;
    }
    lv_label_set_text(ui_StartButtonLabel, "START VISUALISATION");
    lv_obj_add_flag(ui_StartButton, LV_OBJ_FLAG_CLICKABLE);
    // Start sensor operations
    // logMessage("Starting sensor operations with %zu sensors assigned\n", count);
    
    // Switch to visualization screen
    switchToVisualization();
}