#ifndef GUI_NAVIGATION_POLICY_HPP
#define GUI_NAVIGATION_POLICY_HPP

#include "app_settings.hpp"
#include "gui_state.hpp"

class GuiNavigationPolicy
{
private:
    bool databankReturnToVisualization = false;                           ///< Remembers whether Databank was opened from live visualization
    DefaultCommunicationMode defaultCommunicationMode = DefaultCommunicationMode::ASK; ///< User preference from Settings
    DefaultCommunicationMode sessionCommunicationMode = DefaultCommunicationMode::ASK; ///< Runtime communication mode used by current visualization flow
    ThemeMode themeMode = ThemeMode::LIGHT;                               ///< User theme preference placeholder.
    LanguageMode languageMode = LanguageMode::ENGLISH;                    ///< User language preference placeholder.
    bool selectionBackToMainMenu = false;                                 ///< Whether Selection should back-navigate directly to Main Menu

public:
    /**
     * @brief Resolve the target state for a generic Back action.
     */
    GuiState resolveBackTarget(GuiState fromState) const;
    /**
     * @brief Start the Visualization entry flow, optionally skipping Communication.
     */
    GuiState beginVisualizationFlow();
    /**
     * @brief Finish Communication selection and continue to Selection.
     */
    GuiState finishCommunicationSelection(DefaultCommunicationMode mode);
    GuiState openDatabankFromMainMenu();
    GuiState openDatabankFromVisualization();

    bool shouldSelectionBackGoToMainMenu() const { return selectionBackToMainMenu; }
    DefaultCommunicationMode getDefaultCommunicationMode() const { return defaultCommunicationMode; }
    void setDefaultCommunicationMode(DefaultCommunicationMode mode) { defaultCommunicationMode = mode; }
    DefaultCommunicationMode getSessionCommunicationMode() const { return sessionCommunicationMode; }
    ThemeMode getThemeMode() const { return themeMode; }
    void setThemeMode(ThemeMode mode) { themeMode = mode; }
    LanguageMode getLanguageMode() const { return languageMode; }
    void setLanguageMode(LanguageMode mode) { languageMode = mode; }
};

#endif // GUI_NAVIGATION_POLICY_HPP
