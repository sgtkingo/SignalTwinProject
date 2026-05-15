#include "file_transfer_service.hpp"

#include "storage_manager.hpp"
#include "expt.hpp"

#if defined(ARDUINO_H)
#include <SD.h>
#include <SPI.h>
#endif

bool FileTransferService::isSdCardPresent() const
{
#if !defined(ARDUINO_H)
    return false;
#elif STORAGE_OPTION == STORAGE_OPTION_SD
    if (storageManager().isAvailable() || storageManager().isTransferLocked())
    {
        return true;
    }

    SPI.begin(STORAGE_SD_PIN_SCK, STORAGE_SD_PIN_MISO, STORAGE_SD_PIN_MOSI, STORAGE_SD_PIN_CS);
    const bool mounted = SD.begin(STORAGE_SD_PIN_CS);
    if (mounted)
    {
        SD.end();
    }
    return mounted;
#else
    SPI.begin(STORAGE_SD_PIN_SCK, STORAGE_SD_PIN_MISO, STORAGE_SD_PIN_MOSI, STORAGE_SD_PIN_CS);
    const bool mounted = SD.begin(STORAGE_SD_PIN_CS);
    if (mounted)
    {
        SD.end();
    }
    return mounted;
#endif
}

bool FileTransferService::start()
{
    try
    {
        if (!isSdCardPresent())
        {
            state = FileTransferState::MISSING_SD;
            lastMessage = "SD card is not detected.";
            debugLogMessage(DEBUG_VERBOSE_ERRORS, "FileTransferService::start", "transfer sd missing", "%s", lastMessage.c_str());
            return false;
        }

        state = FileTransferState::CONNECTING;
        lastMessage = "Connecting...";
        transferModeActive = true;
        setLoggerUsbCdcAvailable(false);

#if STORAGE_OPTION == STORAGE_OPTION_SD
        if (!storageManager().enterTransferLock())
        {
            setLoggerUsbCdcAvailable(true);
            flushBufferedLogMessages();
            transferModeActive = false;
            state = FileTransferState::ERROR;
            lastMessage = "Failed to lock SD card for transfer.";
            debugLogMessage(DEBUG_VERBOSE_ERRORS, "FileTransferService::start", "transfer lock failed", "%s", lastMessage.c_str());
            return false;
        }
#endif

#if defined(FILE_TRANSFER_USB_MSC_ENABLED) && FILE_TRANSFER_USB_MSC_ENABLED
        state = FileTransferState::READY;
        lastMessage = "Transfer ready";
        debugLogMessage(DEBUG_VERBOSE_IMPORTANT, "FileTransferService::start", "transfer ready", "%s", lastMessage.c_str());
        return true;
#else
        state = FileTransferState::ERROR;
        lastMessage = "USB MSC bridge backend is not enabled in this build.";
        debugLogMessage(DEBUG_VERBOSE_ERRORS, "FileTransferService::start", "transfer backend unavailable", "%s", lastMessage.c_str());
#if STORAGE_OPTION == STORAGE_OPTION_SD
        storageManager().exitTransferLock();
#endif
        setLoggerUsbCdcAvailable(true);
        flushBufferedLogMessages();
        transferModeActive = false;
        return false;
#endif
    }
    catch (const Exception &ex)
    {
        ex.print();
        state = FileTransferState::ERROR;
        lastMessage = ex.flush(0);
#if STORAGE_OPTION == STORAGE_OPTION_SD
        storageManager().exitTransferLock();
#endif
        setLoggerUsbCdcAvailable(true);
        flushBufferedLogMessages();
        transferModeActive = false;
        return false;
    }
    catch (const std::exception &ex)
    {
        Exception("FileTransferService::start", ex.what()).print();
        state = FileTransferState::ERROR;
        lastMessage = ex.what();
#if STORAGE_OPTION == STORAGE_OPTION_SD
        storageManager().exitTransferLock();
#endif
        setLoggerUsbCdcAvailable(true);
        flushBufferedLogMessages();
        transferModeActive = false;
        return false;
    }
    catch (...)
    {
        Exception("FileTransferService::start", "Unknown exception").print();
        state = FileTransferState::ERROR;
        lastMessage = "Unknown exception while starting transfer.";
#if STORAGE_OPTION == STORAGE_OPTION_SD
        storageManager().exitTransferLock();
#endif
        setLoggerUsbCdcAvailable(true);
        flushBufferedLogMessages();
        transferModeActive = false;
        return false;
    }
}

bool FileTransferService::stop()
{
    try
    {
        if (!transferModeActive)
        {
            setLoggerUsbCdcAvailable(true);
            state = FileTransferState::IDLE;
            lastMessage = "Transfer mode is idle.";
            return true;
        }

#if STORAGE_OPTION == STORAGE_OPTION_SD
        if (!storageManager().exitTransferLock())
        {
            setLoggerUsbCdcAvailable(true);
            flushBufferedLogMessages();
            transferModeActive = false;
            state = FileTransferState::ERROR;
            lastMessage = "Failed to remount SD card after transfer.";
            debugLogMessage(DEBUG_VERBOSE_ERRORS, "FileTransferService::stop", "transfer unlock failed", "%s", lastMessage.c_str());
            return false;
        }
#endif

        setLoggerUsbCdcAvailable(true);
        flushBufferedLogMessages();
        transferModeActive = false;
        state = FileTransferState::IDLE;
        lastMessage = "Transfer mode stopped. SD card is mounted back in HMI.";
        debugLogMessage(DEBUG_VERBOSE_IMPORTANT, "FileTransferService::stop", "transfer unlock", "%s", lastMessage.c_str());
        return true;
    }
    catch (const Exception &ex)
    {
        ex.print();
        setLoggerUsbCdcAvailable(true);
        flushBufferedLogMessages();
        transferModeActive = false;
        state = FileTransferState::ERROR;
        lastMessage = ex.flush(0);
        return false;
    }
    catch (const std::exception &ex)
    {
        Exception("FileTransferService::stop", ex.what()).print();
        setLoggerUsbCdcAvailable(true);
        flushBufferedLogMessages();
        transferModeActive = false;
        state = FileTransferState::ERROR;
        lastMessage = ex.what();
        return false;
    }
    catch (...)
    {
        Exception("FileTransferService::stop", "Unknown exception").print();
        setLoggerUsbCdcAvailable(true);
        flushBufferedLogMessages();
        transferModeActive = false;
        state = FileTransferState::ERROR;
        lastMessage = "Unknown exception while stopping transfer.";
        return false;
    }
}
