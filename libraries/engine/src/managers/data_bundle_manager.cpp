/**
 * @file data_bundle_manager.hpp
 * @brief Recording bundle manager.
 *
 * This source file implements storage and retrieval of recorded device signal bundles.
 *
 * @copyright 2025 MTA
 * @author Ondřej Wrubel
 **/

#include "data_bundle_manager.hpp"
#include "expt.hpp"

#ifdef ARDUINO_H
#include <Arduino.h>
#endif

#include <cctype>
#include <cstdio>

namespace
{
unsigned long currentRuntimeMs()
{
#ifdef ARDUINO_H
    return millis();
#else
    return 0;
#endif
}

int findColumnIndex(const std::vector<std::string> &columns, const char *name)
{
    for (size_t i = 0; i < columns.size(); ++i)
    {
        if (columns[i] == name)
        {
            return static_cast<int>(i);
        }
    }
    return -1;
}

bool isCsvBundlePath(const std::string &path)
{
    return path.size() >= 4 && path.substr(path.size() - 4) == ".csv";
}

std::string getBundleFileNameFromPath(const std::string &path)
{
    const size_t separator = path.find_last_of('/');
    if (separator == std::string::npos) {
        return path;
    }
    return path.substr(separator + 1);
}

std::string makeStorageSafeBundleBase(const std::string &deviceUid, const std::string &deviceName)
{
    const std::string source = !deviceUid.empty() ? deviceUid : deviceName;
    std::string safe;
    safe.reserve(source.size());

    for (char c : source) {
        const unsigned char ch = static_cast<unsigned char>(c);
        if (std::isalnum(ch) || c == '-' || c == '_') {
            safe.push_back(c);
        } else if (c == ' ' || c == '.') {
            safe.push_back('_');
        }
    }

    while (!safe.empty() && safe.back() == '_') {
        safe.pop_back();
    }

    if (safe.empty()) {
        safe = "bundle";
    }

    static const size_t MAX_BUNDLE_BASENAME_LENGTH = 14;
    if (safe.size() > MAX_BUNDLE_BASENAME_LENGTH) {
        safe.resize(MAX_BUNDLE_BASENAME_LENGTH);
        while (!safe.empty() && safe.back() == '_') {
            safe.pop_back();
        }
    }

    return safe.empty() ? "bundle" : safe;
}

std::string makeBundlePath(const char *root, const std::string &base, unsigned int order)
{
    char suffix[16];
    if (order < 100) {
        std::snprintf(suffix, sizeof(suffix), "_%02u.csv", order);
    } else {
        std::snprintf(suffix, sizeof(suffix), "_%u.csv", order);
    }

    return std::string(root) + base + suffix;
}
}

// Pin definitions for SPI communication
// Honestly i have no idea where did i find them but they work :D
/*
#define MOSI 11
#define MISO 13
#define CLK  12
#define CS   10
*/

/**
 * @brief Private constructor for singleton pattern
 */
DataBundleManager::DataBundleManager() {}

/**
 * @brief Destructor
 */
DataBundleManager::~DataBundleManager() {}

/**
 * @brief Initialize the data bundle manager and selected storage backend
 * Also calls ensureStorageDirectories() and prints storage info
 * @return True if initialization was successful, false otherwise
 */
bool DataBundleManager::init()
{
    if (initialized)
    {
        debugLogMessage("DataBundleManager::init", "init skipped", "already initialized");
        return true;
    }

    debugLogMessage(DEBUG_VERBOSE_IMPORTANT, "DataBundleManager::init", "init", "initializing DataBundle manager");

    if (!storageManager().isAvailable() && !storageManager().init())
    {
        debugLogMessage(DEBUG_VERBOSE_ERRORS, "DataBundleManager::init", "storage unavailable", "storage manager failed to initialize");
        return false;
    }

    if (!ensureStorageDirectories())
    {
        debugLogMessage(DEBUG_VERBOSE_ERRORS, "DataBundleManager::init", "storage write failed", "%s directory failed to create", root);
    }

    debugLogMessage(DEBUG_VERBOSE_IMPORTANT, "DataBundleManager::init", "init", "initialized successfully");

#if ENABLE_DEBUG && DEBUG_VERBOSE_LEVEL >= 3
    getStorageInfo();
    listAllBundles();
#endif

    initialized = true;
    return true;
}

