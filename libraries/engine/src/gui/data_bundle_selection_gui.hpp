/**
 * @file data_bundle_selection_gui.hpp
 * @brief Header file for the DataBundleSelectionGui class
 *
 * This header file declares the DataBundleSelectionGui class responsible for
 * data bundles made from record on visualisation
 *
 * @copyright 2025 MTA
 * @author Ondřej Wrubel
 */

#ifndef DATA_BUNDLE_SELECTION_GUI_HPP
#define DATA_BUNDLE_SELECTION_GUI_HPP

#include "lvgl.h"
#include <array>
#include <map>

#include "gui_callbacks.hpp"
#include "../managers/manager.hpp"
#include "../managers/data_bundle_manager.hpp"
#include "../exceptions/data_exceptions.hpp"

/**
 * @class DataBundleSelectionGui
 * @brief Handles data bundle selection, display, and navigation.
 *
 * This class is responsible for:
 * - Displaying data bundles with charts and information
 * - Handling navigation between data bundles
 * - Managing data bundle-specific events and interactions
 */
class DataBundleSelectionGui
{
private:
    DataBundleManager &dataBundleManager;///< Reference to the databundle manager instance

    bool initialized = false; ///< Initialization state flag

    unsigned char currentPage = 0;

    lv_obj_t *ui_DataBundlesWidget;                         ///< DataBundlesWidget
    lv_obj_t *ui_DataBundlePageWatcher;                     ///< Page watcher bar
    lv_obj_t *ui_DataBundlePageWatcherCell[5];              ///< Page indicator cells (0–4)
    lv_obj_t *ui_DataBundle[6];                             ///< Single data bundle container [6]
    lv_obj_t *ui_DataBundleHeaderGroup[6];                  ///< Header group container [6]
    lv_obj_t *ui_DataBundleHeaderCornerBottomLeft[6];       ///< Header corner bottom-left [6]
    lv_obj_t *ui_DataBundleHeaderCornerBottomRight[6];      ///< Header corner bottom-right [6]
    lv_obj_t *ui_DataBundleHeader[6];                       ///< Header background [6]
    lv_obj_t *ui_DataBundleHeaderLabel[6];                  ///< Header label (title) [6]
    lv_obj_t *ui_DataBundleChart[6];                        ///< Chart widget for bundle [6]
    lv_chart_series_t *ui_DataBundleChart_series_1[6];      ///< Chart series for bundle [6]
    lv_obj_t *ui_DataBundleFooterGroup[6];                  ///< Footer group container [6]
    lv_obj_t *ui_DataBundleFooterBridge[6];                 ///< Footer bridge decorative element [6]
    lv_obj_t *ui_DataBundleFooterBridgeFill[6];             ///< Footer bridge fill [6]
    lv_obj_t *ui_DataBundleFooterTimerGroup[6];             ///< Timer group container [6]
    lv_obj_t *ui_DataBundleFooterDateCornerTopLeft[6];      ///< Date corner top-left [6]
    lv_obj_t *ui_DataBundleFooterDateCornerTopRight[6];     ///< Date corner top-right [6]
    lv_obj_t *ui_DataBundleFooterDate[6];                   ///< Date background [6]
    lv_obj_t *ui_DataBundleFooterLabelDate[6];              ///< Date label [6]
    lv_obj_t *ui_DataBundleFooterLabelTime[6];              ///< Time label [6]
    lv_obj_t *ui_DataBundleFooterButtonsGroup[6];           ///< Footer buttons group [6]
    lv_obj_t *ui_DataBundleFooterButtonsCornerTopLeft[6];   ///< Buttons corner top-left [6]
    lv_obj_t *ui_DataBundleFooterButtonsCornerTopRight[6];  ///< Buttons corner top-right [6]
    lv_obj_t *ui_DataBundleFooterButtons[6];                ///< Buttons background [6]
    lv_obj_t *ui_DataBundleFooterButtonExport[6];           ///< Export button [6]
    lv_obj_t *ui_DataBundleFooterButtonExportImage[6];      ///< Export button image [6]
    lv_obj_t *ui_DataBundleFooterButtonClear[6];            ///< Clear button [6]
    lv_obj_t *ui_DataBundleFooterButtonClearImage[6];       ///< Clear button image [6]
    lv_obj_t *ui_ShadowOverlay;                             ///< Shadow overlay for popups
    lv_obj_t *ui_LogoGroup;                                 ///< Logo group container
    lv_obj_t *ui_LogoCornerBottomLeft;                      ///< Logo corner bottom-left
    lv_obj_t *ui_LogoCornerFillBottomLeft;                  ///< Logo corner fill bottom-left
    lv_obj_t *ui_LogoCornerBottomRight;                     ///< Logo corner bottom-right
    lv_obj_t *ui_LogoCornerFillBottomRight;                 ///< Logo corner fill bottom-right
    lv_obj_t *ui_LogoOutlay;                                ///< Logo outlay
    lv_obj_t *ui_LogoImage;                                 ///< Logo image widget

