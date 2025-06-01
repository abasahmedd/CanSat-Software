#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>

void setup() {
  Serial.begin(115200);
  while (!Serial);

  if (!LoRa.begin(433E6)) {
    Serial.println("LoRa init failed. Check your connections.");
    while (true);
  }

  Serial.println("LoRa Receiver ready");
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    String received = "";
    while (LoRa.available()) {
      received += (char)LoRa.read();
    }

    Serial.print("Received packet: ");
    Serial.println(received);

    // إذا تحب تحلل البيانات كقيم منفصلة:
    // مثال:
    float time_val, altitude, temp, pressure, gyroX, gyroY, gyroZ, accX, accY, accZ;
    int free_fall;

    sscanf(received.c_str(), "%f,%f,%d,%f,%f,%f,%f,%f,%f,%f,%f",
           &time_val, &altitude, &free_fall, &temp, &pressure,
           &gyroX, &gyroY, &gyroZ, &accX, &accY, &accZ);

    // جرب تطبع بعض القيم كمثال:
    Serial.print("Time: "); Serial.println(time_val);
    Serial.print("Altitude: "); Serial.println(altitude);
    Serial.print("Temperature: "); Serial.println(temp);
  }
}
