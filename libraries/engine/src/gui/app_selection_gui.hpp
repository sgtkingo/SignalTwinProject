/**
 * @file app_selection.hpp
 * @brief Header file for the AppSelectionGui class
 * This header file declares the AppSelectionGui class responsible
 * 
 * @copyright 2025 MTA
 * @author Ondřej Wrubel
 */

#ifndef APP_SELECTION_GUI_HPP
#define APP_SELECTION_GUI_HPP

#include "lvgl.h"

#include "gui_callbacks.hpp"
#include "../managers/manager.hpp"
#include "../exceptions/data_exceptions.hpp"

/**
 * @class AppSelectionGui
 * @brief Handles application selection.
 *
 * This class is responsible for:
 * - Displaying available applications with icons and information
 * - Handling navigation between applications
 */
class AppSelectionGui
{
private:
    bool initialized = false; ///< Initialization state flag

    lv_obj_t * ui_Widget;
    lv_obj_t * ui_LogoGroup;
    lv_obj_t * ui_LogoCornerBottomLeft;
    lv_obj_t * ui_LogoCornerFillBottomLeft;
    lv_obj_t * ui_LogoCornerBottomRight;
    lv_obj_t * ui_LogoCornerFillBottomRight;
    lv_obj_t * ui_LogoOutlay;
    lv_obj_t * ui_LogoImage;
    lv_obj_t * ui_EduPanelLabel;
    lv_obj_t * ui_AppGroup;
    lv_obj_t * ui_AppImage;
    lv_obj_t * ui_AppLabel;
    lv_obj_t * ui_ComingSoonGroup;
    lv_obj_t * ui_ComingSoonImage;
    lv_obj_t * ui_ComingSoonLabel;

public:
    /**
     * @brief Constructor
     */
    AppSelectionGui();
    /**
     * @brief Destructor
     */
    ~AppSelectionGui() = default;
    /**
     * @brief Init the application selection screen
     */
    void init(void);
    /**
     * @brief Constructs the application selection screen
     */
    void constructAppSelection(void);
    /**
     * @brief Hide the application selection screen
     */
    void hideAppSelection(void);

};

#endif // APP_SELECTION_GUI_HPP

