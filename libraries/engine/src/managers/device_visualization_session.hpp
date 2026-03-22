/**
 * @file device_visualization_session.hpp
 * @brief Runtime visualization session for connected devices.
 */

#ifndef DEVICE_VISUALIZATION_SESSION_HPP
#define DEVICE_VISUALIZATION_SESSION_HPP

#include <cstddef>
#include <vector>

#include "../devices/base_device.hpp"

class DeviceVisualizationSession
{
private:
    std::vector<BaseDevice *> selectedDevices; ///< Runtime devices included in the active visualization flow
    size_t currentIndex = 0;                   ///< Currently focused device inside the session

    BaseDevice *getSelectedDeviceAt(size_t index) const;
    BaseDevice *stepCurrentDevice(int direction);

public:
    /**
     * @brief Reset the active visualization session.
     */
    void clear();

    /**
     * @brief Replace the active runtime device list used by visualization.
     */
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
