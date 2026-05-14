#ifndef LIBRARY_EDITOR_GUI_HPP
#define LIBRARY_EDITOR_GUI_HPP

#include "lvgl.h"
#include <map>
#include <string>
#include <vector>

#include "gui_router.hpp"
#include "../managers/device_catalog.hpp"
#include "../managers/device_browser_state.hpp"

class LibraryEditorGui
{
private:
    DeviceCatalog &deviceCatalog;
    DeviceBrowserState &browserState;
    GuiRouter &router;
    bool initialized = false;
    bool editingConfigParam = false;
    int editingParamIndex = -1;

    lv_obj_t *ui_Widget = nullptr;
    lv_obj_t *ui_Title = nullptr;
    lv_obj_t *ui_Form = nullptr;
    lv_obj_t *ui_UidInput = nullptr;
    lv_obj_t *ui_TypeInput = nullptr;
    lv_obj_t *ui_RoleDropdown = nullptr;
    lv_obj_t *ui_AllowedPinsInput = nullptr;
    lv_obj_t *ui_DevicePinsInput = nullptr;
    lv_obj_t *ui_DefaultPinsInput = nullptr;
    lv_obj_t *ui_PicturePreview = nullptr;
    lv_obj_t *ui_PictureImage = nullptr;
    lv_obj_t *ui_PictureGif = nullptr;
    lv_obj_t *ui_PictureFallbackLabel = nullptr;
    lv_obj_t *ui_DescriptionInput = nullptr;
    lv_obj_t *ui_ValuesPanel = nullptr;
    lv_obj_t *ui_ValuesList = nullptr;
    lv_obj_t *ui_ConfigsPanel = nullptr;
    lv_obj_t *ui_ConfigsList = nullptr;
    lv_obj_t *ui_ParamEditorOverlay = nullptr;
    lv_obj_t *ui_ParamEditorPanel = nullptr;
    lv_obj_t *ui_ParamEditorTitle = nullptr;
    lv_obj_t *ui_ParamKeyInput = nullptr;
    lv_obj_t *ui_ParamTypeDropdown = nullptr;
    lv_obj_t *ui_ParamUnitInput = nullptr;
    lv_obj_t *ui_ParamValueInput = nullptr;
    lv_obj_t *ui_ParamMinInput = nullptr;
    lv_obj_t *ui_ParamMaxInput = nullptr;
    lv_obj_t *ui_ParamStepInput = nullptr;
    lv_obj_t *ui_ParamOptionsInput = nullptr;
    lv_obj_t *ui_Keyboard = nullptr;

    std::vector<DeviceParamSchema> valueDraftParams;
    std::vector<DeviceParamSchema> configDraftParams;
    std::string pictureSourcePath;

    void build();
    void refresh();
    void saveDraft();
    void ensureDraftLoaded();
    void buildKeyboard();
    void attachKeyboard(lv_obj_t *textarea);
    void handleKeyboardEvent(lv_event_t *e);
    void showKeyboardFor(lv_obj_t *textarea);
    void hideKeyboard();
    void updatePicturePreview(const std::string &deviceUid, const std::string &storedPicture = "");
    void buildParamListSection(lv_obj_t *panel, const char *title, lv_obj_t **listOut, bool configSection);
    void buildParamEditor();
    void populateParamList(lv_obj_t *list, const std::vector<DeviceParamSchema> &params, bool configSection);
    void showParamEditor(bool configSection, int index);
    void hideParamEditor();
    bool saveEditedParam(std::string &error);
    std::vector<DeviceParamSchema> &currentEditedParamCollection();
    const std::vector<DeviceParamSchema> &currentEditedParamCollection() const;
    bool validateTopLevelDraft(const DeviceDefinitionSchema &draft, std::string &error) const;
    lv_obj_t *createFieldLabel(lv_obj_t *parent, const char *text, lv_coord_t x, lv_coord_t y);
    lv_obj_t *createSingleLineInput(lv_obj_t *parent, lv_coord_t x, lv_coord_t y, lv_coord_t width);

    static bool validateParamSchema(const DeviceParamSchema &schema, std::string &error);
    static uint16_t getRoleDropdownIndex(DeviceRole role);
    static uint16_t getDataTypeDropdownIndex(DeviceDataType dtype);
    static DeviceRole getRoleFromDropdownIndex(uint16_t index);
    static DeviceDataType getDataTypeFromDropdownIndex(uint16_t index);
    static std::vector<std::string> parsePinsCsv(const std::string &csv);
    static std::string formatPinsCsv(const std::vector<std::string> &pins);
    static std::map<std::string, std::string> parseDefaultPinsMap(const std::string &csv);
    static std::string formatDefaultPinsMap(const std::map<std::string, std::string> &pins);

public:
    explicit LibraryEditorGui(DeviceCatalog &deviceCatalog, DeviceBrowserState &browserState, GuiRouter &router);
    ~LibraryEditorGui() = default;

    void init();
    bool isInitialized() const { return initialized; }
    void showEditor();
    void hideEditor();
};

#endif // LIBRARY_EDITOR_GUI_HPP
