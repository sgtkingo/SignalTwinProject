#ifndef LIBRARY_EDITOR_GUI_HPP
#define LIBRARY_EDITOR_GUI_HPP

#include "lvgl.h"

#include "gui_callbacks.hpp"
#include "../managers/device_browser_state.hpp"

class LibraryEditorGui
{
private:
    DeviceBrowserState &browserState;
    bool initialized = false;

    lv_obj_t *ui_Widget = nullptr;
    lv_obj_t *ui_Title = nullptr;
    lv_obj_t *ui_Text = nullptr;

    void build();
    void refresh();

public:
    explicit LibraryEditorGui(DeviceBrowserState &browserState);
    ~LibraryEditorGui() = default;

    void init();
    bool isInitialized() const { return initialized; }
    void showEditor();
    void hideEditor();
};

#endif // LIBRARY_EDITOR_GUI_HPP