    /**
     * @brief Add navigation buttons to a widget
     * @param parentWidget The parent widget to add buttons to
     */
    void addNavButtonsToWidget(lv_obj_t *parentWidget);

    /**
     * @brief Add control buttons (sync, back) to a widget
     * @param parentWidget The parent widget to add the buttons to
     */
    void addControlButtonsToWidget(lv_obj_t *parentWidget);

    /**
     * @brief Add logo panel to a widget
     * @param parentWidget The parent widget to add the logo panel to
     */
    void addLogoPanelToWidget(lv_obj_t *parentWidget);

    /**
     * @brief Show shadow overlay
     * @param popup The popup dialog which is meant to be highlighted
     */
    void showShadowOverlay();

    /**
     * @brief Hide shadow overlay
     */
    void hideShadowOverlay();

    /**
     * @brief update data bundles currently shown
     */
    void updateBundles();

    /**
     * @brief update watcher cells to indicate which page is currently active
     */
    void updateWatcherCells();

    // not needed
    /**
     * @brief update nav buttons based on available pages 
     */
    void updateNavButtons();

public:
    /**
     * @brief Constructor
     */
    DataBundleSelectionGui(DataBundleManager &dataBundleManager);

    /**
     * @brief Destructor
     */
    ~DataBundleSelectionGui() = default;

    /**
     * @brief Initialize the data bundle selection GUI
     */
    void init();

    /**
     * @brief construct the data bundle selection GUI
     */
    void constructDataBundleSelection();

    /**
     * @brief Create a single data bundle container
     * @param i The index of the data bundle to create
     * @param dataBundleName Name of the data bundle
     * @param time The time it was created
     * @param date The date it was created
     * @param values The values shown in the visual chart
     */
    void createDataBundle(unsigned char i, const char *dataBundleName, const char *time, const char *date, std::array<std::string,10> values);

    /**
     * @brief Go to the previous page in the list
     */
    void goToPreviousPage();

    /**
     * @brief Go to the next page in the list
     */
    void goToNextPage();

    /**
     * @brief opens a confirmation dialog to clear the current data bundle
     * @param index the bundle that will be cleared
     */
    void handleClearButtonClick(unsigned char index);

    /**
     * @brief clears the current data bundle upon confirmation
     * @param index the bundle that will be cleared
     */
    void handleClearConfirmButtonClick(unsigned char index);

    /**
     * @brief Show the data bundle selection screen
     */
    void showDataBundles();

    /**
     * @brief Hide the data bundle selection screen
     */
    void hideDataBundles();

    /**
     * @brief Hides specific Data Bundle
     * Made so that it hides the bundle that exceeds current number of shown data bundles
     */
    void hideSpecificDataBundle(unsigned char index);
};

#endif // DATA_BUNDLES_GUI_HPP