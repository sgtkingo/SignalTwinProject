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

bool DataBundleManager::startRecording(std::string deviceName)
{
    recordingBundleMetadata.deviceName = deviceName;

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
    // TODO: persist a real timestamp for each sample.
    DataPoint dataPoint = {signalName, value, ""};
    recordingDataPoints.push_back(dataPoint);
    return true;
}

bool DataBundleManager::saveRecording()
{
    File saved = storageManager().open(recordingBundleMetadata.filePath, FILE_WRITE);

    if (saved)
    {
        saved.println("SignalName;Value;Time");
        for (unsigned int i = 0; i < recordingDataPoints.size(); i++)
        {
            saved.printf("%s;%s;%s\n", recordingDataPoints[i].signalName.c_str(), recordingDataPoints[i].value.c_str(), recordingDataPoints[i].time.c_str());
        }

        saved.close(); // Save and close

        if(isBundleStorageFull()){
        pruneOldestBundle();
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

    return true;
}

void DataBundleManager::discardRecording()
{
    recordingBundleMetadata.deviceName = "";
    recordingBundleMetadata.filePath = "";
    recordingBundleMetadata.startDate = "";
    recordingDataPoints.clear();
}

std::array<DataBundleBuffer,6> DataBundleManager::getBundlePage(unsigned char page)
{
    std::array<DataBundleBuffer,6> buff;
    for(unsigned char i=0;i<6&&i<bundleFileNames.size()-(6*page);i++){
        buff[i].metaBuffer = getBundleMetadata(i+(page*6));
        buff[i].dataBuffer = getBundleValuePreview(i+(page*6));
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
    std::string fullPath = root + bundleFileNames[index];

    File file = storageManager().open(fullPath, FILE_READ);

    if (!file)
    {
        logMessage("Error: Could not open file %s", fullPath.c_str());
        return {"","",""};
    }

    std::string fileName = file.name();
    std::string deviceName = fileName.substr(0,fileName.find("_"));

    return {deviceName,fullPath,""};
}

std::array<std::string,10> DataBundleManager::getBundleValuePreview(unsigned char index){
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
    
    std::string header = readLine(file);
    logMessage("DEBUG: Header: '%s'", header.c_str());

    std::string line = readLine(file);
    logMessage("DEBUG: First Data Line: '%s'", line.c_str());

    if(line.empty()) {
        file.close();
        return temp;
    }

    // dataParsed[0] = signalName, dataParsed[1] = value, dataParsed[2] = time
    std::array<std::string,3> dataParsed = parseCSVLine(line);
    std::string signalName = dataParsed[0];
    temp[0] = dataParsed[1];

    for (unsigned char i=1;i<10;i++) {
        line = readLine(file);

        if (!line.empty()) {
            // If the recording is shorter than 10 values, repeat the last known value.
            temp[i] = temp[i-1];
            continue;
        }

        dataParsed = parseCSVLine(line);

        if(signalName == dataParsed[0]){
            temp[i] = dataParsed[1];
            continue;
        }
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

std::array<std::string,3> DataBundleManager::parseCSVLine(std::string line) {
    std::array<std::string,3> data;
    
    unsigned char firstSemi = line.find(';');
    unsigned char secondSemi = line.find(';', firstSemi + 1);

    data[0] = (line.substr(0, firstSemi));
    data[1] = (line.substr(firstSemi + 1, secondSemi - firstSemi - 1));
    data[2] = (line.substr(secondSemi + 1));
    
    return data;
}
