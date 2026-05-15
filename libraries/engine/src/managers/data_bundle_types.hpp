#pragma once
#include <string>
#include <array>

// Metadata for each data bundle
struct BundleMetadata {
    std::string deviceName;  // "DHT11"
    std::string deviceUid;   // "cpu_temp"
    std::string filePath;    // "/records/data_12345.csv"
    std::string startDate;   // "YYYY-MM-DD"
};

// Used only when loading specific data for a chart
struct DataPoint {
    std::string deviceName;       // "CPU Temp"
    std::string deviceUid;        // "cpu_temp"
    unsigned long sampleIndex;    // monotonic sample number in this recording
    unsigned long runtimeMs;      // milliseconds since recording start
    std::string signalName;       // "Temperature"
    std::string value;            // "24.5"
};

// Buffer that is returned to the GUI with 6 or less current bundles
struct DataBundleBuffer {
    BundleMetadata             metaBuffer;
    std::array<std::string,10> dataBuffer;
    std::string                startTime;
};