/**
 * @brief Initialize records directory
 * @return True if init was succesful, false otherwise
 */
bool DataBundleManager::ensureStorageDirectories()
{
    if (!storageManager().ensureDirectory(root))
    {
        debugLogMessage(DEBUG_VERBOSE_ERRORS, "DataBundleManager::ensureStorageDirectories", "storage write failed", "failed to prepare %s directory", root);
        return false;
    }

    return true;
}

void DataBundleManager::getStorageInfo()
{
    uint64_t total = storageManager().totalBytes();
    uint64_t used = storageManager().usedBytes();

    debugLogMessage("DataBundleManager::getStorageInfo", "storage read", "totalBytes=%llu usedBytes=%llu", total, used);
}

bool DataBundleManager::startRecording(std::string deviceName, std::string deviceUid)
{
    debugLogMessage(DEBUG_VERBOSE_IMPORTANT, "DataBundleManager::startRecording", "recording start", "deviceName=%s deviceUid=%s", deviceName.c_str(), deviceUid.c_str());
    recordingDataPoints.clear();
    recordingBundleMetadata.deviceName = deviceName;
    recordingBundleMetadata.deviceUid = deviceUid;
    recordingBundleMetadata.filePath = "";
    recordingBundleMetadata.startDate = "";
    recordingStartMs = currentRuntimeMs();
    recordingSampleCounter = 0;

    if (!ensureStorageDirectories())
    {
        debugLogMessage(DEBUG_VERBOSE_ERRORS, "DataBundleManager::startRecording", "storage write failed", "failed to prepare %s directory", root);
        return false;
    }

    unsigned int tempOrder = 1;
    const std::string fileBase = makeStorageSafeBundleBase(deviceUid, deviceName);
    std::string temp = makeBundlePath(root, fileBase, tempOrder);
    while (storageManager().exists(temp))
    {
        debugLogMessage("DataBundleManager::startRecording", "storage read", "candidate exists path=%s", temp.c_str());
        tempOrder++;
        temp = makeBundlePath(root, fileBase, tempOrder);
    }

    recordingBundleMetadata.filePath = temp;
    debugLogMessage(DEBUG_VERBOSE_IMPORTANT, "DataBundleManager::startRecording", "recording start", "fileBase=%s filePath=%s startMs=%lu", fileBase.c_str(), temp.c_str(), recordingStartMs);

    // TODO: persist real recording start date/time metadata.
    //recordingBundleMetadata.startDate
    
    return true;
}

bool DataBundleManager::saveNewDataPoint(std::string signalName, std::string value)
{
    if (recordingBundleMetadata.filePath.empty())
    {
        debugLogMessage(DEBUG_VERBOSE_ERRORS, "DataBundleManager::saveNewDataPoint", "recording state invalid", "no active recording signal=%s", signalName.c_str());
        return false;
    }

    const unsigned long now = currentRuntimeMs();
    const unsigned long runtimeMs = now >= recordingStartMs ? now - recordingStartMs : 0;
    DataPoint dataPoint = {
        recordingBundleMetadata.deviceName,
        recordingBundleMetadata.deviceUid,
        recordingSampleCounter++,
        runtimeMs,
        signalName,
        value
    };
    recordingDataPoints.push_back(dataPoint);
    debugLogMessage(
        "DataBundleManager::saveNewDataPoint",
        "runtime memory write",
        "sample=%lu runtimeMs=%lu deviceUid=%s signal=%s value=%s",
        dataPoint.sampleIndex,
        dataPoint.runtimeMs,
        dataPoint.deviceUid.c_str(),
        dataPoint.signalName.c_str(),
        dataPoint.value.c_str());
    return true;
}

