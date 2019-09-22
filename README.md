# arduino-sonic

A small Arduino program to measure distance with HC-SR04 sonic and send the
result to an MQTT server.

Copy the file ```arduino_secrets.h.example``` to ```arduino_secrets.h``` and
set username, password, and hostname for MQTT and the publishing topics.

Both, the duration in microseconds and the distance in centimeters are
published via MQTT.

To calculate the distance in inches, use 148 instead of 58 in the distance
formula.

Be aware to set the MAC address and the pins according to your setup.
