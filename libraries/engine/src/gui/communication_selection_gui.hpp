#ifndef COMMUNICATION_SELECTION_GUI_HPP
#define COMMUNICATION_SELECTION_GUI_HPP

#include "lvgl.h"

#include "gui_router.hpp"
#include "../managers/device_manager.hpp"

class CommunicationSelectionGui
{
private:
    GuiRouter &router;
    DeviceManager &deviceManager;
    bool initialized = false;

    lv_obj_t *ui_Widget = nullptr;

    void createOptionButton(const char *text, lv_coord_t x, lv_coord_t y, DefaultCommunicationMode mode, bool supported = true);
    void handleModeSelection(DefaultCommunicationMode mode);

public:
    explicit CommunicationSelectionGui(GuiRouter &router, DeviceManager &deviceManager);
    ~CommunicationSelectionGui() = default;

    void init(void);
    void constructCommunicationSelection(void);
    void hideCommunicationSelection(void);
};

#endif // COMMUNICATION_SELECTION_GUI_HPP
