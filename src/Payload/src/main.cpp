#include <Arduino.h>
#include "ServoHandler.h"
#include "SensorReader.h"
#include "SDLogger.h"
#include <LoRa.h>



#define BUZZER_PIN 32  



void setup() {
  Serial.begin(115200);
  initSensors(); 
  LoRa.setPins(18, 14, 26); 

  if (!LoRa.begin(915E6)) {
    Serial.println("LoRa init failed!");
    while (1);
  }
  Serial.println("LoRa init success");
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW); 



}

void loop() {
  readSensors();  

  Serial.println("PRESSURE1, TEMP, ALTITUDE1, GPS LAT, GPS LNG, GPS ALT, PITCH, ROLL, YAW");

  String packet = "";
  packet += String(pressure1) + "|";
  packet += String(temp) + "|";
  packet += String(altitude1) + "|";
  packet += String(gpsLat, 6) + "|";
  packet += String(gpsLng, 6) + "|";
  packet += String(gpsAlt) + "|";
  packet += String(pitch) + "|";
  packet += String(roll) + "|";
  packet += String(yaw);

  Serial.println(packet);

  LoRa.beginPacket();
  LoRa.print(packet);
  LoRa.endPacket();

if (altitude1 < 10.0) {  
  digitalWrite(BUZZER_PIN, HIGH);  
} else {
  digitalWrite(BUZZER_PIN, LOW);  
}


  delay(1000);
}
