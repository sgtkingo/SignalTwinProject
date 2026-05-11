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
        return true;

    logMessage("Initializing DataBundle Manager...");

    if (!storageManager().isAvailable() && !storageManager().init())
    {
        logMessage("DataBundle Manager Failed to initialize");
        return false;
    }

    if (!ensureStorageDirectories())
    {
        logMessage("Error: dir DataBundles failed to create");
    }

    logMessage("DataBundle Manager initialized successfully");

    #ifdef DATABUNDLE_DEBUG
    getStorageInfo();
    listAllBundles();
    #endif

    initialized = true;
    return true;
}

/**
 * @brief Initialize DataBundles directory
 * @return True if init was succesful, false otherwise
 */
bool DataBundleManager::ensureStorageDirectories()
{
    if (!storageManager().ensureDirectory(root))
    {
        logMessage("Error: Failed to prepare /DataBundles directory");
        return false;
    }

    #ifdef DATABUNDLE_DEBUG
    // log.txt creation test
    File myFile = storageManager().open("/DataBundles/log.txt", FILE_WRITE);

    if (myFile)
    {
        myFile.println("Device Data: 123");
        myFile.close(); // Save and close
        logMessage("Created log.txt successfully");
    }
    else
    {
        logMessage("Error: Failed to create log.txt (Check permissions/connections)");
        return false;
    }       
    #endif

    return true;
}

void DataBundleManager::getStorageInfo()
{
    uint64_t total = storageManager().totalBytes();
    uint64_t used = storageManager().usedBytes();

    logMessage("Total Bytes: %llu", total);
    logMessage("Used Bytes: %llu", used);

    storageManager().exists("/DataBundles/log.txt") ? logMessage("log.txt exists!") : logMessage("log.txt doesnt exist");
}

bool DataBundleManager::startRecording(std::string deviceName, std::string deviceUid)
{
    recordingDataPoints.clear();
    recordingBundleMetadata.deviceName = deviceName;
    recordingBundleMetadata.deviceUid = deviceUid;
    recordingBundleMetadata.filePath = "";
    recordingBundleMetadata.startDate = "";
    recordingStartMs = currentRuntimeMs();
    recordingSampleCounter = 0;

    uint8_t tempOrder = 1;
    std::string temp = root + deviceName + "_0" + std::to_string(tempOrder) + ".csv";
    while (storageManager().exists(temp))
    {
        if (tempOrder < 10)
        {
            std::string toRemove = "0" + std::to_string(tempOrder) + ".csv";
            if (temp.length() >= toRemove.length())
            {
                temp.resize(temp.length() - toRemove.length());
            }
        }
        else
        {
            std::string toRemove = std::to_string(tempOrder) + ".csv";
            if (temp.length() >= toRemove.length())
            {
                temp.resize(temp.length() - toRemove.length());
            }
        }

        tempOrder++;

        if (tempOrder < 10)
        {
            temp+=("0" + std::to_string(tempOrder) + ".csv");
        }
        else
        {
            temp+=(std::to_string(tempOrder) + ".csv");
        }
    }

    recordingBundleMetadata.filePath = temp;

    // TODO: persist real recording start date/time metadata.
    //recordingBundleMetadata.startDate
    
    return true;
}

bool DataBundleManager::saveNewDataPoint(std::string signalName, std::string value)
{
    if (recordingBundleMetadata.filePath.empty())
    {
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
    return true;
}

bool DataBundleManager::saveRecording()
{
    if (recordingBundleMetadata.filePath.empty())
    {
        logMessage("Error: No active recording to save");
        return false;
    }

    if (recordingDataPoints.empty())
    {
        logMessage("Error: Recording has no data points");
        discardRecording();
        return false;
    }

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
        if(isBundleStorageFull()){
            pruneOldestBundle();
            reloadBundleFileNames();
        }

        //logMessage("Created %s successfully", recordingBundleMetadata.filePath.c_str());
    }
    else
    {
        logMessage("Error: Failed to create %s", recordingBundleMetadata.filePath.c_str());
        return false;
    }

    listAllBundles();
    printBundleCsv(recordingBundleMetadata.filePath);
    discardRecording();

    return true;
}

void DataBundleManager::discardRecording()
{
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
    reloadBundleFileNames();

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
    if (filesToDelete.empty()) {
        return true;
    }

    for (const auto &file : filesToDelete)
    {
        storageManager().remove(file);
    }

    bundleFileNames.clear();

    return true;
}

bool DataBundleManager::reloadBundleFileNames()
{
    bundleFileNames.clear();

    const std::vector<std::string> bundlePaths = storageManager().listFiles(root);
    if (bundlePaths.empty() && !storageManager().ensureDirectory(root)) {
        logMessage("Error: Failed to open /DataBundles/ directory whilst getting bundle names");
        return false;
    }

    const std::string rootPath = root;
    for (const std::string &fullPath : bundlePaths) {
        if (fullPath.rfind(rootPath, 0) == 0) {
            bundleFileNames.push_back(fullPath.substr(rootPath.length()));
        } else {
            bundleFileNames.push_back(fullPath);
        }
    }

    return true;
}

void DataBundleManager::pruneOldestBundle()
{
    const std::vector<std::string> bundlePaths = storageManager().listFiles(root);
    if (bundlePaths.empty())
        return;

    storageManager().remove(bundlePaths.front());
}

void DataBundleManager::listAllBundles()
{
    logMessage("--- Listing Files in /DataBundles ---");

    const std::vector<std::string> bundlePaths = storageManager().listFiles(root);
    if (bundlePaths.empty()) {
        logMessage("No bundle files found in /DataBundles");
        return;
    }

    for (const std::string &bundlePath : bundlePaths)
    {
        File file = storageManager().open(bundlePath, FILE_READ);
        if (!file) {
            logMessage("  [FILE] %s  (unavailable)", bundlePath.c_str());
            continue;
        }

        logMessage("  [FILE] %s  (%u bytes)", file.name(), file.size());
        file.close();
    }

    logMessage("--- End of List ---");
}

void DataBundleManager::printBundleCsv(std::string filename)
{
    std::string fullPath = filename.rfind(root, 0) == 0 ? filename : std::string(root) + filename;

    logMessage("--- Reading CSV: %s ---", fullPath.c_str());

    File file = storageManager().open(fullPath, FILE_READ);

    if (!file)
    {
        logMessage("Error: Could not open file %s", fullPath.c_str());
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
                logMessage("%s", currentLine.c_str());
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
        logMessage("%s", currentLine.c_str());
    }

    file.close();
    logMessage("--- End of CSV ---");
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
        logMessage("Error: Could not open file %s", fullPath.c_str());
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
        logMessage("Error: Could not open file %s", fullPath.c_str());
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
