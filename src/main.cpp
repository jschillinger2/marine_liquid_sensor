// Signal K application template file.
//
// This application demonstrates core SensESP concepts in a very
// concise manner. You can build and upload the application as is
// and observe the value changes on the serial port monitor.
//
// You can use this source file as a basis for your own projects.
// Remove the parts that are not relevant to you, and add your own code
// for external hardware libraries.

#include <memory>

#include "sensesp.h"
#include "sensesp/sensors/analog_input.h"
#include "sensesp/sensors/digital_input.h"
#include "sensesp/sensors/sensor.h"
#include "sensesp/signalk/signalk_output.h"
#include "sensesp/system/lambda_consumer.h"
#include "sensesp_app_builder.h"

#include "app_config.h"

using namespace sensesp;

// The setup function performs one-time application initialization.
void setup()
{
  SetupLogging(ESP_LOG_DEBUG);

  // 1. Construct the global SensESPApp() object
  SensESPAppBuilder base_builder;
  auto *builder = base_builder.set_hostname(kHostName);

  if (strlen(kWifiSSID) > 0)
  {
    debugD("Setting WiFi credentials - SSID: %s, Password: %s", kWifiSSID, kWifiPassword);
    builder->set_wifi_client(kWifiSSID, kWifiPassword);
  }
  else
    debugD("No WiFi SSID configured");

  if (strlen(kSKServerIP) > 0)
  {
    esp_log_level_set("signalk_ws_client", ESP_LOG_WARN);
    debugD("Setting KServer Manual to IP: %s, Port: %d", kSKServerIP, kSKServerPort);
    builder->set_sk_server(kSKServerIP, kSKServerPort);
  }

  sensesp_app = builder->get_app();

  // 2. Create a RepeatSensor that evaluates the highest active level every 1000ms
  pinMode(kLevelPin20, INPUT_PULLUP);
  pinMode(kLevelPin40, INPUT_PULLUP);
  pinMode(kLevelPin60, INPUT_PULLUP);
  pinMode(kLevelPin80, INPUT_PULLUP);

  const unsigned int kLevelReadInterval = 1000;
  auto water_level_sensor = std::make_shared<RepeatSensor<float>>(
      kLevelReadInterval,
      []() -> float
      {
        if (digitalRead(kLevelPin80) == kSensorActiveState)
        {
          debugD("Sensor 80%% active (Pin %d is LOW)", kLevelPin80);
          return 0.8f;
        }
        if (digitalRead(kLevelPin60) == kSensorActiveState)
        {
          debugD("Sensor 60%% active (Pin %d is LOW)", kLevelPin60);
          return 0.6f;
        }
        if (digitalRead(kLevelPin40) == kSensorActiveState)
        {
          debugD("Sensor 40%% active (Pin %d is LOW)", kLevelPin40);
          return 0.4f;
        }
        if (digitalRead(kLevelPin20) == kSensorActiveState)
        {
          debugD("Sensor 20%% active (Pin %d is LOW)", kLevelPin20);
          return 0.2f;
        }

        // If no sensors are active, the tank is below 20% (or empty)
        debugD("No sensors active. Tank is < 20%% (or empty).");
        return 0.0f;
      });

  // 3. Connect the evaluated level to a Signal K output
  auto level_metadata = std::make_shared<SKMetadata>("ratio", "Fresh Water Tank Level");
  auto level_sk_output = std::make_shared<SKOutput<float>>(
      kSignalKPath,     // Signal K path
      kWebUIConfigPath, // Web UI config path
      level_metadata);

  ConfigItem(level_sk_output)
      ->set_title("Aft Water Tank Level SK Path")
      ->set_sort_order(100);

  water_level_sensor->connect_to(level_sk_output);

  // loop from here.
  while (true)
  {
    loop();
  }
}

void loop() { event_loop()->tick(); }