bool DataBundleManager::saveRecording()
{
    if (recordingBundleMetadata.filePath.empty())
    {
        debugLogMessage(DEBUG_VERBOSE_ERRORS, "DataBundleManager::saveRecording", "recording state invalid", "no active recording to save");
        return false;
    }

    if (recordingDataPoints.empty())
    {
        debugLogMessage(DEBUG_VERBOSE_ERRORS, "DataBundleManager::saveRecording", "recording state invalid", "recording has no data points");
        discardRecording();
        return false;
    }

    debugLogMessage(
        DEBUG_VERBOSE_IMPORTANT,
        "DataBundleManager::saveRecording",
        "storage write",
        "saving filePath=%s samples=%u",
        recordingBundleMetadata.filePath.c_str(),
        static_cast<unsigned int>(recordingDataPoints.size()));
    File saved = storageManager().open(recordingBundleMetadata.filePath, FILE_WRITE);

    if (saved)
    {
        saved.println("DeviceName;DeviceUid;SampleIndex;RuntimeMs;SignalName;Value");
        for (unsigned int i = 0; i < recordingDataPoints.size(); i++)
        {
            saved.printf(
                "%s;%s;%lu;%lu;%s;%s\n",
                recordingDataPoints[i].deviceName.c_str(),
                recordingDataPoints[i].deviceUid.c_str(),
                recordingDataPoints[i].sampleIndex,
                recordingDataPoints[i].runtimeMs,
                recordingDataPoints[i].signalName.c_str(),
                recordingDataPoints[i].value.c_str());
        }

        saved.close(); // Save and close

        reloadBundleFileNames();
        if (bundleFileNames.empty())
        {
            const std::string fileName = getBundleFileNameFromPath(recordingBundleMetadata.filePath);
            if (!fileName.empty())
            {
                bundleFileNames.push_back(fileName);
                bundleFileNamesLoaded = true;
                debugLogMessage(DEBUG_VERBOSE_IMPORTANT, "DataBundleManager::saveRecording", "storage cache fallback", "cached saved bundle=%s", fileName.c_str());
            }
        }

        if(isBundleStorageFull()){
            pruneOldestBundle();
            reloadBundleFileNames();
        }

        //logMessage("Created %s successfully", recordingBundleMetadata.filePath.c_str());
    }
    else
    {
        debugLogMessage(DEBUG_VERBOSE_ERRORS, "DataBundleManager::saveRecording", "storage write failed", "failed to create %s", recordingBundleMetadata.filePath.c_str());
        return false;
    }

    listAllBundles();
    printBundleCsv(recordingBundleMetadata.filePath);
    clearRecordingState("saved recording state");

    return true;
}

void DataBundleManager::discardRecording()
{
    clearRecordingState("discarding unsaved recording");
}

void DataBundleManager::clearRecordingState(const char *reason)
{
    debugLogMessage(DEBUG_VERBOSE_IMPORTANT, "DataBundleManager::clearRecordingState", "recording state", "%s filePath=%s samples=%u", reason ? reason : "clearing recording", recordingBundleMetadata.filePath.c_str(), static_cast<unsigned int>(recordingDataPoints.size()));
    recordingBundleMetadata.deviceName = "";
    recordingBundleMetadata.deviceUid = "";
    recordingBundleMetadata.filePath = "";
    recordingBundleMetadata.startDate = "";
    recordingDataPoints.clear();
    recordingStartMs = 0;
    recordingSampleCounter = 0;
}

std::array<DataBundleBuffer,6> DataBundleManager::getBundlePage(unsigned char page)
{
    if (!bundleFileNamesLoaded)
    {
        reloadBundleFileNames();
    }
    debugLogMessage("DataBundleManager::getBundlePage", "storage read", "page=%u bundleCount=%u", page, static_cast<unsigned int>(bundleFileNames.size()));

    std::array<DataBundleBuffer,6> buff{};
    const size_t pageStart = static_cast<size_t>(page) * 6;
    if (pageStart >= bundleFileNames.size())
    {
        return buff;
    }

    const size_t remaining = bundleFileNames.size() - pageStart;
    const size_t count = remaining < 6 ? remaining : 6;

    for(size_t i = 0; i < count; i++){
        const unsigned char bundleIndex = static_cast<unsigned char>(pageStart + i);
        buff[i].metaBuffer = getBundleMetadata(bundleIndex);
        buff[i].dataBuffer = getBundleValuePreview(bundleIndex);
    }
    return buff;
}

