
#include "SensorReader.h"
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_BME280.h>
#include <TinyGPSPlus.h>

Adafruit_MPU6050 mpu;
Adafruit_BME280 bme;
TinyGPSPlus gps;
HardwareSerial gpsSerial(2);

float gpsLat, gpsLng, gpsAlt;
float temp, pressure1, altitude1;
float pitch, roll, yaw;
float batteryVoltage;

void initSensors() {
    Wire.begin(21, 22);
    bme.begin(0x76);
    mpu.begin();
    gpsSerial.begin(9600, SERIAL_8N1, 16, 17);
}

void readSensors() {
    sensors_event_t a, g, t;
    mpu.getEvent(&a, &g, &t);
    pitch = a.acceleration.x;
    roll = a.acceleration.y;
    yaw = a.acceleration.z;

    temp = bme.readTemperature();
    pressure1 = bme.readPressure() / 100.0;
    altitude1 = bme.readAltitude(1013.25);

    while (gpsSerial.available()) {
        gps.encode(gpsSerial.read());
    }

    gpsLat = gps.location.lat();
    gpsLng = gps.location.lng();
    gpsAlt = gps.altitude.meters();

    batteryVoltage = analogRead(36) * (3.3 / 4095.0) * 2; // Divider
}
