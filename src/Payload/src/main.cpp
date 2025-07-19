#include <Arduino.h>
#include "ServoHandler.h"
#include "SensorReader.h"
#include "SDLogger.h"
#include <LoRa.h>



#define BUZZER_PIN 32  // البن المتصل بالبزر



void setup() {
  Serial.begin(115200);
  initSensors();  // تهيئة الحساسات
  LoRa.setPins(18, 14, 26); // NSS, RST, DIO0 — حسب بورد TTGO Heltec LoRa V2

  if (!LoRa.begin(915E6)) {
    Serial.println("LoRa init failed!");
    while (1);
  }
  Serial.println("LoRa init success");
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);  // بالبداية مطفأ



}

void loop() {
  readSensors();  // يقرأ القيم من كل الحساسات

  Serial.println("PRESSURE1, TEMP, ALTITUDE1, GPS LAT, GPS LNG, GPS ALT, PITCH, ROLL, YAW");

  // بناء الباكيت
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

  // طباعة الباكيت للمراقبة
  Serial.println(packet);

  // إرسال عبر LoRa
  LoRa.beginPacket();
  LoRa.print(packet);
  LoRa.endPacket();

  // شرط الهبوط
if (altitude1 < 10.0) {  // أو gpsAlt < 10.0 حسب الأفضل
  digitalWrite(BUZZER_PIN, HIGH);  // شغّل البزر
} else {
  digitalWrite(BUZZER_PIN, LOW);   // طفيه إذا بعده بالجو
}


  delay(1000);
}
