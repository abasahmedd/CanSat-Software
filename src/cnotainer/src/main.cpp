#include <Wire.h>
#include <Arduino.h>
#include <Adafruit_BMP280.h>
#include <SPI.h>
#include <RF24.h>

Adafruit_BMP280 bmp; // BMP sensor
RF24 radio(9, 10);   // CE, CSN pins for nRF24L01

const byte address[6] = "00001";

void setup() {
  Serial.begin(9600);

  // Initialize BMP280
  if (!bmp.begin()) {
    Serial.println("Failed to find BMP280 sensor!");
    while (1);
  }

  // Initialize nRF24L01
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.setChannel(108);
  radio.openWritingPipe(address);
  radio.stopListening(); // Act as transmitter

  Serial.println("Setup complete.");
}

void loop() {
  float temperature = bmp.readTemperature();
  float pressure = bmp.readPressure() / 100.0F;

  String message = String("T:") + temperature + " C, P:" + pressure + " hPa";
  Serial.println("Sending: " + message);

  radio.write(message.c_str(), message.length() + 1);  // Send as C-string

  delay(1000);
}
