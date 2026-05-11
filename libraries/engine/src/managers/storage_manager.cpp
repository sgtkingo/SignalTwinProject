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
        debugLogMessage("StorageManager::init", "storage init skipped", "already initialized available=%d", available);
        return available;
    }

#if STORAGE_OPTION == STORAGE_OPTION_SD
    activeFilesystem = &SD;
    activeStorageLabel = "SD";
    debugLogMessage("StorageManager::init", "storage mount", "mounting SD backend");
    SPI.begin(STORAGE_SD_PIN_SCK, STORAGE_SD_PIN_MISO, STORAGE_SD_PIN_MOSI, STORAGE_SD_PIN_CS);
    available = SD.begin(STORAGE_SD_PIN_CS);
#elif STORAGE_OPTION == STORAGE_OPTION_SPIFFS
    activeFilesystem = &SPIFFS;
    activeStorageLabel = "SPIFFS";
    debugLogMessage(
        "StorageManager::init",
        "storage mount",
        "mounting SPIFFS backend formatOnFail=%d",
        STORAGE_SPIFFS_FORMAT_ON_FAIL
    );
    available = SPIFFS.begin(STORAGE_SPIFFS_FORMAT_ON_FAIL != 0);
#endif

    initialized = true;

    if (!available) {
#if STORAGE_OPTION == STORAGE_OPTION_SD
        debugLogMessage("StorageManager::init", "storage mount failed", "SD mount failed");
#elif STORAGE_OPTION == STORAGE_OPTION_SPIFFS
        debugLogMessage(
            "StorageManager::init",
            "storage mount failed",
            "SPIFFS mount failed. Check that the selected partition scheme contains a SPIFFS partition."
        );
#endif
        return false;
    }

    debugLogMessage(
        "StorageManager::init",
        "storage mount",
        "%s mounted successfully total=%llu used=%llu",
        activeStorageLabel,
        totalBytes(),
        usedBytes()
    );

    return available;
}

bool StorageManager::exists(const std::string &path) const
{
    if (!available || !activeFilesystem) {
        debugLogMessage("StorageManager::exists", "storage unavailable", "exists skipped path=%s", path.c_str());
        return false;
    }

    const std::string absolutePath = ensureAbsolutePath(path);
    const bool result = activeFilesystem->exists(absolutePath.c_str());
    debugLogMessage("StorageManager::exists", "storage read", "path=%s exists=%d", absolutePath.c_str(), result);
    return result;
}

bool StorageManager::ensureDirectory(const std::string &path)
{
    if (!available || !activeFilesystem) {
        debugLogMessage("StorageManager::ensureDirectory", "storage unavailable", "ensure directory skipped path=%s", path.c_str());
        return false;
    }

#if STORAGE_OPTION == STORAGE_OPTION_SPIFFS
    debugLogMessage("StorageManager::ensureDirectory", "spiffs virtual directory", "path=%s", path.c_str());
    return true;
#else
    const std::string normalized = normalizeDirectoryPath(path);
    if (activeFilesystem->exists(normalized.c_str())) {
        debugLogMessage("StorageManager::ensureDirectory", "storage read", "directory already exists path=%s", normalized.c_str());
        return true;
    }
    const bool created = activeFilesystem->mkdir(normalized.c_str());
    debugLogMessage("StorageManager::ensureDirectory", "storage write", "mkdir path=%s result=%d", normalized.c_str(), created);
    return created;
#endif
}

bool StorageManager::remove(const std::string &path)
{
    if (!available || !activeFilesystem) {
        debugLogMessage("StorageManager::remove", "storage unavailable", "remove skipped path=%s", path.c_str());
        return false;
    }

    const std::string absolutePath = ensureAbsolutePath(path);
    const bool removed = activeFilesystem->remove(absolutePath.c_str());
    debugLogMessage("StorageManager::remove", "storage write", "remove path=%s result=%d", absolutePath.c_str(), removed);
    return removed;
}

File StorageManager::open(const std::string &path, const char *mode) const
{
    if (!available || !activeFilesystem) {
        debugLogMessage("StorageManager::open", "storage unavailable", "open skipped path=%s mode=%s", path.c_str(), mode ? mode : "-");
        return File();
    }

    const std::string absolutePath = ensureAbsolutePath(path);
    debugLogMessage("StorageManager::open", "storage read/write", "open path=%s mode=%s", absolutePath.c_str(), mode ? mode : "-");
    return activeFilesystem->open(absolutePath.c_str(), mode);
}

std::vector<std::string> StorageManager::listFiles(const std::string &directoryPath) const
{
    std::vector<std::string> filePaths;

    if (!available || !activeFilesystem) {
        debugLogMessage("StorageManager::listFiles", "storage unavailable", "list skipped directory=%s", directoryPath.c_str());
        return filePaths;
    }

    const std::string normalizedDirectory = normalizeDirectoryPath(directoryPath);
    debugLogMessage("StorageManager::listFiles", "storage read", "listing directory=%s", normalizedDirectory.c_str());

#if STORAGE_OPTION == STORAGE_OPTION_SPIFFS
    File root = activeFilesystem->open("/", FILE_READ);
    if (!root || !root.isDirectory()) {
        debugLogMessage("StorageManager::listFiles", "storage read failed", "cannot open SPIFFS root");
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
        debugLogMessage("StorageManager::listFiles", "storage read failed", "cannot open directory=%s", normalizedDirectory.c_str());
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
    debugLogMessage("StorageManager::listFiles", "storage read", "listed directory=%s count=%u", normalizedDirectory.c_str(), static_cast<unsigned int>(filePaths.size()));
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
