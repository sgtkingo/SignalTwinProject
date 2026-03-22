/**
 * @file data_bundle_manager.hpp
 * @brief Recording bundle manager.
 *
 * This header defines the manager for data bundles recorded from live device signals.
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
    
    std::vector<std::string> DataBundleNames;  ///< All saved bundle filenames (for example DHT11_01.csv)

    BundleMetadata currentBundleMetaData;      ///< Metadata of the active recording session
    std::vector<DataPoint> currentBundleData;  ///< Buffered signal payload of the active recording session

    const char* root = "/DataBundles/"; ///< Directory where all data bundles are stored

    // HELPERS

    std::string readLine(File &file);

    std::array<std::string,3> parseCSVLine(std::string line);

    // GETTERS

    BundleMetadata getBundleMetaData(unsigned char index);

    // Each data bundle preview uses the first 10 values of one recorded signal stream.
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
     * @brief Load all data bundle names from SD card
     * @return True if initialized, false otherwise
     */
    bool loadAllDataBundleNames();

    // **************************
    // Active recording session
    // **************************

    bool startRecording(std::string deviceName);

    // Called whenever a new runtime sample arrives.
    bool saveNewDataPoint(std::string signalName, std::string value);

    bool saveRecording();

    void scrapRecording();

    // *****************
    // Catalog browsing
    // *****************

    std::array<DataBundleBuffer,6> getDataBundles(unsigned char page);

    bool deleteAllDataBundles();

    // maybe?

    bool renameDataBundle();

    // *********************
    // Single bundle events
    // *********************

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

    unsigned char getDataBundleAmount() {
        loadAllDataBundleNames();
        return DataBundleNames.size();
    }
};

#endif
