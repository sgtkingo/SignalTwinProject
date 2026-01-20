#pragma once
#include <string>
#include <array>

// Metadata for each data bundle
struct BundleMetadata {
    std::string sensorName;  // "DHT11"
    std::string filePath;    // "/DataBundles/data_12345.csv"
    std::string startDate;   // "YYYY-MM-DD"
};

// Used only when loading specific data for a chart
struct DataPoint {
    std::string partName;  // "Temperature"
    std::string value;     // "24.5"
    std::string time;      // "hh:mm:ss"
};

// Buffer that is returned to the GUI with 6 or less current bundles
struct DataBundleBuffer {
    BundleMetadata             metaBuffer;
    std::array<std::string,10> dataBuffer;
    std::string                startTime;
};