bool DataBundleManager::deleteAllBundles()
{
    std::vector<std::string> filesToDelete = storageManager().listFiles(root);
    debugLogMessage(DEBUG_VERBOSE_IMPORTANT, "DataBundleManager::deleteAllBundles", "storage write", "delete count=%u", static_cast<unsigned int>(filesToDelete.size()));
    if (filesToDelete.empty()) {
        return true;
    }

    for (const auto &file : filesToDelete)
    {
        if (isCsvBundlePath(file))
        {
            storageManager().remove(file);
        }
    }

    bundleFileNames.clear();
    bundleFileNamesLoaded = true;

    return true;
}

bool DataBundleManager::reloadBundleFileNames()
{
    const std::vector<std::string> previousBundleFileNames = bundleFileNames;
    bundleFileNames.clear();

    const std::vector<std::string> bundlePaths = storageManager().listFiles(root);
    if (bundlePaths.empty() && !storageManager().ensureDirectory(root)) {
        debugLogMessage(DEBUG_VERBOSE_ERRORS, "DataBundleManager::reloadBundleFileNames", "storage read failed", "failed to open %s directory", root);
        return false;
    }

    const std::string rootPath = root;
    for (const std::string &fullPath : bundlePaths) {
        if (!isCsvBundlePath(fullPath)) {
            continue;
        }

        if (fullPath.rfind(rootPath, 0) == 0) {
            bundleFileNames.push_back(fullPath.substr(rootPath.length()));
        } else {
            bundleFileNames.push_back(fullPath);
        }
    }

    if (bundleFileNames.empty() && !previousBundleFileNames.empty())
    {
        for (const std::string &fileName : previousBundleFileNames)
        {
            const std::string fullPath = fileName.rfind(rootPath, 0) == 0 ? fileName : rootPath + fileName;
            if (isCsvBundlePath(fullPath) && storageManager().exists(fullPath))
            {
                bundleFileNames.push_back(getBundleFileNameFromPath(fullPath));
            }
        }

        if (!bundleFileNames.empty())
        {
            debugLogMessage(DEBUG_VERBOSE_IMPORTANT, "DataBundleManager::reloadBundleFileNames", "storage cache fallback", "restored cached bundle filenames count=%u", static_cast<unsigned int>(bundleFileNames.size()));
        }
    }

    bundleFileNamesLoaded = true;
    debugLogMessage(DEBUG_VERBOSE_IMPORTANT, "DataBundleManager::reloadBundleFileNames", "storage read", "loaded bundle filenames count=%u", static_cast<unsigned int>(bundleFileNames.size()));
    return true;
}

void DataBundleManager::pruneOldestBundle()
{
    const std::vector<std::string> bundlePaths = storageManager().listFiles(root);
    for (const std::string &bundlePath : bundlePaths)
    {
        if (!isCsvBundlePath(bundlePath))
        {
            continue;
        }

        debugLogMessage(DEBUG_VERBOSE_IMPORTANT, "DataBundleManager::pruneOldestBundle", "storage write", "removing oldest=%s", bundlePath.c_str());
        storageManager().remove(bundlePath);
        return;
    }
}

void DataBundleManager::listAllBundles()
{
    debugLogMessage("DataBundleManager::listAllBundles", "storage read", "listing %s", root);

    const std::vector<std::string> bundlePaths = storageManager().listFiles(root);
    if (bundlePaths.empty()) {
        debugLogMessage("DataBundleManager::listAllBundles", "storage read", "no bundle files found");
        return;
    }

    for (const std::string &bundlePath : bundlePaths)
    {
        if (!isCsvBundlePath(bundlePath)) {
            continue;
        }

        File file = storageManager().open(bundlePath, FILE_READ);
        if (!file) {
            debugLogMessage("DataBundleManager::listAllBundles", "storage read failed", "file unavailable %s", bundlePath.c_str());
            continue;
        }

        debugLogMessage("DataBundleManager::listAllBundles", "storage read", "file=%s bytes=%u", file.name(), file.size());
        file.close();
    }
}

