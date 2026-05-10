/**
 * @file config.h
 * @brief Configuration file for platform-specific settings.
 * 
 * This file defines macros to select the execution environment (Arduino or standard console).
 * Uncomment the desired macro to enable the respective environment.
 * 
 * @copyright 2025 MTA
 * @author Ing. Jiri Konecny
 */

#ifndef CONFIG_ENGINE_H
#define CONFIG_ENGINE_H

/// Uncomment to enable Arduino-based environments
#ifndef ARDUINO_H
#define ARDUINO_H 
#endif

/// Uncomment to enable standard console applications (PC/Linux)
//#define STDIO_H 

// Uncomment to enable LVGL support
#ifndef USE_LVGL
#define USE_LVGL
#endif

// Uncomment to enable ESP32 platform
//#define ESP_PLATFORM

// For debugging and testing purposes, you can enable the following macros:
// #define DATABUNDLE_DEBUG  // For debugging DataBundleManager operations
// #define PROTOCOL_DEBUG    // For debugging Protocol operations

// Storage backend selection for all persistent data access.
#define STORAGE_OPTION_SD 1
#define STORAGE_OPTION_SPIFFS 2

#ifndef STORAGE_OPTION
#define STORAGE_OPTION STORAGE_OPTION_SPIFFS
#endif

// When enabled, SPIFFS will be formatted automatically if mount fails.
#ifndef STORAGE_SPIFFS_FORMAT_ON_FAIL
#define STORAGE_SPIFFS_FORMAT_ON_FAIL 1
#endif

#if STORAGE_OPTION == STORAGE_OPTION_SPIFFS
#define STORAGE_DEFAULT_DEVICE_DB_PATH "/DB.json"
#define STORAGE_SEED_DEFAULT_DB_ON_MISSING 1
#else
#define STORAGE_DEFAULT_DEVICE_DB_PATH "/data/DB.json"
#define STORAGE_SEED_DEFAULT_DB_ON_MISSING 0
#endif

// SD card wiring used by the display board.
#ifndef STORAGE_SD_PIN_MOSI
#define STORAGE_SD_PIN_MOSI 11
#endif

#ifndef STORAGE_SD_PIN_MISO
#define STORAGE_SD_PIN_MISO 13
#endif

#ifndef STORAGE_SD_PIN_SCK
#define STORAGE_SD_PIN_SCK 12
#endif

#ifndef STORAGE_SD_PIN_CS
#define STORAGE_SD_PIN_CS 10
#endif

#endif // CONFIG_ENGINE_H
