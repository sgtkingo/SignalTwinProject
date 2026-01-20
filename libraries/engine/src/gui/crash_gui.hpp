/**
 * @file crash_gui.hpp
 * @brief Declaration of the CrashGui class for displaying system crash information
 *
 * This header defines the CrashGui class which displays a crash screen with
 * error information and provides a restart option when the system encounters
 * a critical error.
 *
 * @copyright 2025 MTA
 * @author Ing. Jiri Konecny, Ondřej Wrubel
 */

#ifndef CRASH_GUI_HPP
#define CRASH_GUI_HPP

#include <lvgl.h>
#include <string>

/**
 * @brief Crash GUI class
 * 
 * This class handles the display of system crash information and provides
 * a restart mechanism when critical errors occur.
 */
class CrashGui {
private:
    bool initialized;                ///< Initialization flag
    
    // LVGL UI elements
    lv_obj_t *ui_CrashWidget;        ///< Main crash screen container
    lv_obj_t *ui_CrashTitle;         ///< "CRASH!" title label
    lv_obj_t *ui_ReasonLabel;        ///< Reason text label
    lv_obj_t *ui_RestartButton;      ///< Restart button
    lv_obj_t *ui_RestartButtonLabel; ///< Restart button label
    
    /**
     * @brief Build the crash screen UI
     */
    void buildCrashGui();
    
    /**
     * @brief Handle restart button click
     */
    void handleRestartButtonClick();
    
public:
    /**
     * @brief Constructor
     */
    CrashGui();
    
    /**
     * @brief Destructor
     */
    ~CrashGui();
    
    /**
     * @brief Initialize the crash GUI
     */
    void init();
    
    /**
     * @brief Show crash screen with specific reason
     * @param reason The crash reason text to display
     */
    void showCrash(const std::string& reason);
    
    /**
     * @brief Hide crash screen
     */
    void hideCrash();
    
    /**
     * @brief Check if crash GUI is initialized
     * @return true if initialized, false otherwise
     */
    bool isInitialized() const { return initialized; }
};

#endif // CRASH_GUI_HPP