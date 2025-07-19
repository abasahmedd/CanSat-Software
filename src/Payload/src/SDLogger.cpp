
#include "SDLogger.h"
#include <SD.h>
#include <SPI.h>

File dataFile;

void initSDCard() {
    if (!SD.begin(5)) {
        Serial.println("SD Card init failed!");
        return;
    }
    dataFile = SD.open("/log.csv", FILE_WRITE);
    if (dataFile) {
        dataFile.println("ALT1,ALT2,PRES1,PRES2,TEMP,BATT,GPSLAT,GPSLNG,GPSALT,PITCH,ROLL,YAW");
        dataFile.close();
    }
}

void logToSD() {
    extern float gpsLat, gpsLng, gpsAlt;
    extern float temp, pressure2, altitude2;
    extern float pitch, roll, yaw;
    extern float batteryVoltage;

    static float pressure1 = 0.0, altitude1 = 0.0;
    extern bool receivePressure(float*, float*);
    receivePressure(&pressure1, &altitude1);

    dataFile = SD.open("/log.csv", FILE_APPEND);
    if (dataFile) {
        dataFile.printf("%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.6f,%.6f,%.2f,%.2f,%.2f,%.2f\n",
            altitude1, altitude2, pressure1, pressure2, temp, batteryVoltage,
            gpsLat, gpsLng, gpsAlt, pitch, roll, yaw);
        dataFile.close();
    }
}
