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
#include "storage_manager.hpp"
#include <vector>

class DataBundleManager {
private:
    bool initialized = false;                 ///< Initialization state flag
    bool bundleFileNamesLoaded = false;       ///< True when bundleFileNames mirrors storage
    
    std::vector<std::string> bundleFileNames;  ///< All saved bundle filenames (for example DHT11_01.csv)

    BundleMetadata recordingBundleMetadata;      ///< Metadata of the active recording session
    std::vector<DataPoint> recordingDataPoints;  ///< Buffered signal payload of the active recording session
    unsigned long recordingStartMs = 0;          ///< Runtime timestamp when active recording started
    unsigned long recordingSampleCounter = 0;    ///< Monotonic sample counter for active recording

    const char* root = "/DataBundles/"; ///< Directory where all data bundles are stored

    // HELPERS

    std::string readLine(File &file);

    std::vector<std::string> parseCSVLine(std::string line);

    void clearRecordingState(const char *reason);

    // GETTERS

    BundleMetadata getBundleMetadata(unsigned char index);

    // Each data bundle preview uses the first 10 values of one recorded signal stream.
    std::array<std::string,10> getBundleValuePreview(unsigned char index);
    

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
     * @brief Initialize the data bundle manager and selected storage backend
     * @return True if initialization was successful, false otherwise
     */
    bool init();

    /**
     * @brief Initialize directories such as DataBundles
     * @return True if init was succesful, false otherwise
     */
    bool ensureStorageDirectories();

    /**
     * @brief Check if the manager has been initialized
     * @return True if initialized, false otherwise
     */
    bool isInitialized() const { return initialized; }

    /**
     * @brief Load all data bundle names from storage
     * @return True if initialized, false otherwise
     */
    bool reloadBundleFileNames();

    // **************************
    // Active recording session
    // **************************

    bool startRecording(std::string deviceName, std::string deviceUid);

    // Called whenever a new runtime sample arrives.
    bool saveNewDataPoint(std::string signalName, std::string value);

    bool hasRecordingData() const { return !recordingDataPoints.empty(); }

    bool saveRecording();

    void discardRecording();

    // *****************
    // Catalog browsing
    // *****************

    std::array<DataBundleBuffer,6> getBundlePage(unsigned char page);

    bool deleteAllBundles();

    // maybe?

    bool renameBundle();

    // *********************
    // Single bundle events
    // *********************

    void deleteBundle(unsigned char index);

    bool exportBundle();

    /**
     * @brief Remove the oldest data bundle
     * Called when storage is full
     */
    void pruneOldestBundle();

    // CHECKER

    /**
     * @brief Check if the data bundle storage is full
     * There are max 30 data bundles allowed to be stored
     * @return True if full, false otherwise
     */
    bool isBundleStorageFull();

    // DEBUG

    void listAllBundles();

    void printBundleCsv(std::string filename);

    /**
     * @brief Prints size of the active storage and its used size in bytes
     */
    void getStorageInfo();

    // public GETTERS

    unsigned char getDataBundleAmount() {
        if (!bundleFileNamesLoaded) {
            reloadBundleFileNames();
        }
        return bundleFileNames.size();
    }
};

#endif
