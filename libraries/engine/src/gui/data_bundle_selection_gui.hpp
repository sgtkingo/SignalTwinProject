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
#include <string>
#include <vector>

#include "gui_router.hpp"
#include "../managers/device_manager.hpp"
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
    static const int BUNDLE_VIEW_CHART_POINTS = 50;

    struct BundleCsvRow
    {
        std::vector<std::string> cells;
        std::string signalName;
        std::string value;
        bool numeric = false;
        bool scaled = false;
        lv_coord_t chartValue = 0;
    };

    GuiRouter &router;
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
    lv_obj_t *ui_BundleViewerOverlay;                       ///< Bundle CSV viewer overlay
    lv_obj_t *ui_BundleViewerPanel;                         ///< Bundle CSV viewer panel
    lv_obj_t *ui_BundleViewerGraphTab = nullptr;            ///< Graph viewer tab button
    lv_obj_t *ui_BundleViewerCsvTab = nullptr;              ///< CSV viewer tab button
    lv_obj_t *ui_BundleViewerSettingsButton = nullptr;      ///< Local viewer settings button
    lv_obj_t *ui_BundleViewerChart = nullptr;               ///< Bundle graph chart
    lv_obj_t *ui_BundleViewerTable = nullptr;               ///< Bundle CSV table
    lv_obj_t *ui_BundleViewerScalingLabel = nullptr;        ///< Bundle graph scaling label
    lv_obj_t *ui_BundleViewerCursorLabel = nullptr;         ///< Bundle graph cursor label
    lv_obj_t *ui_BundleViewerPrimaryLegend = nullptr;       ///< Primary signal legend
    lv_obj_t *ui_BundleViewerSecondaryLegend = nullptr;     ///< Secondary signal legend
    lv_obj_t *ui_BundleViewerPrimaryLegendLabel = nullptr;  ///< Primary signal legend text
    lv_obj_t *ui_BundleViewerSecondaryLegendLabel = nullptr;///< Secondary signal legend text
    lv_obj_t *ui_BundleViewerCursorXLine = nullptr;         ///< Cursor horizontal line
    lv_obj_t *ui_BundleViewerCursorYLine = nullptr;         ///< Cursor vertical line
    lv_obj_t *ui_BundleViewerSettingsOverlay = nullptr;     ///< Local settings overlay
    lv_obj_t *ui_BundleViewerSettingsPanel = nullptr;       ///< Local settings panel
    lv_obj_t *ui_BundleViewerPrimarySwatch = nullptr;       ///< Primary color swatch
    lv_obj_t *ui_BundleViewerSecondarySwatch = nullptr;     ///< Secondary color swatch
    lv_chart_series_t *ui_BundleViewerPrimarySeries = nullptr;   ///< Primary graph series
    lv_chart_series_t *ui_BundleViewerSecondarySeries = nullptr; ///< Secondary graph series
    lv_obj_t *ui_DeleteAllButtonGroup;                      ///< Delete all bundles button group
    lv_obj_t *ui_LogoGroup;                                 ///< Logo group container
    lv_obj_t *ui_LogoCornerBottomLeft;                      ///< Logo corner bottom-left
    lv_obj_t *ui_LogoCornerFillBottomLeft;                  ///< Logo corner fill bottom-left
    lv_obj_t *ui_LogoCornerBottomRight;                     ///< Logo corner bottom-right
    lv_obj_t *ui_LogoCornerFillBottomRight;                 ///< Logo corner fill bottom-right
    lv_obj_t *ui_LogoOutlay;                                ///< Logo outlay
    lv_obj_t *ui_LogoImage;                                 ///< Logo image widget
    std::vector<std::string> bundleViewerHeaders;           ///< CSV headers for active viewer
    std::vector<BundleCsvRow> bundleViewerRows;             ///< CSV rows for active viewer
    std::vector<std::string> bundleViewerSignals;           ///< Numeric signals available in active viewer
    bool bundleViewerCsvMode = false;                       ///< True when CSV table tab is visible
    int bundleViewerHistoryOffset = 0;                      ///< Graph history pan offset
    int bundleViewerDragAccumulatorPx = 0;                  ///< Touch drag accumulator
    int bundleViewerCursorIndex = 0;                        ///< Cursor point in the visible graph window
    bool bundleViewerCursorVisible = false;                 ///< True while touch cursor is active
    lv_coord_t bundleViewerRangeMin = -1;                   ///< Active graph range minimum
    lv_coord_t bundleViewerRangeMax = 1;                    ///< Active graph range maximum
    lv_point_t bundleViewerCursorXPoints[2];                ///< Horizontal cursor line points
    lv_point_t bundleViewerCursorYPoints[2];                ///< Vertical cursor line points
    uint8_t bundleViewerPrimaryColorIndex = 0;              ///< Primary line color palette index
    uint8_t bundleViewerSecondaryColorIndex = 1;            ///< Secondary line color palette index

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
     * @brief Bind a bundle preview object to the CSV viewer
     * @param object Object that should open the viewer when clicked
     * @param index Bundle slot on the current page
     */
    void bindBundleOpenEvent(lv_obj_t *object, unsigned char index);

    /**
     * @brief Open the CSV viewer for a bundle slot on the current page
     * @param index Bundle slot on the current page
     */
    void showBundleViewer(unsigned char index);

    /**
     * @brief Close the active CSV viewer if it exists
     */
    void closeBundleViewer();

    /**
     * @brief Parse CSV text and refresh internal viewer data buffers
     * @param csvText CSV text loaded from storage
     */
    void parseBundleViewerCsv(const std::string &csvText);

    /**
     * @brief Create and render the graph/table content area of the bundle viewer
     */
    void createBundleViewerContent();

    /**
     * @brief Switch between graph and CSV table presentation
     */
    void setBundleViewerMode(bool csvMode);

    /**
     * @brief Update graph tab from parsed bundle rows
     */
    void updateBundleViewerGraph();

    /**
     * @brief Update CSV table tab from parsed bundle rows
     */
    void updateBundleViewerTable();

    /**
     * @brief Pan graph history by touch/step offset
     */
    void panBundleViewerHistory(int steps);

    /**
     * @brief Handle touch events on the bundle graph
     */
    void handleBundleViewerChartDrag(lv_event_t *e);

    /**
     * @brief Move graph cursor by one visible sample
     */
    void moveBundleViewerCursor(int steps);

    /**
     * @brief Show cursor at a visible graph point.
     */
    void showBundleViewerCursorAtIndex(int index);

    /**
     * @brief Hide the touch cursor.
     */
    void hideBundleViewerCursor();

    /**
     * @brief Show local graph settings
     */
    void showBundleViewerSettings();

    /**
     * @brief Hide local graph settings
     */
    void hideBundleViewerSettings();

    /**
     * @brief Cycle graph line color
     */
    void cycleBundleViewerSeriesColor(bool primary);

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
    DataBundleSelectionGui(GuiRouter &router, DataBundleManager &dataBundleManager);

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
     * @brief opens a confirmation dialog to clear all data bundles
     */
    void handleDeleteAllButtonClick();

    /**
     * @brief clears all data bundles upon confirmation
     */
    void handleDeleteAllConfirmButtonClick();

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
