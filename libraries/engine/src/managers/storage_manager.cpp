/**
 * @file storage_manager.cpp
 * @brief Storage backend abstraction for persistent project data.
 */

#include "storage_manager.hpp"

#include "expt.hpp"

#include <algorithm>

namespace
{
std::string normalizeDirectoryPath(std::string path)
{
    if (path.empty()) {
        return "/";
    }

    if (path.front() != '/') {
        path.insert(path.begin(), '/');
    }

    if (path.size() > 1 && path.back() != '/') {
        path.push_back('/');
    }

    return path;
}

std::string ensureAbsolutePath(std::string path)
{
    if (path.empty()) {
        return "/";
    }

    if (path.front() != '/') {
        path.insert(path.begin(), '/');
    }

    return path;
}
}

StorageManager &StorageManager::instance()
{
    static StorageManager manager;
    return manager;
}

StorageManager &storageManager()
{
    return StorageManager::instance();
}

bool StorageManager::init()
{
    if (initialized) {
        return available;
    }

#if STORAGE_OPTION == STORAGE_OPTION_SD
    activeFilesystem = &SD;
    activeStorageLabel = "SD";
    logMessage("StorageManager: mounting SD backend...");
    SPI.begin(STORAGE_SD_PIN_SCK, STORAGE_SD_PIN_MISO, STORAGE_SD_PIN_MOSI, STORAGE_SD_PIN_CS);
    available = SD.begin(STORAGE_SD_PIN_CS);
#elif STORAGE_OPTION == STORAGE_OPTION_SPIFFS
    activeFilesystem = &SPIFFS;
    activeStorageLabel = "SPIFFS";
    logMessage(
        "StorageManager: mounting SPIFFS backend (formatOnFail=%d)...",
        STORAGE_SPIFFS_FORMAT_ON_FAIL
    );
    available = SPIFFS.begin(STORAGE_SPIFFS_FORMAT_ON_FAIL != 0);
#endif

    initialized = true;

    if (!available) {
#if STORAGE_OPTION == STORAGE_OPTION_SD
        logMessage("StorageManager: SD mount failed.");
#elif STORAGE_OPTION == STORAGE_OPTION_SPIFFS
        logMessage(
            "StorageManager: SPIFFS mount failed. Check that the selected partition scheme contains a SPIFFS partition."
        );
#endif
        return false;
    }

    logMessage(
        "StorageManager: %s mounted successfully. Total=%llu Used=%llu",
        activeStorageLabel,
        totalBytes(),
        usedBytes()
    );

    return available;
}

bool StorageManager::exists(const std::string &path) const
{
    if (!available || !activeFilesystem) {
        return false;
    }

    return activeFilesystem->exists(ensureAbsolutePath(path).c_str());
}

bool StorageManager::ensureDirectory(const std::string &path)
{
    if (!available || !activeFilesystem) {
        return false;
    }

#if STORAGE_OPTION == STORAGE_OPTION_SPIFFS
    (void)path;
    return true;
#else
    const std::string normalized = normalizeDirectoryPath(path);
    if (activeFilesystem->exists(normalized.c_str())) {
        return true;
    }
    return activeFilesystem->mkdir(normalized.c_str());
#endif
}

bool StorageManager::remove(const std::string &path)
{
    if (!available || !activeFilesystem) {
        return false;
    }

    return activeFilesystem->remove(ensureAbsolutePath(path).c_str());
}

File StorageManager::open(const std::string &path, const char *mode) const
{
    if (!available || !activeFilesystem) {
        return File();
    }

    return activeFilesystem->open(ensureAbsolutePath(path).c_str(), mode);
}

std::vector<std::string> StorageManager::listFiles(const std::string &directoryPath) const
{
    std::vector<std::string> filePaths;

    if (!available || !activeFilesystem) {
        return filePaths;
    }

    const std::string normalizedDirectory = normalizeDirectoryPath(directoryPath);

#if STORAGE_OPTION == STORAGE_OPTION_SPIFFS
    File root = activeFilesystem->open("/", FILE_READ);
    if (!root || !root.isDirectory()) {
        return filePaths;
    }

    root.rewindDirectory();
    while (true) {
        File entry = root.openNextFile();
        if (!entry) {
            break;
        }

        const char *rawName = entry.name();
        const std::string fullPath = rawName ? ensureAbsolutePath(rawName) : std::string();
        if (!fullPath.empty() && !entry.isDirectory() && fullPath.rfind(normalizedDirectory, 0) == 0) {
            filePaths.push_back(fullPath);
        }

        entry.close();
    }
    root.close();
#else
    File directory = activeFilesystem->open(normalizedDirectory.c_str(), FILE_READ);
    if (!directory || !directory.isDirectory()) {
        return filePaths;
    }

    directory.rewindDirectory();
    while (true) {
        File entry = directory.openNextFile();
        if (!entry) {
            break;
        }

        const char *rawName = entry.name();
        std::string fullPath = rawName ? std::string(rawName) : std::string();
        if (!fullPath.empty() && fullPath.front() != '/') {
            fullPath = normalizedDirectory + fullPath;
        } else if (fullPath.empty() && rawName) {
            fullPath = normalizedDirectory + rawName;
        }

        if (!fullPath.empty() && !entry.isDirectory()) {
            filePaths.push_back(fullPath);
        }

        entry.close();
    }
    directory.close();
#endif

    std::sort(filePaths.begin(), filePaths.end());
    return filePaths;
}

uint64_t StorageManager::totalBytes() const
{
    if (!available || !activeFilesystem) {
        return 0;
    }

#if STORAGE_OPTION == STORAGE_OPTION_SD
    return SD.totalBytes();
#else
    return SPIFFS.totalBytes();
#endif
}

uint64_t StorageManager::usedBytes() const
{
    if (!available || !activeFilesystem) {
        return 0;
    }

#if STORAGE_OPTION == STORAGE_OPTION_SD
    return SD.usedBytes();
#else
    return SPIFFS.usedBytes();
#endif
}
