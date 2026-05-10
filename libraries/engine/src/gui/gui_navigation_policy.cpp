#include "gui_navigation_policy.hpp"

GuiState GuiNavigationPolicy::resolveBackTarget(GuiState fromState) const
{
    switch (fromState) {
    case GuiState::DATA_BUNDLE_SELECTION:
        return databankReturnToVisualization ? GuiState::VISUALIZATION : GuiState::MAIN_MENU;
    case GuiState::VISUALIZATION:
        return GuiState::SELECTION;
    case GuiState::CONNECTION:
        return GuiState::SELECTION;
    case GuiState::SELECTION:
        return selectionBackToMainMenu ? GuiState::MAIN_MENU : GuiState::COMMUNICATION_SELECTION;
    case GuiState::COMMUNICATION_SELECTION:
    case GuiState::LIBRARY:
    case GuiState::SETTINGS:
        return GuiState::MAIN_MENU;
    case GuiState::LIBRARY_EDITOR:
        return GuiState::LIBRARY;
    case GuiState::CREDITS:
        return GuiState::VISUALIZATION;
    default:
        return GuiState::MAIN_MENU;
    }
}

GuiState GuiNavigationPolicy::beginVisualizationFlow()
{
    selectionBackToMainMenu = false;
    return GuiState::COMMUNICATION_SELECTION;
}

GuiState GuiNavigationPolicy::finishCommunicationSelection(DefaultCommunicationMode mode)
{
    sessionCommunicationMode = mode;
    selectionBackToMainMenu = false;
    return GuiState::SELECTION;
}

GuiState GuiNavigationPolicy::openDatabankFromMainMenu()
{
    databankReturnToVisualization = false;
    return GuiState::DATA_BUNDLE_SELECTION;
}

GuiState GuiNavigationPolicy::openDatabankFromVisualization()
{
    databankReturnToVisualization = true;
    return GuiState::DATA_BUNDLE_SELECTION;
}
