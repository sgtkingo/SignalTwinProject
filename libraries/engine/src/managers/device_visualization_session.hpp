/**
 * @file device_visualization_session.hpp
 * @brief Visualization session state for selected runtime devices.
 */

#ifndef DEVICE_VISUALIZATION_SESSION_HPP
#define DEVICE_VISUALIZATION_SESSION_HPP

#include <cstddef>
#include <vector>

#include "../devices/base_device.hpp"

class DeviceVisualizationSession
{
private:
    std::vector<BaseDevice *> selectedDevices;
    size_t currentIndex = 0;

    BaseDevice *getSelectedDeviceAt(size_t index) const;
    BaseDevice *stepCurrentDevice(int direction);

public:
    void clear();
    void setDevices(const std::vector<BaseDevice *> &devices);

    bool hasDevices() const { return !selectedDevices.empty(); }
    const std::vector<BaseDevice *> &getDevices() const { return selectedDevices; }

    BaseDevice *getCurrentDevice() const;
    BaseDevice *nextDevice();
    BaseDevice *previousDevice();

    size_t getCurrentIndex() const { return currentIndex; }
    void resetCurrentIndex() { currentIndex = 0; }
};

#endif // DEVICE_VISUALIZATION_SESSION_HPP
