/**
 * @file storage_manager.hpp
 * @brief Storage backend abstraction for persistent project data.
 */

#ifndef STORAGE_MANAGER_HPP
#define STORAGE_MANAGER_HPP

#include "../config.hpp"

#include <FS.h>
#include <string>
#include <vector>

#if STORAGE_OPTION == STORAGE_OPTION_SD
#include <SD.h>
#include <SPI.h>
#elif STORAGE_OPTION == STORAGE_OPTION_SPIFFS
#include <SPIFFS.h>
#else
#error Unsupported STORAGE_OPTION value.
#endif

class StorageManager {
private:
    fs::FS *activeFilesystem = nullptr;
    const char *activeStorageLabel = "Storage";
    bool initialized = false;
    bool available = false;
    bool transferLocked = false;

    StorageManager() = default;

public:
    StorageManager(const StorageManager &) = delete;
    StorageManager &operator=(const StorageManager &) = delete;

    static StorageManager &instance();

    bool init();

    bool isInitialized() const { return initialized; }
    bool isAvailable() const { return available; }
    const char *getStorageLabel() const { return activeStorageLabel; }
    bool isTransferLocked() const { return transferLocked; }

    bool exists(const std::string &path) const;
    bool ensureDirectory(const std::string &path);
    bool remove(const std::string &path);
    File open(const std::string &path, const char *mode = FILE_READ) const;
    std::vector<std::string> listFiles(const std::string &directoryPath) const;

    uint64_t totalBytes() const;
    uint64_t usedBytes() const;

    bool enterTransferLock();
    bool exitTransferLock();
};

StorageManager &storageManager();

#endif // STORAGE_MANAGER_HPP
