# WiFi Controlled Light & Temperature Monitor

## Overview
This project controls a light and sends temperature data from a Dallas Temperature sensor to a server using an ESP32 microcontroller. The light is toggled based on server data, and temperature readings are sent every 3 seconds.


## Functions

### `getLight()`
- Sends a GET request to control the light based on the server’s response (`true` or `false`).
- Updates the LED accordingly.

### `putTemperature()`
- Reads the temperature from the sensor.
- Sends the temperature to the server in JSON format.

## Purpose
Written to demonstrate sensor reading, HTTP requests, and server communication using ESP32. It updates light status and sends temperature data to a server.



