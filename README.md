### Project: Motor Control via BLE with ESP32

**Description:**

This project implements a BLE server on the ESP32 to control a motor using commands sent from a client application.

**Functionalities:**
- **BLE Configuration:** Defines UUIDs for the BLE service and read/write characteristics.
- **Motor Control:** Uses an ESP32 pin to turn an engine on and off.
- **Command Decoding:** Decodes commands in Base64 for motor control.
- **BLE Connection:** Waits for connections from a BLE client and processes received commands to change the engine state.

**Technical details:**
- **Motor Pin:** Configured as output on ESP32.
- **BLE Service:** Starts and configures BLE, making the device visible and available for connections.
- **Command Processing:** Interprets the commands "T1" (start engine) and "T0" (turn off engine).

**How ​​it works:**
1. ESP32 starts BLE and waits for connections.
2. When a client connects, it can send Base64 encoded commands.
3. ESP32 decodes these commands and controls the engine according to the received commands.

This project demonstrates skills in BLE, hardware control and data processing on a microcontroller.

### Demonstration Video

[Watch the video](https://youtu.be/BUT6_Ezaww8?si=Hb7LZrK3193k3n62)
