#pragma once
// app_config.h – Editable device properties
// -------------------------------------------------------------
// Modify the constants below, then re‑compile & flash.
// -------------------------------------------------------------

/************* Network *************/
constexpr const char* kHostName = "FreshWaterControlAft";
constexpr const char* kWifiSSID       = "";   // Wi‑Fi SSID
constexpr const char* kWifiPassword   = "";   // Wi‑Fi password

/********* Signal K Server *********/
// Leave kSKServerIP empty ("") for auto‑discovery via mDNS.
constexpr const char*  kSKServerIP    = "";        // e.g. "10.10.7.16"
constexpr const uint16_t kSKServerPort = 3000;     // usual SK port

/************* Tank Level Sensors *************/
// Define the GPIO pins for each level sensor
constexpr const uint8_t kLevelPin20 = 34;
constexpr const uint8_t kLevelPin40 = 35;
constexpr const uint8_t kLevelPin60 = 32;
constexpr const uint8_t kLevelPin80 = 33;

// Set to LOW if your DJLK-A6 is NPN (pulls to ground when water detected).
// Set to HIGH if your DJLK-A6 is PNP (outputs voltage when water detected).
constexpr const uint8_t kSensorActiveState = LOW;

constexpr const char* kSignalKPath = "tanks.freshWater.aft.currentLevel";
constexpr const char* kWebUIConfigPath = "/Tanks/FreshWaterAft/Level";


