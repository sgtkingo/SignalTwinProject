/**
 * @file communication_selection.hpp
 * @brief Declaration of the CommunicationSelectionGui class for managing the communication selection GUI.
 * This class handles the initialization, construction, and hiding of the communication selection interface.
 * 
 * @copyright 2025 MTA
 * @author Ondřej Wrubel
 */

#ifndef COMMUNICATION_SELECTION_GUI_HPP
#define COMMUNICATION_SELECTION_GUI_HPP

#include "lvgl.h"

#include "gui_callbacks.hpp"
#include "../managers/manager.hpp"
#include "../exceptions/data_exceptions.hpp"

/**
 * @class CommunicationSelectionGui
 * @brief Handles communication selection GUI.
 *
 * This class is responsible for:
 * - Displaying communication options (Cable, Bluetooth)
 * - Handling user interactions for communication selection
 */
class CommunicationSelectionGui
{
private:
    bool initialized = false; ///< Initialization state flag

    lv_obj_t *ui_Widget;
    lv_obj_t *ui_ConnectionLabel;
    lv_obj_t *ui_CableButton;
    lv_obj_t *ui_CableImage;
    lv_obj_t *ui_BluetoothButton;
    lv_obj_t *ui_BluetoothImage;

public:
    /**
     * @brief Constructor
     */
    CommunicationSelectionGui();

    /**
     * @brief Destructor
     */
    ~CommunicationSelectionGui() = default;
    /**
     * @brief Initialize the communication selection GUI
     */
    void init(void);
    /**
     * @brief Construct the communication selection GUI
     */
    void constructCommunicationSelection(void);
    /**
     * @brief Hide the communication selection GUI
     */
    void hideCommunicationSelection(void);
};
#endif // COMMUNICATION_SELECTION_GUI_HPP