void DataBundleManager::printBundleCsv(std::string filename)
{
    std::string fullPath = filename.rfind(root, 0) == 0 ? filename : std::string(root) + filename;

    debugLogMessage("DataBundleManager::printBundleCsv", "storage read", "reading csv=%s", fullPath.c_str());

    File file = storageManager().open(fullPath, FILE_READ);

    if (!file)
    {
        debugLogMessage("DataBundleManager::printBundleCsv", "storage read failed", "could not open file %s", fullPath.c_str());
        return;
    }

    std::string currentLine = "";

    while (file.available())
    {
        char c = (char)file.read();

        if (c == '\n')
        {
            if (!currentLine.empty())
            {
                debugLogMessage("DataBundleManager::printBundleCsv", "storage read", "%s", currentLine.c_str());
            }
            currentLine.clear();
        }
        else if (c != '\r')
        {
            currentLine += c;
        }
    }

    if (!currentLine.empty())
    {
        debugLogMessage("DataBundleManager::printBundleCsv", "storage read", "%s", currentLine.c_str());
    }

    file.close();
    debugLogMessage("DataBundleManager::printBundleCsv", "storage read", "end csv=%s", fullPath.c_str());
}

std::string DataBundleManager::getBundleCsvText(unsigned char index, size_t maxBytes)
{
    if (!bundleFileNamesLoaded)
    {
        reloadBundleFileNames();
    }

    if (index >= bundleFileNames.size())
    {
        debugLogMessage(DEBUG_VERBOSE_ERRORS, "DataBundleManager::getBundleCsvText", "storage read failed", "index=%u count=%u", index, static_cast<unsigned int>(bundleFileNames.size()));
        return "";
    }

    const std::string fullPath = std::string(root) + bundleFileNames[index];
    File file = storageManager().open(fullPath, FILE_READ);
    if (!file)
    {
        debugLogMessage(DEBUG_VERBOSE_ERRORS, "DataBundleManager::getBundleCsvText", "storage read failed", "could not open file %s", fullPath.c_str());
        return "";
    }

    std::string csv;
    csv.reserve(maxBytes < 256 ? maxBytes : 256);
    size_t bytesRead = 0;
    while (file.available() && bytesRead < maxBytes)
    {
        const char c = static_cast<char>(file.read());
        if (c != '\r')
        {
            csv += c;
            ++bytesRead;
        }
    }

    const bool truncated = file.available();
    file.close();

    if (truncated)
    {
        csv += "\n...";
    }

    debugLogMessage("DataBundleManager::getBundleCsvText", "storage read", "file=%s bytes=%u truncated=%u", fullPath.c_str(), static_cast<unsigned int>(bytesRead), truncated ? 1 : 0);
    return csv;
}

BundleMetadata DataBundleManager::getBundleMetadata(unsigned char index){
    if(index >= bundleFileNames.size())
    {
        return {"","","",""};
    }

    std::string fullPath = root + bundleFileNames[index];
    std::string fileName = bundleFileNames[index];
    std::string deviceName = fileName.substr(0,fileName.find("_"));
    std::string deviceUid = "";

    File file = storageManager().open(fullPath, FILE_READ);

    if (!file)
    {
        debugLogMessage("DataBundleManager::getBundleMetadata", "storage read failed", "could not open file %s", fullPath.c_str());
        return {deviceName,deviceUid,fullPath,""};
    }

    const std::string header = readLine(file);
    const std::string firstDataLine = readLine(file);
    const std::vector<std::string> headerColumns = parseCSVLine(header);
    const std::vector<std::string> firstDataColumns = parseCSVLine(firstDataLine);

    const int deviceNameIndex = findColumnIndex(headerColumns, "DeviceName");
    const int deviceUidIndex = findColumnIndex(headerColumns, "DeviceUid");

    if (deviceNameIndex >= 0 && static_cast<size_t>(deviceNameIndex) < firstDataColumns.size() && !firstDataColumns[deviceNameIndex].empty())
    {
        deviceName = firstDataColumns[deviceNameIndex];
    }

    if (deviceUidIndex >= 0 && static_cast<size_t>(deviceUidIndex) < firstDataColumns.size())
    {
        deviceUid = firstDataColumns[deviceUidIndex];
    }

    file.close();
    debugLogMessage("DataBundleManager::getBundleMetadata", "storage read", "file=%s deviceName=%s deviceUid=%s", fullPath.c_str(), deviceName.c_str(), deviceUid.c_str());
    return {deviceName,deviceUid,fullPath,""};
}

