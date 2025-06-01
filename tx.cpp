#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>

void setup() {
  Serial.begin(115200);
  while (!Serial);

  LoRa.begin(433E6);  
}

void loop() {
  float time_val = millis() / 1000.0;
  float altitude = 123.6;
  int free_fall = 0;
  float temp = 26.5;
  float pressure = 1013.2;
  float gyroX = 0.05;
  float gyroY = 0.02;
  float gyroZ = 0.01;
  float accX = 1.2;
  float accY = 0.9;
  float accZ = 0.8;

  String packet = String(time_val) + "," + String(altitude) + "," + String(free_fall) + "," +
                  String(temp) + "," + String(pressure) + "," +
                  String(gyroX) + "," + String(gyroY) + "," + String(gyroZ) + "," +
                  String(accX) + "," + String(accY) + "," + String(accZ);

  LoRa.beginPacket();
  LoRa.print(packet);
  LoRa.endPacket();
  Serial.println("Packet sent: " + packet); 
  delay(1000); 
}
