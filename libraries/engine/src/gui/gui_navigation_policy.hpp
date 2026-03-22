#ifndef GUI_NAVIGATION_POLICY_HPP
#define GUI_NAVIGATION_POLICY_HPP

#include "app_settings.hpp"
#include "gui_state.hpp"

class GuiNavigationPolicy
{
private:
    bool databankReturnToVisualization = false;
    DefaultCommunicationMode defaultCommunicationMode = DefaultCommunicationMode::ASK;
    DefaultCommunicationMode sessionCommunicationMode = DefaultCommunicationMode::ASK;
    bool selectionBackToMainMenu = false;

public:
    GuiState resolveBackTarget(GuiState fromState) const;
    GuiState beginVisualizationFlow();
    GuiState finishCommunicationSelection(DefaultCommunicationMode mode);
    GuiState openDatabankFromMainMenu();
    GuiState openDatabankFromVisualization();

    bool shouldSelectionBackGoToMainMenu() const { return selectionBackToMainMenu; }
    DefaultCommunicationMode getDefaultCommunicationMode() const { return defaultCommunicationMode; }
    void setDefaultCommunicationMode(DefaultCommunicationMode mode) { defaultCommunicationMode = mode; }
    DefaultCommunicationMode getSessionCommunicationMode() const { return sessionCommunicationMode; }
};

#endif // GUI_NAVIGATION_POLICY_HPP
