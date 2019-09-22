#include <Ethernet.h>
#include <ArduinoMqttClient.h>

#include "arduino_secrets.h"

byte mac[] = { 0x90, 0xA2, 0xDA, 0x10, 0xCC, 0xB8 };

EthernetClient net;
MqttClient mqttClient(net);

const char mqtt_topic_duration[] = MQTT_TOPIC_DURATION;
const char mqtt_topic_distance[] = MQTT_TOPIC_DISTANCE;
const int mqtt_port = 1883;
const int echo_pin = 9;
const int trig_pin = 8;
unsigned int mqtt_counter = 0;

void printIPAddress() { 
  Serial.print(F("My IP address: "));
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    Serial.print(Ethernet.localIP()[thisByte], DEC);
    Serial.print(".");
  }
  Serial.println();
}

void initEthernet() {
  while (Ethernet.begin(mac) != 1) {
    Serial.println(F("Failed to configure Ethernet using DHCP"));
    Serial.println(F("Trying again..."));
    delay(15000);
  }
  printIPAddress();
}

void renewEthernet() {
  switch (Ethernet.maintain())
  { 
    case 1:
      Serial.println(F("Error: renewed fail"));
      break;
    
    case 2:
      Serial.println(F("Renewed success"));
      printIPAddress();
      break;
    
    case 3:
      Serial.println(F("Error: rebind fail"));
      break;
    
    case 4:
      Serial.println(F("Rebind success"));
      printIPAddress();
      break;
    
    default:
      //nothing happened
      break;
  
  }
}

void initMqtt() {
  mqttClient.setUsernamePassword(MQTT_USERNAME, MQTT_PASSWORD);
}

void renewMqtt() {
  while (mqttClient.connect(MQTT_BROKER, mqtt_port) != 1) {
    Serial.println(F("Failed to connect to MQTT broker"));
    Serial.println(F("Trying again..."));
    delay(1000);
  }
}

void setup() {
  Serial.begin(9600);
  initEthernet();
  initMqtt();
  pinMode(trig_pin, OUTPUT);
  pinMode(echo_pin, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {

  renewEthernet();
  renewMqtt();

  digitalWrite (trig_pin, LOW);
  digitalWrite (trig_pin, HIGH);
  delayMicroseconds (10);

  digitalWrite(LED_BUILTIN, HIGH);
  float duration = pulseIn (echo_pin, HIGH);
  digitalWrite(LED_BUILTIN, LOW);

  if (duration > 0.0) {
    Serial.println (duration);
    mqttClient.beginMessage(mqtt_topic_duration);
    mqttClient.print(duration);
    mqttClient.endMessage();

    float distance = duration / 58;
    Serial.println (distance);
    mqttClient.beginMessage(mqtt_topic_distance);
    mqttClient.print(distance);
    mqttClient.endMessage();
  }

  Serial.print(F("MQTT Counter: "));
  Serial.println(mqtt_counter);
  mqtt_counter++;
  
  delay (300000);
}
