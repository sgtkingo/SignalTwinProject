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

#endif // CONFIG_ENGINE_H