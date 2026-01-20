/**
 * @file data_bundle_manager.hpp
 * @brief bundles made from recording
 *
 * This header defines the manager for data bundles recorded from sensors.
 *
 * @copyright 2025 MTA
 * @author Ondřej Wrubel
 */

#ifndef DATA_BUNDLE_MANAGER_H
#define DATA_BUNDLE_MANAGER_H

#include "data_bundle_types.hpp"
#include "SD.h"

class DataBundleManager {
private:
    bool initialized = false;                 ///< Initialization state flag
    
    std::vector<std::string> DataBundleNames;  ///< All Data Bundle Names saved (DHT11_01.csv)

    BundleMetadata currentBundleMetaData;     ///< Current Bundle that is being recorded
    std::vector<DataPoint> currentBundleData; ///< Current Bundle Data that are being recorded 

    const char* root = "/DataBundles/"; ///<The directory where all databundles are saved

    // HELPERS

    std::string readLine(File &file);

    std::array<std::string,3> parseCSVLine(std::string line);

    // GETTERS

    BundleMetadata getBundleMetaData(unsigned char index);

    // each databundle has as a preview chart with first 10 values from one of the sensor parts
    std::array<std::string,10> getBundleDataValuePreview(unsigned char index);
    

public:
    /**
     * @brief Private constructor for singleton pattern
     */
    DataBundleManager();

    /**
     * @brief Destructor
     */
    ~DataBundleManager();

    /**
     * @brief Initialize the data bundle manager and SD card
     * @return True if initialization was successful, false otherwise
     */
    bool init();

    /**
     * @brief Initialize directories such as DataBundles
     * @return True if init was succesful, false otherwise
     */
    bool initDirectories();

    /**
     * @brief Check if the manager has been initialized
     * @return True if initialized, false otherwise
     */
    bool isInitialized() const { return initialized; }

    /**
     * @brief loads all data bundle names from SD
     * @return True if initialized, false otherwise
     */
    bool loadAllDataBundleNames();

    // *********************
    // Current record events
    // *********************

    bool startRecording(std::string sName);

    // called when updated data comes
    bool saveNewDataPoint(std::string partName, std::string value);

    bool saveRecording();

    void scrapRecording();

    // All DataBundle events

    std::array<DataBundleBuffer,6> getDataBundles(unsigned char page);

    bool deleteAllDataBundles();

    // maybe?

    bool renameDataBundle();

    // Single Databundle events

    void deleteDataBundle(unsigned char index);

    bool exportDataBundle();

    /**
     * @brief Remove the oldest data bundle
     * Called when storage is full
     */
    void removeOldestDataBundle();

    // CHECKER

    /**
     * @brief Check if the data bundle storage is full
     * There are max 30 data bundles allowed to be stored
     * @return True if full, false otherwise
     */
    bool isDataBundleFull();

    // DEBUG

    void listAllBundles();

    void printCSV(std::string filename);

    /**
     * @brief Prints size of SD and its used size in bytes
     */
    void getSDInfo();

    // public GETTERS

    unsigned char getDataBundleAmount() {return DataBundleNames.size();}
};

#endif