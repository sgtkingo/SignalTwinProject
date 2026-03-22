/**
 * @file device_visualization_session.cpp
 * @brief Visualization session state for selected runtime devices.
 */

#include "device_visualization_session.hpp"

void DeviceVisualizationSession::clear()
{
    selectedDevices.clear();
    resetCurrentIndex();
}

void DeviceVisualizationSession::setDevices(const std::vector<BaseDevice *> &devices)
{
    selectedDevices = devices;
    resetCurrentIndex();
}

BaseDevice *DeviceVisualizationSession::getSelectedDeviceAt(size_t index) const
{
    if (selectedDevices.empty() || index >= selectedDevices.size()) {
        return nullptr;
    }

    return selectedDevices[index];
}

BaseDevice *DeviceVisualizationSession::stepCurrentDevice(int direction)
{
    if (selectedDevices.empty()) {
        return nullptr;
    }

    if (direction > 0) {
        currentIndex = (currentIndex + 1) % selectedDevices.size();
    } else if (direction < 0) {
        currentIndex = (currentIndex == 0) ? selectedDevices.size() - 1 : currentIndex - 1;
    }

    return getSelectedDeviceAt(currentIndex);
}

BaseDevice *DeviceVisualizationSession::getCurrentDevice() const
{
    return getSelectedDeviceAt(currentIndex);
}

BaseDevice *DeviceVisualizationSession::nextDevice()
{
    return stepCurrentDevice(1);
}

BaseDevice *DeviceVisualizationSession::previousDevice()
{
    return stepCurrentDevice(-1);
}
