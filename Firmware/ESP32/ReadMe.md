Both versions of the ESP32 firmware contain an over the air update facility, however it has proved unreliable in testing.

The ESP32 firmware version 3.0 and above incorporates firmware version reporting. In order to establish which ESP32 firmware version you have, the DCU can be sent a status command. If the status command returns status information that gives the DCU firmware version then it is NOT version 3.0 or lower.