# ESP-WROOM-32 PlatformIO Project

This project is designed for the **ESP-WROOM-32** microcontroller using the **PlatformIO IDE**. It contains the initial setup, configuration, and basic code to get started with ESP32 development.

## Requirements

* PlatformIO IDE (for VS Code or standalone)
* ESP32 toolchain installed via PlatformIO
* ESP-WROOM-32 development board

## Project Setup

1. **Clone or download the repository**:
   ```bash
   git clone <repository-url>
   ```

2. **Open the project in PlatformIO**:
   * Open the folder containing this project in VS Code or PlatformIO IDE.

3. **Install dependencies**:
   PlatformIO will automatically install the required libraries and dependencies based on the configuration in the `platformio.ini` file when the project is opened.

## Configuration

### `platformio.ini`

The main configuration file (`platformio.ini`) contains:

```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
monitor_speed = 115200
upload_speed = 921600
```

Key settings:
* **Platform**: `espressif32`, for ESP32 devices.
* **Board**: `esp32dev`, default for ESP-WROOM-32.
* **Framework**: Arduino framework, commonly used for ESP32 projects.
* **Monitor speed**: Serial monitor baud rate, set to `115200`.
* **Upload speed**: Upload speed, set to `921600` for faster flashing.

## Usage

### Compiling and Uploading

1. **Connect your ESP-WROOM-32** to your computer via USB.

2. **Build the project**:
   * Click the checkmark (✓) in the PlatformIO toolbar.
   * Or use the terminal:
     ```bash
     pio run
     ```

3. **Upload the code**:
   * Click the right arrow (→) in the toolbar.
   * Or use the terminal:
     ```bash
     pio run --target upload
     ```

4. **Monitor serial output**:
   * Click the plug icon to open the Serial Monitor.
   * Or use the terminal:
     ```bash
     pio device monitor
     ```

## Initial Code

The initial `main.cpp` file provides a simple test, printing "Hello, ESP-WROOM-32!" to the Serial Monitor.

```cpp
#include <Arduino.h>

void setup() {
    Serial.begin(115200);
    Serial.println("Hello, ESP-WROOM-32!");
}

void loop() {
    // Your code here
}
```

## Future Development

This project is set up for future expansion, such as:
* Using **ESPAsyncWebServer** for web requests
* Interfacing with sensors
* Cloud integrations

## License

This project is licensed under the **MIT License**. See the LICENSE file for more details.

## Contributing

Contributions are welcome! Feel free to fork the repository, make improvements, and submit pull requests.
