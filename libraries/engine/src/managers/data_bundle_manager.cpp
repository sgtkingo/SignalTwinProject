/**
 * @file data_bundle_manager.hpp
 * @brief bundles made from recording
 *
 * This header defines the manager for data bundles recorded from sensors.
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
 * @brief Initialize the data bundle manager and SD card
 * Also calls initDirectories and prints SD info
 * @return True if initialization was successful, false otherwise
 */
bool DataBundleManager::init()
{
    if (initialized)
        return true;

    logMessage("Initializing DataBundle Manager...");


    SPI.begin(12, 13, 11, 10);
    if (!SD.begin(10))
    {
        logMessage("DataBundle Manager Failed to initialize");
        return false;
    }

    if (!initDirectories())
    {
        logMessage("Error: dir DataBundles failed to create");
    }

    logMessage("DataBundle Manager initialized successfully");

    #ifdef VISENSORS_DEBUG
    getSDInfo();
    listAllBundles();
    #endif

    return true;
}

/**
 * @brief Initialize DataBundles directory
 * @return True if init was succesful, false otherwise
 */
bool DataBundleManager::initDirectories()
{
    if (!SD.exists(root))
    {
        if (!SD.mkdir(root))
        {
            logMessage("Error: Failed to create /DataBundles directory");
            return false;
        }
        //logMessage("Created /DataBundles directory");
    }

    #ifdef VISENSORS_DEBUG
    // log.txt creation test
    File myFile = SD.open("/DataBundles/log.txt", FILE_WRITE);

    if (myFile)
    {
        myFile.println("Sensor Data: 123");
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

void DataBundleManager::getSDInfo()
{
    uint64_t total = SD.totalBytes();
    uint64_t used = SD.usedBytes();

    logMessage("Total Bytes: %llu", total);
    logMessage("Used Bytes: %llu", used);

    (SD.exists("/DataBundles/log.txt")) ? logMessage("log.txt exists!") : logMessage("log.txt doesnt exist");
}

bool DataBundleManager::startRecording(std::string sensorName)
{
    currentBundleMetaData.sensorName = sensorName;

    uint8_t tempOrder = 1;
    std::string temp = root + sensorName + "_0" + std::to_string(tempOrder) + ".csv";
    while (SD.exists(temp.c_str()))
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

    currentBundleMetaData.filePath = temp;

    // to be implemented
    //currentBundleMetaData.startDate
    
    return true;
}

bool DataBundleManager::saveNewDataPoint(std::string partName, std::string value)
{
    // to be implemented - time
    DataPoint temp = {partName, value, ""};
    currentBundleData.push_back(temp);
    return true;
}

bool DataBundleManager::saveRecording()
{
    File saved = SD.open(currentBundleMetaData.filePath.c_str(), FILE_WRITE);

    if (saved)
    {
        saved.println("PartName;Value;Time");
        for (unsigned int i = 0; i < currentBundleData.size(); i++)
        {
            saved.printf("%s;%s;%s\n", currentBundleData[i].partName.c_str(), currentBundleData[i].value.c_str(), currentBundleData[i].time.c_str());
        }

        saved.close(); // Save and close

        if(isDataBundleFull()){
        removeOldestDataBundle();
        }

        //logMessage("Created %s successfully", currentBundleMetaData.filePath.c_str());
    }
    else
    {
        logMessage("Error: Failed to create %s", currentBundleMetaData.filePath.c_str());
        return false;
    }

    listAllBundles();
    printCSV(currentBundleMetaData.filePath);

    return true;
}

void DataBundleManager::scrapRecording()
{
    currentBundleMetaData.sensorName = "";
    currentBundleMetaData.filePath = "";
    currentBundleMetaData.startDate = "";
    currentBundleData.clear();
}

std::array<DataBundleBuffer,6> DataBundleManager::getDataBundles(unsigned char page)
{
    std::array<DataBundleBuffer,6> buff;
    for(unsigned char i=0;i<6&&i<DataBundleNames.size()-(6*page);i++){
        buff[i].metaBuffer = getBundleMetaData(i+(page*6));
        buff[i].dataBuffer = getBundleDataValuePreview(i+(page*6));
    }
    return buff;
}

bool DataBundleManager::deleteAllDataBundles()
{
    File dir = SD.open(root);
    if (!dir || !dir.isDirectory())
        return false;

    std::vector<std::string> filesToDelete;

    dir.rewindDirectory();

    while (true)
    {
        File entry = dir.openNextFile();
        if (!entry)
            break;

        std::string fullPath = root;
        fullPath += entry.name();

        filesToDelete.push_back(fullPath);
        entry.close();
    }
    dir.close();

    for (const auto &file : filesToDelete)
    {
        SD.remove(file.c_str());
    }

    DataBundleNames.clear();

    return true;
}

bool DataBundleManager::loadAllDataBundleNames()
{
    DataBundleNames.clear();

    File dir = SD.open(root);
    if (!dir || !dir.isDirectory()) {
        logMessage("Error: Failed to open /DataBundles/ directory whilst getting bundle names");
        return false;
    }

    dir.rewindDirectory();

    while (true)
    {
        std::string fileName = dir.getNextFileName().c_str();
        if (fileName.empty())
            break;

        fileName = fileName.substr(strlen(root));   

        DataBundleNames.push_back(fileName);
    }
    dir.close();

    return true;
}

void DataBundleManager::removeOldestDataBundle()
{
    File dir = SD.open(root);
    if (!dir || !dir.isDirectory())
        return;

    dir.rewindDirectory();

    // After rewinding, the first file is the oldest
    File oldestFile = dir.openNextFile();
    if(!oldestFile)
    {
        dir.close();
        return; 
    }
    else{
        SD.remove(oldestFile.name());
    }
}

void DataBundleManager::listAllBundles()
{
    logMessage("--- Listing Files in /DataBundles ---");

    File dir = SD.open(root);

    if (!dir)
    {
        logMessage("Error: Failed to open directory /DataBundles");
        return;
    }

    if (!dir.isDirectory())
    {
        logMessage("Error: /DataBundles is not a directory");
        dir.close();
        return;
    }

    dir.rewindDirectory();

    File file = dir.openNextFile();

    while (file)
    {
        if (file.isDirectory())
        {
            logMessage("  [DIR]  %s", file.name());
        }
        else
        {
            logMessage("  [FILE] %s  (%u bytes)", file.name(), file.size());
        }

        file.close();
        file = dir.openNextFile();
    }

    dir.close();
    logMessage("--- End of List ---");
}

void DataBundleManager::printCSV(std::string filename)
{
    std::string fullPath = std::string(root) + filename;

    logMessage("--- Reading CSV: %s ---", fullPath.c_str());

    File file = SD.open(fullPath.c_str(), FILE_READ);

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

BundleMetadata DataBundleManager::getBundleMetaData(unsigned char index){
    std::string fullPath = root + DataBundleNames[index];

    File file = SD.open(fullPath.c_str(), FILE_READ);

    if (!file)
    {
        logMessage("Error: Could not open file %s", fullPath.c_str());
        return {"","",""};
    }

    std::string fileName = file.name();
    std::string sensorName = fileName.substr(0,fileName.find("_"));

    return {sensorName,fullPath,""};
}

std::array<std::string,10> DataBundleManager::getBundleDataValuePreview(unsigned char index){
    std::string fullPath = std::string(root) + DataBundleNames[index];

    //logMessage("Full path to the file is %s",fullPath.c_str());
    std::array<std::string,10> temp;
    for(int k=0; k<10; k++) temp[k] = "0";
    
    File file = SD.open(fullPath.c_str(), FILE_READ);
    
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

    // dataParsed[0] = sensorPart, dataParsed[1] = value, dataParsed[2] = time
    std::array<std::string,3> dataParsed = parseCSVLine(line);
    std::string sensorPart = dataParsed[0];
    temp[0] = dataParsed[1];

    for (unsigned char i=1;i<10;i++) {
        line = readLine(file);

        if (!line.empty()) {
            // if record is smaller than 10 values we repeat the last recorded value
            temp[i] = temp[i-1];
            continue;
        }

        dataParsed = parseCSVLine(line);

        if(sensorPart == dataParsed[0]){
            temp[i] = dataParsed[1];
            continue;
        }
    }

    file.close();   
    return temp;
}

bool DataBundleManager::isDataBundleFull(){
    return (DataBundleNames.size()>=30)? 1 : 0;
}

void DataBundleManager::deleteDataBundle(unsigned char index){
    if(index >= DataBundleNames.size())
        return;

    std::string fullPath = std::string(root) + DataBundleNames[index];
    SD.remove(fullPath.c_str());
    DataBundleNames.erase(DataBundleNames.begin() + index);
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