std::array<std::string,10> DataBundleManager::getBundleValuePreview(unsigned char index){
    if(index >= bundleFileNames.size())
    {
        std::array<std::string,10> empty;
        for(int k=0; k<10; k++) empty[k] = "0";
        return empty;
    }

    std::string fullPath = std::string(root) + bundleFileNames[index];

    //logMessage("Full path to the file is %s",fullPath.c_str());
    std::array<std::string,10> temp;
    for(int k=0; k<10; k++) temp[k] = "0";
    
    File file = storageManager().open(fullPath, FILE_READ);
    
    if (!file)
    {
        debugLogMessage("DataBundleManager::getBundleValuePreview", "storage read failed", "could not open file %s", fullPath.c_str());
        return temp;
    }
    
    const std::string header = readLine(file);
    const std::vector<std::string> headerColumns = parseCSVLine(header);
    int signalIndex = findColumnIndex(headerColumns, "SignalName");
    int valueIndex = findColumnIndex(headerColumns, "Value");
    if (signalIndex < 0 || valueIndex < 0)
    {
        signalIndex = 0;
        valueIndex = 1;
    }

    std::string line = readLine(file);

    if(line.empty()) {
        file.close();
        return temp;
    }

    std::vector<std::string> dataParsed = parseCSVLine(line);
    if (static_cast<size_t>(signalIndex) >= dataParsed.size() || static_cast<size_t>(valueIndex) >= dataParsed.size())
    {
        file.close();
        return temp;
    }

    std::string signalName = dataParsed[signalIndex];
    temp[0] = dataParsed[valueIndex];

    unsigned char written = 1;
    while (written < 10 && file.available()) {
        line = readLine(file);

        if (line.empty()) {
            continue;
        }

        dataParsed = parseCSVLine(line);

        if (static_cast<size_t>(signalIndex) >= dataParsed.size() || static_cast<size_t>(valueIndex) >= dataParsed.size())
        {
            continue;
        }

        if(signalName == dataParsed[signalIndex]){
            temp[written++] = dataParsed[valueIndex];
        }
    }

    while (written < 10)
    {
        temp[written] = temp[written - 1];
        ++written;
    }

    file.close();   
    debugLogMessage("DataBundleManager::getBundleValuePreview", "storage read", "file=%s signal=%s written=%u", fullPath.c_str(), signalName.c_str(), written);
    return temp;
}

bool DataBundleManager::isBundleStorageFull(){
    return (bundleFileNames.size()>=30)? 1 : 0;
}

void DataBundleManager::deleteBundle(unsigned char index){
    if(index >= bundleFileNames.size())
        return;

    std::string fullPath = std::string(root) + bundleFileNames[index];
    storageManager().remove(fullPath);
    bundleFileNames.erase(bundleFileNames.begin() + index);
    bundleFileNamesLoaded = true;
}

std::string DataBundleManager::readLine(File &file) {
    std::string line = "";
    char c;

    while (file.available()) {
        c = (char)file.read();

        if (c == '\n') {
            break;
        } 
        else if (c != '\r') {
            line += c;
        }
    }
    return line;
}

std::vector<std::string> DataBundleManager::parseCSVLine(std::string line) {
    std::vector<std::string> data;

    size_t start = 0;
    while (start <= line.size())
    {
        const size_t separator = line.find(';', start);
        if (separator == std::string::npos)
        {
            data.push_back(line.substr(start));
            break;
        }

        data.push_back(line.substr(start, separator - start));
        start = separator + 1;
    }
    
    return data;
}
