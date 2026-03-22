#ifndef GUI_ROUTER_HPP
#define GUI_ROUTER_HPP

#include "app_settings.hpp"

class GuiRouter
{
public:
    virtual ~GuiRouter() = default;

    virtual void showConnection() = 0;
    virtual void showMainMenu() = 0;
    virtual void showVisualization() = 0;
    virtual void showSelection() = 0;
    virtual void showLibrary() = 0;
    virtual void showLibraryEditor() = 0;
    virtual void showSettings() = 0;
    virtual void showCreditsScreen() = 0;
    virtual void showAppSelectionScreen() = 0;
    virtual void showCommunicationSelectionScreen() = 0;

    virtual void openVisualizationFlow() = 0;
    virtual void completeCommunicationSelection(DefaultCommunicationMode mode) = 0;
    virtual void openDatabankFromMainMenu() = 0;
    virtual void openDatabankFromVisualization() = 0;
    virtual void navigateBackFromDatabank() = 0;
    virtual void prepareNewLibraryEntity() = 0;

    virtual bool shouldSelectionBackGoToMainMenu() const = 0;
    virtual DefaultCommunicationMode getDefaultCommunicationMode() const = 0;
    virtual void setDefaultCommunicationMode(DefaultCommunicationMode mode) = 0;
};

#endif
