# SensESP Fresh Water Tank Level Sensor

This project uses an ESP32 microcontroller and SensESP to monitor a fresh water tank's level using four discrete digital liquid level sensors (DJLK-A6). The highest active sensor determines the tank's fill percentage (20%, 40%, 60%, 80%), and the data is seamlessly published to a Signal K server on your boat.

## Overview
- **Framework:** [SensESP](https://github.com/SignalK/SensESP) (ESP-IDF / Arduino)
- **Hardware:** ESP32 Development Board (ESP-32S / NodeMCU-32S)
- **Sensors:** 4x DJLK-A6 non-contact digital liquid level sensors
- **Output:** Signal K standard tank ratio (`0.0` to `0.8`)

## Hardware & Wiring

### Pin Configuration
By default, the sensors are mapped to the following GPIO pins on the ESP32. You can change these in `app_config.h`.

| Tank Level | ESP32 GPIO Pin | Sensor Location |
| :--- | :--- | :--- |
| **80%** | `GPIO 33` | Top sensor |
| **60%** | `GPIO 32` | Upper-middle sensor |
| **40%** | `GPIO 35` | Lower-middle sensor |
| **20%** | `GPIO 34` | Bottom sensor |

*Note: Pins 34 and 35 are input-only pins on the ESP32 and do not have internal pull-up resistors. If you are using NPN open-collector sensors, you must add external pull-up resistors (e.g., 10kΩ to 3.3V) to these specific pins.*

### ⚠️ Sensor Voltage Warning
The DJLK-A6 sensors typically operate at 5V - 24V. **The ESP32 logic pins are NOT 5V tolerant (Max 3.3V).**
* **If using NPN (Active LOW) sensors:** The sensor pulls the signal to Ground. Use pull-up resistors to 3.3V. This is safe for the ESP32.
* **If using PNP (Active HIGH) sensors:** The sensor outputs its supply voltage (5V+) when triggered. **Do not connect this directly to the ESP32.** You must use a voltage divider or an optocoupler to step the signal down to 3.3V.

## Software Configuration

All user-configurable properties are located in `app_config.h`. 

1. **Network Credentials:** Update `kWifiSSID` and `kWifiPassword`.
2. **Signal K Server:** Set `kSKServerIP` to your server's IP (e.g., `192.168.31.61`). Leave it empty (`""`) to enable mDNS auto-discovery.
3. **Sensor Type:** Set `kSensorActiveState` to `LOW` for NPN sensors or `HIGH` for PNP sensors.
4. **Signal K Path:** The default output path is `tanks.freshWater.aft.currentLevel`.

## Building and Flashing (PlatformIO)

This project is built using PlatformIO. Because of the ESP-IDF backend, ensure your `platformio.ini` environment is set to `esp32dev_espidf`.

1. Connect your ESP32 via USB.
2. Build and upload the project using PlatformIO:
   ```bash
   pio run -e esp32dev_espidf -t upload
   ```
3. **Important Note for Flashing:** If the upload hangs at `Connecting........`, hold down the **BOOT** button on your ESP32 board for 1-2 seconds until the flashing process begins. *(Alternatively, solder a 10µF capacitor between the EN pin and GND to enable auto-reset).*

## Signal K Integration

Once connected to your network, the ESP32 will automatically send data to the configured Signal K server. 

- **Data Format:** The tank level is sent as a ratio (`0.0` = Empty, `1.0` = Full). With 4 sensors, the possible outputs are `0.0`, `0.2`, `0.4`, `0.6`, and `0.8`.
- **Web UI:** You can adjust the Signal K path directly from the SensESP Web UI by navigating to the ESP32's local IP address in your browser.

## Troubleshooting

- **"No Signal K server found in network when using mDNS service!"**
  This warning is normal if you have hardcoded the Signal K Server IP in `app_config.h`. The system falls back to the manual IP, and this warning is suppressed in the code.
- **Always reading 80% (or getting erratic readings):**
  Check your sensor active state (`kSensorActiveState`). If you are using NPN sensors on pins 34/35 without external pull-up resistors, the pins will "float" and cause false readings. 