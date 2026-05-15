#ifndef FILE_TRANSFER_USB_MSC_BRIDGE_HPP
#define FILE_TRANSFER_USB_MSC_BRIDGE_HPP

#include <string>

class FileTransferUsbMscBridge
{
private:
    bool active = false;

public:
    static FileTransferUsbMscBridge &instance();

    bool isSupported() const;
    bool isActive() const { return active; }
    bool start(std::string &error);
    bool stop(std::string &error);
};

FileTransferUsbMscBridge &fileTransferUsbMscBridge();

#endif // FILE_TRANSFER_USB_MSC_BRIDGE_HPP
