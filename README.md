# EZ Waterflow

An automatic watering system built with ESP32.

It monitors soil moisture, checks upcoming rainfall through Plantly (powered by Open-Meteo), automatically waters plants when needed, and saves watering records to Plantsdb.

## Features

- Automatic watering
- Soil moisture monitoring
- Weather-based watering control using Plantly (powered by Open-Meteo)
- Save watering records to Plantsdb

## Technologies

- ESP32
- Arduino
- HTTP
- JSON
- Wi-Fi

## Required Libraries

- WiFi
- WebServer
- HTTPClient
- ArduinoJson

## Setup

Configure the following before uploading the program:

- Wi-Fi SSID and password
- API server IP address for Plantly
- API server IP address for Plantsdb

## License

This project is licensed under the MIT License.