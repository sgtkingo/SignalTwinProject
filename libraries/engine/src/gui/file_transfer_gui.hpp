#ifndef FILE_TRANSFER_GUI_HPP
#define FILE_TRANSFER_GUI_HPP

#include "lvgl.h"
#include "gui_router.hpp"
#include "../managers/file_transfer_service.hpp"

class FileTransferGui
{
private:
    GuiRouter &router;
    FileTransferService transferService;
    bool initialized = false;

    lv_obj_t *ui_Widget = nullptr;
    lv_obj_t *ui_Title = nullptr;
    lv_obj_t *ui_SubCaption = nullptr;
    lv_obj_t *ui_CableImage = nullptr;
    lv_obj_t *ui_StateSymbol = nullptr;
    lv_obj_t *ui_StatusLabel = nullptr;
    lv_obj_t *ui_HintLabel = nullptr;
    lv_obj_t *ui_StartButton = nullptr;
    lv_obj_t *ui_StartButtonLabel = nullptr;
    lv_obj_t *ui_StopButton = nullptr;
    lv_obj_t *ui_StopButtonLabel = nullptr;
    lv_obj_t *ui_BackButton = nullptr;

    void build();
    void refresh();
    void renderConnecting();
    void handleStart();
    void handleStop();

public:
    explicit FileTransferGui(GuiRouter &router);
    ~FileTransferGui();

    void init();
    void showFileTransfer();
    void hideFileTransfer();
};

#endif // FILE_TRANSFER_GUI_HPP
