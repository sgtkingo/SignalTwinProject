#ifndef FILE_TRANSFER_SERVICE_HPP
#define FILE_TRANSFER_SERVICE_HPP

#include "../config.hpp"
#include <string>

enum class FileTransferState
{
    IDLE,
    CONNECTING,
    READY,
    MISSING_SD,
    UNSUPPORTED,
    ERROR
};

class FileTransferService
{
private:
    FileTransferState state = FileTransferState::IDLE;
    std::string lastMessage = "Transfer mode is idle.";
    bool transferModeActive = false;

public:
    bool isSdCardPresent() const;
    FileTransferState getState() const { return state; }
    const std::string &getLastMessage() const { return lastMessage; }
    bool isTransferModeActive() const { return transferModeActive; }

    bool start();
    bool stop();
};

#endif // FILE_TRANSFER_SERVICE_HPP
