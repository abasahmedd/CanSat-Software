
#ifndef SENSOR_READER_H
#define SENSOR_READER_H

void initSensors();
void readSensors();

extern float gpsLat, gpsLng, gpsAlt;
extern float temp, pressure1, altitude1;
extern float pitch, roll, yaw;
extern float batteryVoltage;

#endif
